#ifndef LARGURA_HPP
#define LARGURA_HPP

#include <vector>
#include <string>
#include <queue> //fila da busca em largura
#include "cubo.hpp"

struct ResultadoBuscaLargura{
    bool sucesso = false;
    std::vector<std::string> passos;
    int estadosVisitados = 0;
};

struct NoLargura {
    Cubo estado;
    NoLargura* pai;
    std::string movimento;
    int profundidade;

    NoLargura(Cubo est, NoLargura* p, std::string mov, int prof) {
        estado = est;
        pai = p;
        movimento = mov;
        profundidade = prof;
    }
};

class BuscaPorLargura {
public:
    ResultadoBuscaLargura resolver(Cubo cuboInicial, int limiteMaximo = 10);
};

#endif
