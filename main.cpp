#include <iostream>
#include <SDL2/SDL.h>
#include "cubo.hpp"
#include "interface.hpp"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    InterfaceEstado estado;
    if (!interface_inicializar(estado, 960, 640)) {
        SDL_Quit();
        return 1;
    }

    Cubo cuboAtual; // fonte única de verdade do cubo

    while (interface_estaRodando(estado)) {
        interface_processarEventos(estado, cuboAtual);
        interface_atualizar(estado);
        interface_desenhar(estado, cuboAtual);
        SDL_Delay(16);
    }

    interface_finalizar(estado);
    SDL_Quit();
    return 0;
}
