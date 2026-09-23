#include "largura.hpp"

#include <queue>
#include <unordered_set>
#include <utility>

namespace {

class FronteiraLargura : public FronteiraBusca {
public:
    explicit FronteiraLargura(int limiteMaximo) : limiteMaximo(limiteMaximo) {}

    void limpar() override {
        fila = {};
        visitados.clear();
    }

    void adicionar(NoBusca no) override { fila.push(std::move(no)); }

    NoBusca removerProximo() override {
        NoBusca no = std::move(fila.front());
        fila.pop();
        return no;
    }

    bool vazia() const override { return fila.empty(); }

    bool aceitar(const NoBusca& no) override {
        return visitados.insert(serializarCubo(no.estado)).second;
    }

    bool podeExpandir(const NoBusca& no) const override {
        return no.profundidade < limiteMaximo;
    }

private:
    int limiteMaximo;
    std::queue<NoBusca> fila;
    std::unordered_set<std::string> visitados;
};

}  // namespace

ResultadoBuscaLargura BuscaPorLargura::resolver(Cubo cuboInicial, int limiteMaximo) {
    FronteiraLargura fronteira(limiteMaximo);
    return executarBusca(cuboInicial, fronteira);
}
