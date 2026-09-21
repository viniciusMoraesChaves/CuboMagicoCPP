#include "interface.hpp"
#include "fonte.hpp"
#include "profundidade.hpp"
#include "largura.hpp"
#include "astar.hpp"
#include <iostream>
#include <random>

static std::mt19937& gerador() {
    static std::mt19937 g(std::random_device{}());
    return g;
}

Interface::Interface() = default;

Interface::~Interface() {
    if (render) SDL_DestroyRenderer(render);
    if (janela) SDL_DestroyWindow(janela);
    SDL_Quit();
}

bool Interface::iniciar() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Erro SDL: " << SDL_GetError() << "\n";
        return false;
    }
    janela = SDL_CreateWindow("Cubo 2x2 - interface",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!janela) { std::cout << "Erro janela: " << SDL_GetError() << "\n"; return false; }
    render = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!render) { std::cout << "Erro render: " << SDL_GetError() << "\n"; return false; }
    rodando = true;
    mostrarMenuTerminal();
    return true;
}

void Interface::mostrarMenuTerminal() const {
    std::cout << "\n=== CUBO 2x2 ===\n"
              << "[1] Solver: Profundidade | [2] Largura | [3] A*   (atual: "
              << (solver == 0 ? "Profundidade" : solver == 1 ? "Largura" : "A*") << ")\n"
              << "[E] Embaralhar sozinho  |  [U/R/F] girar manual (+Shift = inverso: U')\n"
              << "[S ou ENTER] Resolver tudo  |  [ESPACO] 1 passo  |  [Z] resetar  |  [Q] sair\n"
              << "Na janela: clique nos 3 quadrados de cima p/ trocar solver,\n"
              << "  laranja=embaralhar, verde=resolver, amarelo=prox. passo, vermelho=reset.\n\n";
}

