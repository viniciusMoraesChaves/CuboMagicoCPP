#include "Cubo.hpp"

// Índices das faces
enum { U = 0, D = 1, F = 2, B = 3, L = 4, R = 5 };

Cubo::Cubo() {
    faces.resize(24);
    for (int f = 0; f < 6; ++f) {
        for (int i = 0; i < 4; ++i) {
            faces[f * 4 + i] = f;
        }
    }
}

// Função avaliadora: checa se cada face tem todos os 4 quadrados com a mesma cor
bool Cubo::estaResolvido() const {
    for (int f = 0; f < 6; ++f) {
        int cor = faces[f * 4];
        for (int i = 1; i < 4; ++i) {
            if (faces[f * 4 + i] != cor) return false;
        }
    }
    return true;
}

void Cubo::girarFaceHorario(int f) {
    int b = f * 4;
    int tmp = faces[b];
    faces[b] = faces[b + 2];
    faces[b + 2] = faces[b + 3];
    faces[b + 3] = faces[b + 1];
    faces[b + 1] = tmp;
}

// Função sucessora: aplica o movimento e devolve o novo estado
Cubo Cubo::aplicarMovimento(const std::string& m) const {
    Cubo c = *this;

    if (m == "U") {
        c.girarFaceHorario(U);
        int t0 = c.faces[F*4+0], t1 = c.faces[F*4+1];
        c.faces[F*4+0] = c.faces[R*4+0]; c.faces[F*4+1] = c.faces[R*4+1];
        c.faces[R*4+0] = c.faces[B*4+0]; c.faces[R*4+1] = c.faces[B*4+1];
        c.faces[B*4+0] = c.faces[L*4+0]; c.faces[B*4+1] = c.faces[L*4+1];
        c.faces[L*4+0] = t0;             c.faces[L*4+1] = t1;
    } else if (m == "U'") {
        c = c.aplicarMovimento("U").aplicarMovimento("U").aplicarMovimento("U");
    } else if (m == "R") {
        c.girarFaceHorario(R);
        int t1 = c.faces[U*4+1], t3 = c.faces[U*4+3];
        c.faces[U*4+1] = c.faces[F*4+1]; c.faces[U*4+3] = c.faces[F*4+3];
        c.faces[F*4+1] = c.faces[D*4+1]; c.faces[F*4+3] = c.faces[D*4+3];
        c.faces[D*4+1] = c.faces[B*4+2]; c.faces[D*4+3] = c.faces[B*4+0];
        c.faces[B*4+2] = t1;             c.faces[B*4+0] = t3;
    } else if (m == "R'") {
        c = c.aplicarMovimento("R").aplicarMovimento("R").aplicarMovimento("R");
    } else if (m == "F") {
        c.girarFaceHorario(F);
        int t2 = c.faces[U*4+2], t3 = c.faces[U*4+3];
        c.faces[U*4+2] = c.faces[L*4+3]; c.faces[U*4+3] = c.faces[L*4+1];
        c.faces[L*4+1] = c.faces[D*4+0]; c.faces[L*4+3] = c.faces[D*4+1];
        c.faces[D*4+0] = c.faces[R*4+2]; c.faces[D*4+1] = c.faces[R*4+0];
        c.faces[R*4+0] = t2;             c.faces[R*4+2] = t3;
    } else if (m == "F'") {
        c = c.aplicarMovimento("F").aplicarMovimento("F").aplicarMovimento("F");
    }

    return c;
}