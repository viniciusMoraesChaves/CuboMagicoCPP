// Interface visual do Cubo Magico 2x2 — reescrita do zero (SDL2 puro, sem TTF).
// Layout: header + 3 colunas (algoritmo | cubo | acoes) + footer de status.
#include "interface.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <map>
#include <random>

#include "astar.hpp"
#include "largura.hpp"
#include "profundidade.hpp"

namespace {

// Forward: posiciona os widgets (definida junto da logica, abaixo).
void layoutWidgets(InterfaceEstado& e);

// ---------------------------------------------------------------- paleta
const SDL_Color BG      = {20, 22, 29, 255};
const SDL_Color PANEL   = {30, 34, 48, 255};
const SDL_Color PANEL2  = {37, 42, 60, 255};
const SDL_Color BORDER  = {56, 64, 88, 255};
const SDL_Color TEXT    = {237, 239, 245, 255};
const SDL_Color MUTED   = {154, 163, 181, 255};
const SDL_Color ACCENT  = {76, 141, 255, 255};
const SDL_Color GREEN   = {53, 194, 110, 255};
const SDL_Color ORANGE  = {255, 159, 67, 255};
const SDL_Color REDC    = {255, 90, 90, 255};
const SDL_Color YELLOW  = {255, 213, 79, 255};
const SDL_Color SHADOW  = {0, 0, 0, 90};

const SDL_Color STICKER[6] = {
    {240, 240, 245, 255},  // 0 branco (U)
    {255, 213, 45, 255},   // 1 amarelo (D)
    {45, 200, 110, 255},   // 2 verde (F)
    {70, 130, 255, 255},   // 3 azul (B)
    {255, 150, 50, 255},   // 4 laranja (L)
    {240, 70, 70, 255},    // 5 vermelho (R)
};
const char* NOME_COR[6] = {"BRANCO", "AMARELO", "VERDE", "AZUL", "LARANJA", "VERMELHO"};

// ---------------------------------------------------------------- fonte bitmap 5x7
using Glifo = std::vector<std::string>;

std::map<char, Glifo> construirFonte() {
    std::map<char, Glifo> f;
    auto A = [&](char c, Glifo g) { f[c] = g; };
    A('A', {".###.", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"});
    A('B', {"####.", "#...#", "#...#", "####.", "#...#", "#...#", "####."});
    A('C', {".####", "#....", "#....", "#....", "#....", "#....", ".####"});
    A('D', {"####.", "#...#", "#...#", "#...#", "#...#", "#...#", "####."});
    A('E', {"#####", "#....", "#....", "####.", "#....", "#....", "#####"});
    A('F', {"#####", "#....", "#....", "####.", "#....", "#....", "#...."});
    A('G', {".####", "#....", "#....", "#..##", "#...#", "#...#", ".###."});
    A('H', {"#...#", "#...#", "#...#", "#####", "#...#", "#...#", "#...#"});
    A('I', {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "#####"});
    A('J', {"..###", "...#.", "...#.", "...#.", "...#.", "#..#.", ".##.."});
    A('K', {"#...#", "#..#.", "#.#..", "##...", "#.#..", "#..#.", "#...#"});
    A('L', {"#....", "#....", "#....", "#....", "#....", "#....", "#####"});
    A('M', {"#...#", "##.##", "#.#.#", "#.#.#", "#...#", "#...#", "#...#"});
    A('N', {"#...#", "##..#", "##..#", "#.#.#", "#..##", "#..##", "#...#"});
    A('O', {".###.", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."});
    A('P', {"####.", "#...#", "#...#", "####.", "#....", "#....", "#...."});
    A('Q', {".###.", "#...#", "#...#", "#...#", "#.#.#", "#..#.", ".#.##"});
    A('R', {"####.", "#...#", "#...#", "####.", "#.#..", "#..#.", "#...#"});
    A('S', {".####", "#....", "#....", ".###.", "....#", "....#", "####."});
    A('T', {"#####", "..#..", "..#..", "..#..", "..#..", "..#..", "..#.."});
    A('U', {"#...#", "#...#", "#...#", "#...#", "#...#", "#...#", ".###."});
    A('V', {"#...#", "#...#", "#...#", "#...#", "#...#", ".#.#.", "..#.."});
    A('W', {"#...#", "#...#", "#...#", "#.#.#", "#.#.#", "##.##", "#...#"});
    A('X', {"#...#", "#...#", ".#.#.", "..#..", ".#.#.", "#...#", "#...#"});
    A('Y', {"#...#", "#...#", ".#.#.", "..#..", "..#..", "..#..", "..#.."});
    A('Z', {"#####", "....#", "...#.", "..#..", ".#...", "#....", "#####"});
    A('0', {".###.", "#..##", "#.#.#", "#.#.#", "##..#", "#...#", ".###."});
    A('1', {"..#..", ".##..", "..#..", "..#..", "..#..", "..#..", ".###."});
    A('2', {".###.", "#...#", "....#", "...#.", "..#..", ".#...", "#####"});
    A('3', {"#####", "...#.", "..#..", "...#.", "....#", "#...#", ".###."});
    A('4', {"...#.", "..##.", ".#.#.", "#..#.", "#####", "...#.", "...#."});
    A('5', {"#####", "#....", "####.", "....#", "....#", "#...#", ".###."});
    A('6', {"..##.", ".#...", "#....", "####.", "#...#", "#...#", ".###."});
    A('7', {"#####", "....#", "...#.", "..#..", "..#..", "..#..", "..#.."});
    A('8', {".###.", "#...#", "#...#", ".###.", "#...#", "#...#", ".###."});
    A('9', {".###.", "#...#", "#...#", ".####", "....#", "...#.", ".##.."});
    A(' ', {".....", ".....", ".....", ".....", ".....", ".....", "....."});
    A('-', {".....", ".....", ".....", "#####", ".....", ".....", "....."});
    A('\'', {"..#..", "..#..", ".....", ".....", ".....", ".....", "....."});
    A('*', {".....", "#.#.#", ".###.", "#####", ".###.", "#.#.#", "....."});
    A(':', {".....", ".....", "..#..", ".....", ".....", "..#..", "....."});
    A('.', {".....", ".....", ".....", ".....", ".....", "..#..", "..#.."});
    A(',', {".....", ".....", ".....", ".....", "..#..", "..#..", ".#..."});
    A('/', {"....#", "....#", "...#.", "..#..", ".#...", "#....", "#...."});
    A('(', {"...#.", "..#..", ".#...", ".#...", ".#...", "..#..", "...#."});
    A(')', {".#...", "..#..", "...#.", "...#.", "...#.", "..#..", ".#..."});
    A('[', {".###.", ".#...", ".#...", ".#...", ".#...", ".#...", ".###."});
    A(']', {".###.", "...#.", "...#.", "...#.", "...#.", "...#.", ".###."});
    A('>', {".#...", "..#..", "...#.", "....#", "...#.", "..#..", ".#..."});
    A('<', {"...#.", "..#..", ".#...", "#....", ".#...", "..#..", "...#."});
    A('_', {".....", ".....", ".....", ".....", ".....", ".....", "#####"});
    A('!', {"..#..", "..#..", "..#..", "..#..", "..#..", ".....", "..#.."});
    A('+', {".....", "..#..", "..#..", "#####", "..#..", "..#..", "....."});
    A('=', {".....", ".....", "#####", ".....", "#####", ".....", "....."});
    A('?', {".###.", "#...#", "....#", "...#.", "..#..", ".....", "..#.."});
    A('%', {"##..#", "##..#", "...#.", "..#..", ".#...", "#..##", "#..##"});
    A('|', {"..#..", "..#..", "..#..", "..#..", "..#..", "..#..", "..#.."});
    return f;
}

const std::map<char, Glifo>& fonte() {
    static std::map<char, Glifo> f = construirFonte();
    return f;
}

int textoLargura(const std::string& t, int esc) { return (int)t.size() * 6 * esc - esc; }

void texto(SDL_Renderer* r, int x, int y, const std::string& s, int esc, SDL_Color cor) {
    if (!r) return;
    SDL_SetRenderDrawColor(r, cor.r, cor.g, cor.b, cor.a);
    int cx = x;
    for (char raw : s) {
        char ch = (char)std::toupper((unsigned char)raw);
        auto it = fonte().find(ch);
        if (it == fonte().end()) {
            cx += 6 * esc;
            continue;
        }
        const Glifo& g = it->second;
        for (int row = 0; row < 7; ++row)
            for (int col = 0; col < 5; ++col)
                if (g[row][col] == '#') {
                    SDL_Rect px = {cx + col * esc, y + row * esc, esc, esc};
                    SDL_RenderFillRect(r, &px);
                }
        cx += 6 * esc;
    }
}

void textoCentro(SDL_Renderer* r, int cx, int y, const std::string& s, int esc, SDL_Color cor) {
    texto(r, cx - textoLargura(s, esc) / 2, y, s, esc, cor);
}

// ---------------------------------------------------------------- primitivas
void setCor(SDL_Renderer* r, SDL_Color c) { SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a); }

void fillCircle(SDL_Renderer* r, int cx, int cy, int raio) {
    for (int dy = -raio; dy <= raio; ++dy) {
        int dx = (int)std::sqrt((double)(raio * raio - dy * dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void fillRounded(SDL_Renderer* r, int x, int y, int w, int h, int rad, SDL_Color c) {
    setCor(r, c);
    rad = std::min(rad, std::min(w, h) / 2);
    SDL_Rect mid = {x, y + rad, w, h - 2 * rad};
    SDL_RenderFillRect(r, &mid);
    SDL_Rect mid2 = {x + rad, y, w - 2 * rad, h};
    SDL_RenderFillRect(r, &mid2);
    fillCircle(r, x + rad, y + rad, rad);
    fillCircle(r, x + w - rad - 1, y + rad, rad);
    fillCircle(r, x + rad, y + h - rad - 1, rad);
    fillCircle(r, x + w - rad - 1, y + h - rad - 1, rad);
}

void strokeRounded(SDL_Renderer* r, int x, int y, int w, int h, int rad, SDL_Color c) {
    setCor(r, c);
    rad = std::min(rad, std::min(w, h) / 2);
    SDL_RenderDrawLine(r, x + rad, y, x + w - rad, y);
    SDL_RenderDrawLine(r, x + rad, y + h - 1, x + w - rad, y + h - 1);
    SDL_RenderDrawLine(r, x, y + rad, x, y + h - rad);
    SDL_RenderDrawLine(r, x + w - 1, y + rad, x + w - 1, y + h - rad);
    // cantos: pequenos arcos por pontos
    for (int a = 0; a <= 12; ++a) {
        double t = a * M_PI / 2.0 / 12.0;
        int dx = (int)(rad * std::cos(t)), dy = (int)(rad * std::sin(t));
        SDL_RenderDrawPoint(r, x + rad - dx, y + rad - dy);
        SDL_RenderDrawPoint(r, x + w - rad + dx - 1, y + rad - dy);
        SDL_RenderDrawPoint(r, x + rad - dx, y + h - rad + dy - 1);
        SDL_RenderDrawPoint(r, x + w - rad + dx - 1, y + h - rad + dy - 1);
    }
}

void card(SDL_Renderer* r, int x, int y, int w, int h, const std::string& titulo) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    fillRounded(r, x + 3, y + 4, w, h, 12, SHADOW);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    fillRounded(r, x, y, w, h, 12, PANEL);
    strokeRounded(r, x, y, w, h, 12, BORDER);
    if (!titulo.empty()) {
        texto(r, x + 14, y + 10, titulo, 1, MUTED);
        setCor(r, BORDER);
        SDL_RenderDrawLine(r, x + 12, y + 26, x + w - 12, y + 26);
    }
}

// Mistura cor com um fundo escuro (p/ estados de seleção): k = peso da cor (0..255).
inline SDL_Color tinge(SDL_Color c, int div, int add) {
    return {(Uint8)(c.r / div + add), (Uint8)(c.g / div + add), (Uint8)(c.b / div + add), 255};
}

bool dentro(int mx, int my, const Botao& b) {
    return mx >= b.x && mx <= b.x + b.largura && my >= b.y && my <= b.y + b.altura;
}

// Botão moderno com estados. `acento` = cor principal; `selecionado` p/ algoritmo ativo.
void botao(SDL_Renderer* r, const Botao& b, SDL_Color acento, bool selecionado, bool hover,
           bool pressionado, bool desabilitado) {
    SDL_Color fundo = PANEL2, borda = BORDER, txt = TEXT;
    if (desabilitado) {
        fundo = {30, 32, 40, 255};
        txt = {100, 106, 122, 255};
    } else if (selecionado) {
        fundo = tinge(acento, 4, 22);
        borda = acento;
    } else if (pressionado && hover) {
        fundo = tinge(acento, 3, 16);
        borda = acento;
    } else if (hover) {
        fundo = {48, 54, 76, 255};
        borda = {90, 100, 135, 255};
    }
    if (!desabilitado) {
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
        fillRounded(r, b.x + 2, b.y + 3, b.largura, b.altura, 10, SHADOW);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    }
    fillRounded(r, b.x, b.y, b.largura, b.altura, 10, fundo);
    if (selecionado && !desabilitado) {
        // barra lateral de destaque
        fillRounded(r, b.x, b.y, 5, b.altura, 2, acento);
    }
    strokeRounded(r, b.x, b.y, b.largura, b.altura, 10, borda);
    // faixa de acento no topo p/ botões de ação
    if (!desabilitado && !selecionado) {
        SDL_Rect faixa = {b.x + 10, b.y, b.largura - 20, 3};
        setCor(r, acento);
        SDL_RenderFillRect(r, &faixa);
    }
    textoCentro(r, b.x + b.largura / 2 - (b.dica.empty() ? 0 : 14), b.y + b.altura / 2 - 8, b.texto, 2,
                txt);
    if (!b.dica.empty()) {
        int tw = textoLargura(b.texto, 2);
        texto(r, b.x + b.largura / 2 + tw / 2 - 12 + 16, b.y + b.altura / 2 - 4, b.dica, 1, MUTED);
    }
}

// ---------------------------------------------------------------- cubo
const int QUAD = 32, QGAP = 4;

void sticker(SDL_Renderer* r, int x, int y, int cor) {
    if (cor < 0 || cor > 5) cor = 0;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_Rect sh = {x + 2, y + 3, QUAD, QUAD};
    setCor(r, SHADOW);
    SDL_RenderFillRect(r, &sh);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    fillRounded(r, x, y, QUAD, QUAD, 8, STICKER[cor]);
    strokeRounded(r, x, y, QUAD, QUAD, 8, {15, 17, 24, 255});
    // brilho no topo
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_Rect brilho = {x + 6, y + 4, QUAD - 12, 5};
    setCor(r, {255, 255, 255, 70});
    SDL_RenderFillRect(r, &brilho);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

void faceCubo(SDL_Renderer* r, const Cubo& cubo, int face, int ox, int oy, const std::string& nome,
              bool destaque) {
    int bloco = 2 * QUAD + QGAP;
    // fundo da face
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    fillRounded(r, ox - 8 + 2, oy - 8 + 3, bloco + 16, bloco + 16, 10, SHADOW);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    fillRounded(r, ox - 8, oy - 8, bloco + 16, bloco + 16, 10, PANEL2);
    strokeRounded(r, ox - 8, oy - 8, bloco + 16, bloco + 16, 10,
                  destaque ? YELLOW : BORDER);
    if (destaque) {
        // halo pulsante simples: segunda borda deslocada
        strokeRounded(r, ox - 10, oy - 10, bloco + 20, bloco + 20, 12, YELLOW);
    }
    for (int i = 0; i < 4; ++i) {
        int cor = cubo.faces[face * 4 + i];
        sticker(r, ox + (i % 2) * (QUAD + QGAP), oy + (i / 2) * (QUAD + QGAP), cor);
    }
    textoCentro(r, ox + bloco / 2, oy + bloco + 10, nome, 1, MUTED);
}

// ---------------------------------------------------------------- logica
bool ehInverso(const std::string& a, const std::string& b) {
    return (a == "U" && b == "U'") || (a == "U'" && b == "U") || (a == "R" && b == "R'") ||
           (a == "R'" && b == "R") || (a == "F" && b == "F'") || (a == "F'" && b == "F");
}

void confirmarSeed(InterfaceEstado& e) {
    if (!e.seedEditando) return;
    e.seedEditando = false;
    if (!e.seedBuffer.empty()) {
        try {
            e.seed = (unsigned int)std::stoul(e.seedBuffer);
        } catch (...) {
        }
    }
    std::cout << "[UI] Seed: " << e.seed << std::endl;
}

void fazerEmbaralhar(InterfaceEstado& e, Cubo& cubo) {
    // Engine recriado a cada chamada: mesma seed => mesma sequencia => mesmo cubo.
    std::mt19937 rng{e.seed};
    static const std::vector<std::string> movs = {"U", "U'", "R", "R'", "F", "F'"};
    std::uniform_int_distribution<int> dist(0, 5);
    cubo = Cubo();
    std::string ant;
    std::cout << "[UI] Embaralhando (seed " << e.seed << "): ";
    for (int i = 0; i < 6; ++i) {
        std::string m;
        do {
            m = movs[dist(rng)];
        } while (!ant.empty() && ehInverso(ant, m));
        ant = m;
        cubo = cubo.aplicarMovimento(m);
        std::cout << m << " ";
    }
    std::cout << std::endl;
    e.caminhoResolucao.clear();
    e.passoAtual = 0;
    e.estadosVisitados = 0;
    e.ultimoMovimento = "---";
    e.cuboResolvidoFlag = false;
    e.mensagemStatus = "Seed " + std::to_string(e.seed) + ". Clique RESOLVER.";
}

void fazerResolver(InterfaceEstado& e, Cubo& cubo) {
    e.passoAtual = 0;
    e.caminhoResolucao.clear();
    e.estadosVisitados = 0;
    e.ultimoMovimento = "---";
    if (cubo.estaResolvido()) {
        e.mensagemStatus = "Cubo ja resolvido. Embaralhe primeiro.";
        std::cout << "[UI] Cubo ja resolvido." << std::endl;
        return;
    }
    e.mensagemStatus = std::string("Resolvendo com ") + e.algoritmoSelecionado + "...";
    std::cout << "[UI] Resolvendo com " << e.algoritmoSelecionado << "..." << std::endl;

    bool ok = false;
    std::vector<std::string> passos;
    int vis = 0;
    if (e.algoritmoSelecionado == "Largura") {
        BuscaPorLargura s;
        auto res = s.resolver(cubo, 7);
        ok = res.sucesso;
        passos = res.passos;
        vis = res.estadosVisitados;
    } else if (e.algoritmoSelecionado == "A*") {
        BuscaAEstrela s;
        auto res = s.resolver(cubo, 12);
        ok = res.sucesso;
        passos = res.passos;
        vis = res.estadosVisitados;
    } else {
        BuscaProfundidade s;
        auto res = s.resolver(cubo, 7);
        ok = res.sucesso;
        passos = res.passos;
        vis = res.estadosVisitados;
    }
    interface_definirResultado(e, ok, passos, vis);
    if (ok) {
        std::cout << "[UI] Solucao (" << passos.size() << " mov, " << vis << " estados): ";
        for (auto& m : passos) std::cout << m << " ";
        std::cout << "\n[UI] Clique PASSO (ou ESPACO) para executar." << std::endl;
    } else {
        std::cout << "[UI] Sem solucao no limite (" << vis << " estados)." << std::endl;
    }
}

void fazerPasso(InterfaceEstado& e, Cubo& cubo) {
    if (e.caminhoResolucao.empty() || e.passoAtual >= (int)e.caminhoResolucao.size()) {
        e.mensagemStatus = "Nada a aplicar: clique RESOLVER primeiro.";
        return;
    }
    std::string m = e.caminhoResolucao[e.passoAtual];
    cubo = cubo.aplicarMovimento(m);
    e.passoAtual++;
    e.ultimoMovimento = m;
    e.ultimoPassoEm = SDL_GetTicks();
    std::cout << "[UI] Passo " << e.passoAtual << "/" << e.caminhoResolucao.size() << ": " << m
              << std::endl;
    if (cubo.estaResolvido()) {
        e.cuboResolvidoFlag = true;
        e.mensagemStatus = "CUBO RESOLVIDO! Reset para recomecar.";
        std::cout << "[UI] CUBO RESOLVIDO!" << std::endl;
    } else if (e.passoAtual >= (int)e.caminhoResolucao.size()) {
        e.mensagemStatus = "Fim da sequencia, mas cubo nao confere.";
    } else {
        e.mensagemStatus = "Passo " + std::to_string(e.passoAtual) + "/" +
                           std::to_string(e.caminhoResolucao.size()) + ": " + m;
    }
}

const char* MOVIMENTOS_MANUAIS[6] = {"U", "U'", "R", "R'", "F", "F'"};

// Jogo manual: aplica o movimento e invalida qualquer solução calculada antes.
void fazerMovimentoManual(InterfaceEstado& e, Cubo& cubo, const std::string& m) {
    bool tinhaSolucao = !e.caminhoResolucao.empty();
    cubo = cubo.aplicarMovimento(m);
    e.ultimoMovimento = m;
    e.ultimoPassoEm = SDL_GetTicks();
    e.caminhoResolucao.clear();
    e.passoAtual = 0;
    e.estadosVisitados = 0;
    e.cuboResolvidoFlag = cubo.estaResolvido();
    if (cubo.estaResolvido()) {
        e.mensagemStatus = "CUBO RESOLVIDO MANUALMENTE! Embaralhe p/ jogar.";
        std::cout << "[UI] Manual " << m << " -> CUBO RESOLVIDO!" << std::endl;
    } else {
        e.mensagemStatus =
            "Manual: " + m + (tinhaSolucao ? " (solucao descartada)." : ". RESOLVER p/ dica.");
        std::cout << "[UI] Manual: " << m << std::endl;
    }
}

void fazerReset(InterfaceEstado& e, Cubo& cubo) {
    std::string alg = e.algoritmoSelecionado;
    unsigned int s = e.seed;  // seed preservada p/ repetir o teste
    SDL_Window* w = e.janela;
    SDL_Renderer* r = e.renderer;
    int lw = e.larguraJanela, lh = e.alturaJanela;
    int mx = e.mouseX, my = e.mouseY;
    e = InterfaceEstado{};
    e.janela = w;
    e.renderer = r;
    e.larguraJanela = lw;
    e.alturaJanela = lh;
    e.mouseX = mx;
    e.mouseY = my;
    e.inicializada = true;
    e.algoritmoSelecionado = alg;
    e.seed = s;
    e.cuboResolvidoFlag = true;
    cubo = Cubo();
    layoutWidgets(e);
    e.mensagemStatus = "Cubo resetado. Embaralhe para comecar.";
    std::cout << "[UI] Reset." << std::endl;
}

void layoutWidgets(InterfaceEstado& e) {
    // Esquerda: algoritmos
    for (int i = 0; i < 3; ++i) e.btnAlgoritmo[i] = {24, 116 + i * 62, 206, 54, ""};
    e.btnAlgoritmo[0].texto = "PROFUNDIDADE";
    e.btnAlgoritmo[0].dica = "[1]";
    e.btnAlgoritmo[1].texto = "LARGURA";
    e.btnAlgoritmo[1].dica = "[2]";
    e.btnAlgoritmo[2].texto = "A*";
    e.btnAlgoritmo[2].dica = "[3]";
    // Direita: acoes
    e.btnEmbaralhar = {730, 116, 206, 50, "EMBARALHAR", "[E]"};
    e.btnResolver = {730, 174, 206, 50, "RESOLVER", "[S]"};
    e.btnPasso = {730, 232, 206, 50, "PASSO", "[SPC]"};
    e.btnReset = {730, 290, 206, 50, "RESET", "[BKSP]"};
    // Esquerda, card SEED (12,510,230,86): caixa + [-][+][DADO]
    e.btnSeedBox = {24, 536, 100, 32, "", ""};
    e.btnSeedMenos = {128, 536, 30, 32, "-", ""};
    e.btnSeedMais = {162, 536, 30, 32, "+", ""};
    e.btnSeedDado = {196, 536, 34, 32, "DADO", ""};
    // Faixa inferior: jogo manual (6 botoes centralizados, janela 960 de largura)
    const char* dicasMov[6] = {"[U]", "[SH+U]", "[R]", "[SH+R]", "[F]", "[SH+F]"};
    for (int i = 0; i < 6; ++i) {
        e.btnMov[i] = {95 + i * 130, 636, 120, 36, MOVIMENTOS_MANUAIS[i], dicasMov[i]};
    }
}

std::string pillTexto(const InterfaceEstado& e, const Cubo& cubo) {
    if (cubo.estaResolvido() && e.caminhoResolucao.empty()) return "PRONTO";
    if (cubo.estaResolvido() && e.passoAtual > 0) return "RESOLVIDO";
    if (!e.caminhoResolucao.empty() && e.passoAtual < (int)e.caminhoResolucao.size())
        return "PASSO " + std::to_string(e.passoAtual) + "/" + std::to_string(e.caminhoResolucao.size());
    if (!e.caminhoResolucao.empty()) return "SOLUCAO PRONTA";
    return "EMBARALHADO";
}

SDL_Color pillCor(const InterfaceEstado& e, const Cubo& cubo) {
    std::string p = pillTexto(e, cubo);
    if (p == "RESOLVIDO" || p == "PRONTO") return GREEN;
    if (p == "EMBARALHADO") return ORANGE;
    return ACCENT;
}

}  // namespace

// ---------------------------------------------------------------- API
bool interface_inicializar(InterfaceEstado& estado, int larguraJanela, int alturaJanela) {
    estado.larguraJanela = larguraJanela;
    estado.alturaJanela = alturaJanela;
    estado.janela = SDL_CreateWindow("Cubo Magico 2x2 - Solver", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, larguraJanela, alturaJanela,
                                     SDL_WINDOW_SHOWN);
    if (!estado.janela) {
        std::cerr << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
        return false;
    }
    estado.renderer =
        SDL_CreateRenderer(estado.janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!estado.renderer)
        estado.renderer = SDL_CreateRenderer(estado.janela, -1, SDL_RENDERER_SOFTWARE);
    if (!estado.renderer) {
        std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        return false;
    }
    layoutWidgets(estado);
    estado.inicializada = true;
    return true;
}

void interface_finalizar(InterfaceEstado& estado) {
    if (estado.renderer) SDL_DestroyRenderer(estado.renderer);
    if (estado.janela) SDL_DestroyWindow(estado.janela);
    estado.renderer = nullptr;
    estado.janela = nullptr;
    estado.inicializada = false;
}

void interface_processarEventos(InterfaceEstado& estado, Cubo& cubo) {
    SDL_Event ev;
    const char* nomes[3] = {"Profundidade", "Largura", "A*"};
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            estado.rodando = false;
        } else if (ev.type == SDL_MOUSEMOTION) {
            estado.mouseX = ev.motion.x;
            estado.mouseY = ev.motion.y;
        } else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
            estado.mouseX = ev.button.x;
            estado.mouseY = ev.button.y;
            estado.mousePressionado = true;
            int x = ev.button.x, y = ev.button.y;
            for (int i = 0; i < 3; ++i)
                if (dentro(x, y, estado.btnAlgoritmo[i])) {
                    estado.algoritmoSelecionado = nomes[i];
                    estado.mensagemStatus = std::string("Algoritmo: ") + nomes[i];
                    std::cout << "[UI] Algoritmo: " << nomes[i] << std::endl;
                }
            // Seed: caixa inicia edicao; qualquer outro clique confirma.
            if (dentro(x, y, estado.btnSeedBox)) {
                estado.seedEditando = true;
                estado.seedBuffer = std::to_string(estado.seed);
            } else {
                confirmarSeed(estado);
            }
            if (dentro(x, y, estado.btnSeedMenos)) {
                estado.seedEditando = false;
                estado.seed = estado.seed > 0 ? estado.seed - 1 : 0;
                std::cout << "[UI] Seed: " << estado.seed << std::endl;
            } else if (dentro(x, y, estado.btnSeedMais)) {
                estado.seedEditando = false;
                estado.seed++;
                std::cout << "[UI] Seed: " << estado.seed << std::endl;
            } else if (dentro(x, y, estado.btnSeedDado)) {
                estado.seedEditando = false;
                estado.seed = std::random_device{}();
                std::cout << "[UI] Seed aleatoria: " << estado.seed << std::endl;
            }
            if (dentro(x, y, estado.btnEmbaralhar))
                fazerEmbaralhar(estado, cubo);
            else if (dentro(x, y, estado.btnResolver))
                fazerResolver(estado, cubo);
            else if (dentro(x, y, estado.btnPasso))
                fazerPasso(estado, cubo);
            else if (dentro(x, y, estado.btnReset))
                fazerReset(estado, cubo);
            for (int i = 0; i < 6; ++i)
                if (dentro(x, y, estado.btnMov[i]))
                    fazerMovimentoManual(estado, cubo, MOVIMENTOS_MANUAIS[i]);
        } else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
            estado.mousePressionado = false;
        } else if (ev.type == SDL_KEYDOWN) {
            SDL_Keycode k = ev.key.keysym.sym;
            if (estado.seedEditando) {
                // Enquanto edita a seed, o teclado pertence a caixa (ignora atalhos).
                if (k >= SDLK_0 && k <= SDLK_9) {
                    if (estado.seedBuffer.size() < 10)
                        estado.seedBuffer += (char)('0' + (k - SDLK_0));
                } else if (k >= SDLK_KP_0 && k <= SDLK_KP_9) {
                    if (estado.seedBuffer.size() < 10)
                        estado.seedBuffer += (char)('0' + (k - SDLK_KP_0));
                } else if (k == SDLK_BACKSPACE) {
                    if (!estado.seedBuffer.empty()) estado.seedBuffer.pop_back();
                } else if (k == SDLK_RETURN || k == SDLK_KP_ENTER || k == SDLK_ESCAPE) {
                    confirmarSeed(estado);
                }
            } else if (k == SDLK_e)
                fazerEmbaralhar(estado, cubo);
            else if (k == SDLK_s)
                fazerResolver(estado, cubo);
            else if (k == SDLK_u || k == SDLK_r || k == SDLK_f) {
                // Jogo manual pelo teclado: SHIFT = movimento linha (U').
                bool linha = (ev.key.keysym.mod & KMOD_SHIFT) != 0;
                std::string m;
                m += (char)std::toupper((unsigned char)k);
                if (linha) m += "'";
                fazerMovimentoManual(estado, cubo, m);
            } else if (k == SDLK_SPACE)
                fazerPasso(estado, cubo);
            else if (k == SDLK_BACKSPACE || k == SDLK_DELETE)
                fazerReset(estado, cubo);
            else if (k == SDLK_1)
                estado.algoritmoSelecionado = "Profundidade";
            else if (k == SDLK_2)
                estado.algoritmoSelecionado = "Largura";
            else if (k == SDLK_3)
                estado.algoritmoSelecionado = "A*";
        }
    }
}

void interface_atualizar(InterfaceEstado& estado) {
    if (!estado.inicializada || !estado.janela) return;
    std::string t = "Cubo 2x2 [" + estado.algoritmoSelecionado + "] seed:" +
                    std::to_string(estado.seed) + " " + std::to_string(estado.passoAtual) + "/" +
                    std::to_string(estado.caminhoResolucao.size()) + " ult:" + estado.ultimoMovimento;
    SDL_SetWindowTitle(estado.janela, t.c_str());
}

bool interface_estaRodando(const InterfaceEstado& estado) { return estado.rodando; }

void interface_definirResultado(InterfaceEstado& estado, bool sucesso,
                                const std::vector<std::string>& passos, int estadosVisitados) {
    estado.caminhoResolucao = sucesso ? passos : std::vector<std::string>{};
    estado.estadosVisitados = estadosVisitados;
    estado.passoAtual = 0;
    estado.ultimoMovimento = sucesso ? "---" : "FALHA";
    if (sucesso)
        estado.mensagemStatus = "Solucao pronta: " + std::to_string(passos.size()) + " mov, " +
                                std::to_string(estadosVisitados) +
                                " estados. PASSO p/ executar.";
    else
        estado.mensagemStatus = "Sem solucao no limite (" + std::to_string(estadosVisitados) +
                                " estados). Embaralhe de novo.";
}

// ---------------------------------------------------------------- desenho
void interface_desenhar(InterfaceEstado& estado, const Cubo& cubo) {
    SDL_Renderer* r = estado.renderer;
    if (!r) return;
    const int W = estado.larguraJanela, H = estado.alturaJanela;

    setCor(r, BG);
    SDL_RenderClear(r);

    // ---------- header ----------
    fillRounded(r, 0, 0, W, 64, 0, {26, 29, 40, 255});
    setCor(r, BORDER);
    SDL_RenderDrawLine(r, 0, 64, W, 64);
    // mini logo: 4 stickers
    for (int i = 0; i < 4; ++i) {
        int c = (i == 0 ? 0 : i == 1 ? 5 : i == 2 ? 2 : 1);
        fillRounded(r, 20 + (i % 2) * 17, 15 + (i / 2) * 17, 15, 15, 4, STICKER[c]);
    }
    texto(r, 60, 14, "CUBO MAGICO 2X2", 2, TEXT);
    texto(r, 60, 36, "SOLVER VISUAL  -  DFS  -  BFS  -  A*", 1, MUTED);
    // pill de status
    std::string pill = pillTexto(estado, cubo);
    SDL_Color pc = pillCor(estado, cubo);
    int pillW = textoLargura(pill, 2) + 36;
    int pillX = W - pillW - 20, pillY = 17;
    fillRounded(r, pillX, pillY, pillW, 30, 15, tinge(pc, 5, 18));
    strokeRounded(r, pillX, pillY, pillW, 30, 15, pc);
    setCor(r, pc);
    SDL_Rect dot = {pillX + 10, pillY + 11, 8, 8};
    SDL_RenderFillRect(r, &dot);
    texto(r, pillX + 24, pillY + 8, pill, 2, TEXT);

    // ---------- coluna esquerda: algoritmo ----------
    card(r, 12, 76, 230, 252, "ALGORITMO  [1/2/3]");
    {
        const char* nomes[3] = {"Profundidade", "Largura", "A*"};
        const char* desc[3] = {"DFS ITERATIVO", "BFS EM LARGURA", "A* HEURISTICO"};
        SDL_Color ac[3] = {ACCENT, {150, 110, 255, 255}, GREEN};
        for (int i = 0; i < 3; ++i) {
            Botao b = estado.btnAlgoritmo[i];
            bool sel = estado.algoritmoSelecionado == nomes[i];
            bool hov = dentro(estado.mouseX, estado.mouseY, b);
            // redesenha o botão com nome + descrição em 2 linhas
            SDL_Color fundo = PANEL2, borda = BORDER;
            if (sel) {
                fundo = tinge(ac[i], 4, 22);
                borda = ac[i];
            } else if (hov) {
                fundo = {48, 54, 76, 255};
                borda = {90, 100, 135, 255};
            }
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
            fillRounded(r, b.x + 2, b.y + 3, b.largura, b.altura, 10, SHADOW);
            SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
            fillRounded(r, b.x, b.y, b.largura, b.altura, 10, fundo);
            if (sel) fillRounded(r, b.x, b.y, 5, b.altura, 2, ac[i]);
            strokeRounded(r, b.x, b.y, b.largura, b.altura, 10, borda);
            texto(r, b.x + 14, b.y + 8, b.texto, 2, TEXT);
            texto(r, b.x + 14, b.y + 32, desc[i], 1, sel ? ac[i] : MUTED);
            texto(r, b.x + b.largura - 34, b.y + 8, b.dica, 1, MUTED);
        }
    }

    // ---------- coluna esquerda: estatísticas ----------
    card(r, 12, 336, 230, 172, "ESTATISTICAS");
    {
        int x = 26, y = 372;
        texto(r, x, y, "VISITADOS", 1, MUTED);
        texto(r, x + 130, y - 3, std::to_string(estado.estadosVisitados), 2, ACCENT);
        y += 30;
        texto(r, x, y, "PROFUNDIDADE", 1, MUTED);
        texto(r, x + 130, y - 3, std::to_string(estado.caminhoResolucao.size()), 2, TEXT);
        y += 30;
        texto(r, x, y, "ULTIMO MOV", 1, MUTED);
        texto(r, x + 130, y - 3, estado.ultimoMovimento, 2, YELLOW);
        y += 34;
        // progresso
        int tot = (int)estado.caminhoResolucao.size();
        double frac = tot ? (double)estado.passoAtual / tot : 0.0;
        texto(r, x, y, "PROGRESSO  " + std::to_string(estado.passoAtual) + "/" + std::to_string(tot),
              1, MUTED);
        y += 18;
        fillRounded(r, x, y, 202, 10, 5, {22, 25, 36, 255});
        if (tot > 0) fillRounded(r, x, y, (int)(202 * frac), 10, 5, GREEN);
    }

    // ---------- coluna esquerda: seed ----------
    card(r, 12, 510, 230, 86, "SEED - MESMO INICIO P/ TESTAR");
    {
        // caixa editável (clique + digite, ENTER confirma)
        const Botao& box = estado.btnSeedBox;
        bool hovBox = dentro(estado.mouseX, estado.mouseY, box);
        fillRounded(r, box.x, box.y, box.largura, box.altura, 8, {22, 25, 36, 255});
        strokeRounded(r, box.x, box.y, box.largura, box.altura, 8,
                      estado.seedEditando ? ACCENT : (hovBox ? MUTED : BORDER));
        std::string val = estado.seedEditando ? estado.seedBuffer : std::to_string(estado.seed);
        if (estado.seedEditando && (SDL_GetTicks() / 500) % 2 == 0) val += "_";
        int esc = val.size() <= 6 ? 2 : 1;
        int tw = textoLargura(val.empty() ? " " : val, esc);
        int tx = box.x + std::max(8, (box.largura - tw) / 2);
        texto(r, tx, box.y + (box.altura - 7 * esc) / 2, val, esc, TEXT);
        // mini-botoes [-][+][DADO]
        struct Mini {
            const Botao* b;
            const char* t;
        };
        Mini minis[3] = {{&estado.btnSeedMenos, "-"}, {&estado.btnSeedMais, "+"}, {&estado.btnSeedDado, "DADO"}};
        for (auto& mn : minis) {
            bool hov = dentro(estado.mouseX, estado.mouseY, *mn.b);
            bool press = hov && estado.mousePressionado;
            fillRounded(r, mn.b->x, mn.b->y, mn.b->largura, mn.b->altura, 7,
                        press ? ACCENT : (hov ? PANEL2 : PANEL));
            strokeRounded(r, mn.b->x, mn.b->y, mn.b->largura, mn.b->altura, 7,
                          hov ? ACCENT : BORDER);
            std::string t = mn.t;
            int e2 = t.size() <= 1 ? 2 : 1;
            textoCentro(r, mn.b->x + mn.b->largura / 2, mn.b->y + (mn.b->altura - 7 * e2) / 2, t,
                        e2, press ? BG : TEXT);
        }
        texto(r, 26, 574, "MESMA SEED = MESMO CUBO", 1, MUTED);
    }

    // ---------- centro: cubo ----------
    card(r, 254, 76, 452, 372, "CUBO 2X2  -  U/D/F/B/L/R");
    {
        int bloco = 2 * QUAD + QGAP;       // 68
        int gapF = 14;                     // espaço entre faces
        int netW = 4 * bloco + 3 * gapF;   // 314
        int netH = 3 * bloco + 2 * gapF + 74;
        int ox = 254 + (452 - netW) / 2;
        int oy = 76 + 40 + (372 - 40 - netH) / 2;
        bool hl = (SDL_GetTicks() - estado.ultimoPassoEm) < 1500 && estado.ultimoPassoEm != 0;
        char faceMov = estado.ultimoMovimento.empty() ? '-' : estado.ultimoMovimento[0];
        faceCubo(r, cubo, 0, ox + bloco + gapF, oy, "U", hl && faceMov == 'U');
        faceCubo(r, cubo, 4, ox, oy + bloco + gapF + 20, "L", false);
        faceCubo(r, cubo, 2, ox + bloco + gapF, oy + bloco + gapF + 20, "F", hl && faceMov == 'F');
        faceCubo(r, cubo, 5, ox + 2 * (bloco + gapF), oy + bloco + gapF + 20, "R",
                 hl && faceMov == 'R');
        faceCubo(r, cubo, 3, ox + 3 * (bloco + gapF), oy + bloco + gapF + 20, "B", false);
        faceCubo(r, cubo, 1, ox + bloco + gapF, oy + 2 * (bloco + gapF) + 40, "D", false);
    }

    // ---------- centro: solução ----------
    card(r, 254, 456, 452, 104, "SEQUENCIA DA SOLUCAO");
    {
        int x = 268, y = 490;
        if (estado.caminhoResolucao.empty()) {
            texto(r, x, y + 8, "---  RESOLVA O CUBO PARA VER OS MOVIMENTOS  ---", 1, MUTED);
        } else {
            int cx = x;
            for (size_t i = 0; i < estado.caminhoResolucao.size() && i < 14; ++i) {
                bool feito = (int)i < estado.passoAtual;
                bool atual = (int)i == estado.passoAtual;
                SDL_Color fundo = feito ? SDL_Color{35, 90, 60, 255}
                                : atual ? SDL_Color{60, 90, 160, 255}
                                        : PANEL2;
                SDL_Color borda = feito ? GREEN : atual ? ACCENT : BORDER;
                fillRounded(r, cx, y, 40, 30, 7, fundo);
                strokeRounded(r, cx, y, 40, 30, 7, borda);
                textoCentro(r, cx + 20, y + 8, estado.caminhoResolucao[i], 2,
                            feito || atual ? TEXT : MUTED);
                cx += 46;
                if (cx + 40 > 254 + 452 - 14) break;
            }
            if (estado.caminhoResolucao.size() > 14)
                texto(r, x, y + 40, "(+" + std::to_string(estado.caminhoResolucao.size() - 14) +
                                        " MOV...)",
                      1, MUTED);
            else
                texto(r, x, y + 40, "ESPACO = PROXIMO PASSO  |  AMARELO = FACE DO ULTIMO MOV", 1,
                      MUTED);
        }
    }

    // ---------- direita: ações ----------
    card(r, 718, 76, 230, 278, "ACOES");
    {
        bool temSol = !estado.caminhoResolucao.empty() &&
                      estado.passoAtual < (int)estado.caminhoResolucao.size();
        botao(r, estado.btnEmbaralhar, ORANGE, false, dentro(estado.mouseX, estado.mouseY,
                                                             estado.btnEmbaralhar),
              estado.mousePressionado && dentro(estado.mouseX, estado.mouseY, estado.btnEmbaralhar),
              false);
        botao(r, estado.btnResolver, GREEN, false,
              dentro(estado.mouseX, estado.mouseY, estado.btnResolver),
              estado.mousePressionado && dentro(estado.mouseX, estado.mouseY, estado.btnResolver),
              false);
        botao(r, estado.btnPasso, ACCENT, false,
              dentro(estado.mouseX, estado.mouseY, estado.btnPasso) && temSol,
              estado.mousePressionado && dentro(estado.mouseX, estado.mouseY, estado.btnPasso),
              !temSol);
        botao(r, estado.btnReset, MUTED, false,
              dentro(estado.mouseX, estado.mouseY, estado.btnReset),
              estado.mousePressionado && dentro(estado.mouseX, estado.mouseY, estado.btnReset),
              false);
    }

    // ---------- direita: legenda + ajuda ----------
    card(r, 718, 362, 230, 100, "CORES DAS FACES");
    for (int i = 0; i < 6; ++i) {
        int qx = 732 + (i % 3) * 72, qy = 392 + (i / 3) * 34;
        fillRounded(r, qx, qy, 18, 18, 5, STICKER[i]);
        strokeRounded(r, qx, qy, 18, 18, 5, BORDER);
        texto(r, qx + 23, qy + 3, NOME_COR[i], 1, MUTED);
    }
    card(r, 718, 470, 230, 100, "COMO USAR");
    texto(r, 732, 500, "1 EMBARALHAR [E]", 1, TEXT);
    texto(r, 732, 516, "2 RESOLVER   [S]", 1, TEXT);
    texto(r, 732, 532, "3 PASSO      [ESPACO]", 1, TEXT);
    texto(r, 732, 548, "4 JOGAR: U/R/F+SHIFT", 1, TEXT);

    // ---------- faixa inferior: jogo manual ----------
    card(r, 12, 604, 936, 76, "JOGAR MANUAL - CLIQUE OU TECLAS U/R/F (+SHIFT = LINHA)");
    {
        SDL_Color coresMov[6] = {STICKER[0], STICKER[0], STICKER[5],
                                 STICKER[5], STICKER[2], STICKER[2]};
        for (int i = 0; i < 6; ++i) {
            const Botao& b = estado.btnMov[i];
            botao(r, b, coresMov[i], false, dentro(estado.mouseX, estado.mouseY, b),
                  estado.mousePressionado && dentro(estado.mouseX, estado.mouseY, b), false);
        }
    }

    // ---------- footer ----------
    fillRounded(r, 0, H - 36, W, 36, 0, {26, 29, 40, 255});
    setCor(r, BORDER);
    SDL_RenderDrawLine(r, 0, H - 36, W, H - 36);
    {
        std::string st = estado.mensagemStatus;
        if (st.size() > 72) st = st.substr(0, 72);
        texto(r, 16, H - 26, st, 1, TEXT);
        std::string dica = "[1/2/3] ALG  [E/S/SPC/BKSP] ACOES  [U/R/F]+SHIFT JOGAR";
        texto(r, W - textoLargura(dica, 1) - 16, H - 26, dica, 1, MUTED);
    }

    SDL_RenderPresent(r);
}