bool Interface::cliqueDentro(int mx, int my, int x, int y, int w, int h) const {
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

void Interface::embaralharAleatorio() {
    static const std::vector<std::string> movs = {"R", "R'", "U", "U'", "F", "F'"};
    std::uniform_int_distribution<> d(0, 5);
    std::cout << "Embaralhando: ";
    for (int i = 0; i < 6; ++i) {
        std::string m = movs[d(gerador())];
        std::cout << m << " ";
        cubo = cubo.aplicarMovimento(m);
    }
    std::cout << "\n";
    solucao.clear();
    passoAtual = 0;
}

void Interface::resolver() {
    solucao.clear();
    passoAtual = 0;
    bool ok = false;
    int visitados = 0;
    if (solver == 0) {
        BuscaProfundidade s; auto r = s.resolver(cubo, 7);
        ok = r.sucesso; solucao = r.passos; visitados = r.estadosVisitados;
    } else if (solver == 1) {
        BuscaPorLargura s; auto r = s.resolver(cubo, 7);
        ok = r.sucesso; solucao = r.passos; visitados = r.estadosVisitados;
    } else {
        BuscaAEstrela s; auto r = s.resolver(cubo, 7);
        ok = r.sucesso; solucao = r.passos; visitados = r.estadosVisitados;
    }
    if (!ok) { std::cout << "[FALHA] sem solucao no limite 7.\n"; return; }
    std::cout << "[OK] solucao (" << solucao.size() << " mov, " << visitados << " estados): ";
    for (auto& p : solucao) std::cout << p << " ";
    std::cout << "\nUse ESPACO p/ animar passo a passo, ou R p/ aplicar tudo.\n";
}

void Interface::aplicarProximoPasso() {
    if (passoAtual >= solucao.size()) { std::cout << "(nada p/ aplicar)\n"; return; }
    cubo = cubo.aplicarMovimento(solucao[passoAtual]);
    std::cout << "Aplicado: " << solucao[passoAtual]
              << " (" << passoAtual + 1 << "/" << solucao.size() << ")"
              << (cubo.estaResolvido() ? "  RESOLVIDO!" : "") << "\n";
    passoAtual++;
}

void Interface::processarEvento(const SDL_Event& e) {
    if (e.type == SDL_QUIT) { rodando = false; return; }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx = e.button.x, my = e.button.y;
        if (cliqueDentro(mx, my, 190, 400, 100, 40)) { solver = 0; mostrarMenuTerminal(); }
        else if (cliqueDentro(mx, my, 300, 400, 100, 40)) { solver = 1; mostrarMenuTerminal(); }
        else if (cliqueDentro(mx, my, 410, 400, 100, 40)) { solver = 2; mostrarMenuTerminal(); }
        else if (cliqueDentro(mx, my, 190, 450, 150, 40)) embaralharAleatorio();
        else if (cliqueDentro(mx, my, 350, 450, 150, 40)) {
            resolver();
            for (; passoAtual < solucao.size();) aplicarProximoPasso(); // R = tudo de uma vez
        }
        else if (cliqueDentro(mx, my, 510, 450, 150, 40)) aplicarProximoPasso();
        else if (cliqueDentro(mx, my, 30, 450, 150, 40)) {
            cubo = Cubo(); solucao.clear(); passoAtual = 0; std::cout << "Resetado.\n";
        }
        return;
    }
    if (e.type != SDL_KEYDOWN) return;
    bool inv = (SDL_GetModState() & KMOD_SHIFT) != 0;
    switch (e.key.keysym.sym) {
        case SDLK_1: solver = 0; mostrarMenuTerminal(); break;
        case SDLK_2: solver = 1; mostrarMenuTerminal(); break;
        case SDLK_3: solver = 2; mostrarMenuTerminal(); break;
        case SDLK_e: embaralharAleatorio(); break;
        case SDLK_s: case SDLK_RETURN: // aplica a solucao inteira
            resolver();
            for (; passoAtual < solucao.size();) aplicarProximoPasso();
            break;
        case SDLK_SPACE: aplicarProximoPasso(); break;
        case SDLK_u: cubo = cubo.aplicarMovimento(inv ? "U'" : "U"); solucao.clear(); passoAtual = 0; break;
        case SDLK_f: cubo = cubo.aplicarMovimento(inv ? "F'" : "F"); solucao.clear(); passoAtual = 0; break;
        case SDLK_r: cubo = cubo.aplicarMovimento(inv ? "R'" : "R"); solucao.clear(); passoAtual = 0; break;
        case SDLK_z: cubo = Cubo(); solucao.clear(); passoAtual = 0; std::cout << "Resetado.\n"; break;
        case SDLK_q: case SDLK_ESCAPE: rodando = false; break;
    }
}

void Interface::desenharFace(int face, int x, int y, int tam) {
    int s = tam / 2;
    for (int i = 0; i < 4; ++i) {
        int cor = cubo.faces[face * 4 + i];
        Uint8 r = 255, g = 255, b = 255;
        if (cor == 1) { r = 255; g = 255; b = 0; }        // amarelo
        else if (cor == 2) { r = 0; g = 200; b = 0; }     // verde
        else if (cor == 3) { r = 0; g = 120; b = 255; }   // azul
        else if (cor == 4) { r = 255; g = 140; b = 0; }   // laranja
        else if (cor == 5) { r = 220; g = 0; b = 0; }     // vermelho
        SDL_Rect q = {x + (i % 2) * s, y + (i / 2) * s, s, s};
        SDL_SetRenderDrawColor(render, r, g, b, 255);
        SDL_RenderFillRect(render, &q);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawRect(render, &q);
    }
}

void Interface::desenharBotao(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, bool selecionado) {
    SDL_Rect ret = {x, y, w, h};
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_RenderFillRect(render, &ret);
    SDL_SetRenderDrawColor(render, selecionado ? 255 : 0, selecionado ? 255 : 0, selecionado ? 255 : 0, 255);
    for (int i = 0; i < (selecionado ? 4 : 1); ++i) {
        SDL_Rect borda = {x + i, y + i, w - 2 * i, h - 2 * i};
        SDL_RenderDrawRect(render, &borda);
    }
}

