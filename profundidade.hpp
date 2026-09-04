#ifndef PROFUNDIDADE_HPP
#define PROFUNDIDADE_HPP

#include <vector>
#include <string>
#include <stack>
#include "Cubo.hpp"

struct ResultadoBusca {
    bool sucesso = false;
    std::vector<std::string> passos;
    int estadosVisitados = 0;
};

struct No {
    Cubo estado;
    No* pai;
    std::string movimento;
    int profundidade;

    No(Cubo est, No* p, std::string mov, int prof) {
        estado = est;
        pai = p;
        movimento = mov;
        profundidade = prof;
    }
};

class BuscaProfundidade {
public:
    ResultadoBusca resolver(Cubo cuboInicial, int limiteMaximo = 10);
};

#endif