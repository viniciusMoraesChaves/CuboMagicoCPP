#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <string>
#include <vector>
#include "cubo.hpp"

struct ResultadoBuscaAEstrela {
    bool sucesso = false;
    std::vector<std::string> passos;
    int estadosVisitados = 0;
};

class BuscaAEstrela {
public:
    ResultadoBuscaAEstrela resolver(Cubo cuboInicial, int limiteMaximo = 10);

private:
    int heuristica(const Cubo& cubo) const;
};

#endif
