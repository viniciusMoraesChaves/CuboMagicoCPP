#include "profundidade.hpp"

#include <stack>
#include <unordered_map>
#include <utility>

namespace {

class FronteiraProfundidade : public FronteiraBusca {
public:
    explicit FronteiraProfundidade(int limite) : limite(limite) {}

    void limpar() override {
        pilha = {};
        menorProfundidade.clear();
    }

    void adicionar(NoBusca no) override { pilha.push(std::move(no)); }

    NoBusca removerProximo() override {
        NoBusca no = std::move(pilha.top());
        pilha.pop();
        return no;
    }

    bool vazia() const override { return pilha.empty(); }

    bool aceitar(const NoBusca& no) override {
        std::string chave = serializarCubo(no.estado);
        auto encontrado = menorProfundidade.find(chave);

        if (encontrado != menorProfundidade.end() && encontrado->second <= no.profundidade) {
            return false;
        }

        menorProfundidade[chave] = no.profundidade;
        return true;
    }

    bool podeExpandir(const NoBusca& no) const override { return no.profundidade < limite; }

private:
    int limite;
    std::stack<NoBusca> pilha;
    std::unordered_map<std::string, int> menorProfundidade;
};

}  // namespace

ResultadoBusca BuscaProfundidade::resolver(Cubo cuboInicial, int limiteMaximo) {
    ResultadoBusca resultadoFinal;

    for (int limite = 0; limite <= limiteMaximo; ++limite) {
        FronteiraProfundidade fronteira(limite);
        ResultadoBusca resultado = executarBusca(cuboInicial, fronteira);
        resultadoFinal.estadosVisitados += resultado.estadosVisitados;

        if (resultado.sucesso) {
            resultado.estadosVisitados = resultadoFinal.estadosVisitados;
            return resultado;
        }
    }

    return resultadoFinal;
}
