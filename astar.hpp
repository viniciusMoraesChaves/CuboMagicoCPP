#ifndef A_ESTRELA_HPP
#define A_ESTRELA_HPP

#include "busca_comum.hpp"

using ResultadoBuscaAEstrela = ResultadoBusca;

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
