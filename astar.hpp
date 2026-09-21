#ifndef A_ESTRELA_HPP
#define A_ESTRELA_HPP

#include "cubo.hpp"

#include <vector>
#include <string>

struct ResultadoBuscaAEstrela {

    bool sucesso = false;

    std::vector<std::string> passos;

    int estadosVisitados = 0;

};

class BuscaAEstrela {

public:

    ResultadoBuscaAEstrela resolver(
        Cubo cuboInicial,
        int limiteMaximo
    );

private:

    int heuristica(const Cubo& cubo) const;

};

#endif