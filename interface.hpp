#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "cubo.hpp"

// Botão retangular com rótulo (desenhado pela interface).
struct Botao {
    int x = 0, y = 0, largura = 0, altura = 0;
    std::string texto;
    std::string dica;  // atalho de teclado exibido no canto, ex. "[E]"
};

// Estado completo da UI. A API pública (funções abaixo) é a mesma de antes.
struct InterfaceEstado {
    // SDL
    bool rodando = true;
    bool inicializada = false;
    SDL_Window* janela = nullptr;
    SDL_Renderer* renderer = nullptr;
    int larguraJanela = 960;
    int alturaJanela = 640;

    // Lógica do solver
    std::string algoritmoSelecionado = "Profundidade";  // "Profundidade" | "Largura" | "A*"
    int passoAtual = 0;
    std::vector<std::string> caminhoResolucao;
    int estadosVisitados = 0;
    std::string ultimoMovimento = "---";
    std::string mensagemStatus = "Embaralhe o cubo para comecar.";
    bool cuboResolvidoFlag = true;

    // Seed do embaralhamento: mesma seed => mesmo cubo inicial.
    unsigned int seed = 42;
    bool seedEditando = false;
    std::string seedBuffer;
    Botao btnSeedBox;
    Botao btnSeedMenos;
    Botao btnSeedMais;
    Botao btnSeedDado;

    // Interação (preenchidos a cada frame)
    int mouseX = -1, mouseY = -1;
    bool mousePressionado = false;
    Uint32 ultimoPassoEm = 0;  // timestamp do último Passo (p/ highlight)

    // Widgets
    Botao btnEmbaralhar;
    Botao btnResolver;
    Botao btnPasso;
    Botao btnReset;
    Botao btnAlgoritmo[3];
};

bool interface_inicializar(InterfaceEstado& estado, int larguraJanela, int alturaJanela);
void interface_finalizar(InterfaceEstado& estado);
void interface_processarEventos(InterfaceEstado& estado, Cubo& cubo);
void interface_atualizar(InterfaceEstado& estado);
void interface_desenhar(InterfaceEstado& estado, const Cubo& cubo);
bool interface_estaRodando(const InterfaceEstado& estado);
void interface_definirResultado(InterfaceEstado& estado, bool sucesso,
                                const std::vector<std::string>& passos, int estadosVisitados);

#endif
