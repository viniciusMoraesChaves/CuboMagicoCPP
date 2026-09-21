#ifndef FONTE_HPP
#define FONTE_HPP

#include <SDL2/SDL.h>
#include <string>

// ponytail: fonte 3x5 propria p/ nao depender de SDL_ttf.
static const char* ORDEM_FONTE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 +='*/-";

static const int GLIFOS[][5] = {
    {2,5,7,5,5}, // A
    {6,5,6,5,6}, // B
    {3,4,4,4,3}, // C
    {6,5,5,5,6}, // D
    {7,4,6,4,7}, // E
    {7,4,6,4,4}, // F
    {3,4,5,5,3}, // G
    {5,5,7,5,5}, // H
    {7,2,2,2,7}, // I
    {1,1,1,5,2}, // J
    {5,5,6,5,5}, // K
    {4,4,4,4,7}, // L
    {5,7,7,5,5}, // M
    {6,5,5,5,5}, // N
    {2,5,5,5,2}, // O
    {6,5,6,4,4}, // P
    {2,5,5,6,3}, // Q
    {6,5,6,5,5}, // R
    {3,4,2,1,6}, // S
    {7,2,2,2,2}, // T
    {5,5,5,5,7}, // U
    {5,5,5,5,2}, // V
    {5,5,7,7,5}, // W
    {5,5,2,5,5}, // X
    {5,5,2,2,2}, // Y
    {7,1,2,4,7}, // Z
    {7,5,5,5,7}, // 0
    {2,6,2,2,7}, // 1
    {7,1,7,4,7}, // 2
    {7,1,7,1,7}, // 3
    {5,5,7,1,1}, // 4
    {7,4,7,1,7}, // 5
    {7,4,7,5,7}, // 6
    {7,1,2,2,2}, // 7
    {7,5,7,5,7}, // 8
    {7,5,7,1,7}, // 9
    {0,0,0,0,0}, // espaco
    {0,2,7,2,0}, // +
    {0,7,0,7,0}, // =
    {2,2,0,0,0}, // '
    {0,5,2,5,0}, // *
    {1,1,2,4,4}, // /
    {0,0,7,0,0}, // -
};

inline void desenharTexto(SDL_Renderer* render, int x, int y, const std::string& texto,
                          int esc, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    int cx = x;
    for (char ch : texto) {
        if (ch >= 'a' && ch <= 'z') ch -= 32;
        std::string ordem(ORDEM_FONTE);
        size_t idx = ordem.find(ch);
        if (idx != std::string::npos) {
            for (int l = 0; l < 5; ++l)
                for (int c = 0; c < 3; ++c)
                    if (GLIFOS[idx][l] & (1 << (2 - c))) {
                        SDL_Rect p = {cx + c * esc, y + l * esc, esc, esc};
                        SDL_RenderFillRect(render, &p);
                    }
        }
        cx += 4 * esc;
    }
}

// largura em px do texto (p/ centralizar)
inline int larguraTexto(const std::string& texto, int esc) {
    return (int)texto.size() * 4 * esc - esc;
}

#endif
