#ifndef BUSCA_COMUM_HPP
#define BUSCA_COMUM_HPP

#include "cubo.hpp"

#include <string>
#include <vector>

struct NoBusca {
    Cubo estado;
    std::vector<std::string> passos;
    int profundidade = 0;
    int g = 0;
    int h = 0;
};

struct ResultadoBusca {
    bool sucesso = false;
    std::vector<std::string> passos;
    int estadosVisitados = 0;
};

class FronteiraBusca {
public:
    virtual ~FronteiraBusca() = default;

    virtual void limpar() = 0;
    virtual void adicionar(NoBusca no) = 0;
    virtual NoBusca removerProximo() = 0;
    virtual bool vazia() const = 0;
    virtual bool aceitar(const NoBusca& no) = 0;
    virtual bool podeExpandir(const NoBusca& no) const = 0;
};

std::string serializarCubo(const Cubo& cubo);
std::vector<NoBusca> gerarSucessores(const NoBusca& atual);
ResultadoBusca executarBusca(const Cubo& cuboInicial, FronteiraBusca& fronteira);

#endif
