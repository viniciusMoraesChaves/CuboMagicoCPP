#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "cubo.hpp"

// ponytail: sem SDL_ttf de propósito — texto vai no terminal, janela só desenha cores.
class Interface {
public:
    Interface();
    ~Interface();
    bool iniciar();   // cria janela, retorna false se falhar
    void loop();      // loop principal (eventos + desenho)

private:
    SDL_Window* janela = nullptr;
    SDL_Renderer* render = nullptr;
    bool rodando = false;

    Cubo cubo;
    int solver = 0; // 0=Profundidade, 1=Largura, 2=A*
    std::vector<std::string> solucao;
    size_t passoAtual = 0;

    void processarEvento(const SDL_Event& e);
    void embaralharAleatorio();
    void resolver();
    void aplicarProximoPasso();
    void desenhar();
    void desenharFace(int face, int x, int y, int tam);
    void desenharBotao(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, bool selecionado);
    bool cliqueDentro(int mx, int my, int x, int y, int w, int h) const;
    void mostrarMenuTerminal() const;
};

#endif