void Interface::desenhar() {
    bool ok = cubo.estaResolvido();
    SDL_SetRenderDrawColor(render, ok ? 20 : 30, ok ? 80 : 30, ok ? 20 : 30, 255);
    SDL_RenderClear(render);
    // rede do cubo: U em cima, L F R B no meio, D embaixo
    desenharFace(0, 300, 20, 100);   // U
    desenharFace(4, 190, 130, 100);  // L
    desenharFace(2, 300, 130, 100);  // F
    desenharFace(5, 410, 130, 100);  // R
    desenharFace(3, 520, 130, 100);  // B
    desenharFace(1, 300, 240, 100);  // D
    // botoes: 3 solvers em cima, 3 acoes embaixo
    desenharBotao(190, 400, 100, 40, 40, 80, 200, solver == 0);   // DFS
    desenharBotao(300, 400, 100, 40, 40, 160, 80, solver == 1);   // BFS
    desenharBotao(410, 400, 100, 40, 140, 80, 200, solver == 2);  // A*
    desenharBotao(190, 450, 150, 40, 255, 140, 0, false);         // embaralhar
    desenharBotao(350, 450, 150, 40, 40, 180, 80, false);         // resolver
    desenharBotao(510, 450, 150, 40, 220, 200, 40, false);        // prox passo
    desenharBotao(30, 450, 150, 40, 200, 60, 60, false);          // reset
    // textos (fonte 3x5 propria)
    desenharTexto(render, 30, 25, "CUBO 2X2", 4, 255, 255, 255);
    if (ok) desenharTexto(render, 560, 30, "RESOLVIDO", 3, 80, 255, 80);
    else desenharTexto(render, 560, 30, "EMBARALHADO", 2, 255, 220, 80);
    desenharTexto(render, 190 + (100 - larguraTexto("1 DFS", 2)) / 2, 415, "1 DFS", 2, 255, 255, 255);
    desenharTexto(render, 300 + (100 - larguraTexto("2 BFS", 2)) / 2, 415, "2 BFS", 2, 255, 255, 255);
    desenharTexto(render, 410 + (100 - larguraTexto("3 ASTAR", 2)) / 2, 415, "3 ASTAR", 2, 255, 255, 255);
    desenharTexto(render, 190 + (150 - larguraTexto("EMBARALHAR", 2)) / 2, 465, "EMBARALHAR", 2, 0, 0, 0);
    desenharTexto(render, 350 + (150 - larguraTexto("RESOLVER", 2)) / 2, 465, "RESOLVER", 2, 255, 255, 255);
    desenharTexto(render, 510 + (150 - larguraTexto("PASSO", 2)) / 2, 465, "PASSO", 2, 0, 0, 0);
    desenharTexto(render, 30 + (150 - larguraTexto("RESET", 2)) / 2, 465, "RESET", 2, 255, 255, 255);
    desenharTexto(render, 30, 540, "U R F GIRA - SHIFT INVERSO", 2, 200, 200, 200);
    desenharTexto(render, 30, 562, "S RESOLVE - ESPACO PASSO", 2, 200, 200, 200);
    // lista de passos da solucao (verde=feito, amarelo=proximo, branco=pendente)
    if (!solucao.empty()) {
        desenharTexto(render, 645, 130, "PASSOS", 2, 255, 255, 255);
        for (size_t i = 0; i < solucao.size() && i < 14; ++i) {
            std::string linha = std::to_string(i + 1) + " " + solucao[i];
            Uint8 r = 255, g = 255, b = 255;
            if (i < passoAtual) { r = 80; g = 255; b = 80; }
            else if (i == passoAtual) { r = 255; g = 220; b = 80; }
            desenharTexto(render, 645, 154 + (int)i * 16, linha, 2, r, g, b);
        }
    }
    // barra de progresso da solucao: verde=aplicado, amarelo=pendente
    for (size_t i = 0; i < solucao.size() && i < 20; ++i) {
        SDL_Rect q = {190 + (int)i * 30, 510, 24, 24};
        bool feito = i < passoAtual;
        SDL_SetRenderDrawColor(render, feito ? 40 : 220, feito ? 180 : 200, feito ? 80 : 40, 255);
        SDL_RenderFillRect(render, &q);
    }
    SDL_RenderPresent(render);
}

void Interface::loop() {
    SDL_Event e;
    while (rodando) {
        while (SDL_PollEvent(&e)) processarEvento(e);
        desenhar();
        SDL_Delay(33);
    }
}
