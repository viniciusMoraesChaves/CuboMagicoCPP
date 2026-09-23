#ifndef LARGURA_HPP
#define LARGURA_HPP

#include "busca_comum.hpp"

using ResultadoBuscaLargura = ResultadoBusca;

class BuscaPorLargura {
public:
    ResultadoBuscaLargura resolver(Cubo cuboInicial, int limiteMaximo = 10);
};

#endif
