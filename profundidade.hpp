#ifndef PROFUNDIDADE_HPP
#define PROFUNDIDADE_HPP

#include "busca_comum.hpp"

class BuscaProfundidade {
public:
    ResultadoBusca resolver(Cubo cuboInicial, int limiteMaximo = 10);
};

#endif
