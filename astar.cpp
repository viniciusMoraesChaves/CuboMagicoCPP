#include "astar.hpp"

#include <functional>
#include <queue>
#include <unordered_map>
#include <utility>

namespace {

struct CompararNoAEstrela {
    bool operator()(const NoBusca& a, const NoBusca& b) const {
        return a.g + a.h > b.g + b.h;
    }
};

class FronteiraAEstrela : public FronteiraBusca {
public:
    FronteiraAEstrela(int limiteMaximo, std::function<int(const Cubo&)> heuristica)
        : limiteMaximo(limiteMaximo), heuristica(std::move(heuristica)) {}

    void limpar() override {
        fila = {};
        melhorCusto.clear();
    }

    void adicionar(NoBusca no) override {
        no.h = heuristica(no.estado);
        fila.push(std::move(no));
    }

    NoBusca removerProximo() override {
        NoBusca no = fila.top();
        fila.pop();
        return no;
    }

    bool vazia() const override { return fila.empty(); }

    bool aceitar(const NoBusca& no) override {
        std::string chave = serializarCubo(no.estado);
        auto encontrado = melhorCusto.find(chave);

        if (encontrado != melhorCusto.end() && encontrado->second <= no.g) {
            return false;
        }

        melhorCusto[chave] = no.g;
        return true;
    }

    bool podeExpandir(const NoBusca& no) const override { return no.g < limiteMaximo; }

private:
    int limiteMaximo;
    std::function<int(const Cubo&)> heuristica;
    std::priority_queue<NoBusca, std::vector<NoBusca>, CompararNoAEstrela> fila;
    std::unordered_map<std::string, int> melhorCusto;
};

}  // namespace

int BuscaAEstrela::heuristica(const Cubo& cubo) const {
    int cantosErrados = 0;
    int cantos[8][3] = {
        {3, 9, 20}, {2, 8, 17}, {1, 12, 21}, {0, 13, 16},
        {5, 11, 22}, {4, 10, 19}, {7, 14, 23}, {6, 15, 18}
    };

    for (int i = 0; i < 8; i++) {
        bool cantoErrado = false;
        for (int j = 0; j < 3; j++) {
            int posicao = cantos[i][j];
            int corEsperada = posicao / 4;
            if (cubo.faces[posicao] != corEsperada) {
                cantoErrado = true;
            }
        }
        if (cantoErrado) {
            cantosErrados++;
        }
    }

    return (cantosErrados + 3) / 4;
}

ResultadoBuscaAEstrela BuscaAEstrela::resolver(Cubo cuboInicial, int limiteMaximo) {
    FronteiraAEstrela fronteira(
        limiteMaximo,
        [this](const Cubo& cubo) { return heuristica(cubo); }
    );
    return executarBusca(cuboInicial, fronteira);
}
