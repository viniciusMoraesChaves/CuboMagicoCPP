#include "astar.hpp"
#include <queue>
#include <unordered_map>

struct NoAEstrela {
    Cubo estado;
    std::vector<std::string> passos;
    int g = 0;
    int h = 0;
};

struct CompararNoAEstrela {
    bool operator()(const NoAEstrela& a, const NoAEstrela& b) const {
        int fa = a.g + a.h;
        int fb = b.g + b.h;
        if (fa == fb) return a.h > b.h;
        return fa > fb;
    }
};

static std::string serializarCuboAEstrela(const Cubo& cubo) {
    std::string s;
    s.reserve(cubo.faces.size());
    for (int cor : cubo.faces) {
        s.push_back(static_cast<char>('0' + cor));
    }
    return s;
}

static bool ehMovimentoInverso(const std::string& anterior, const std::string& atual) {
    return (anterior == "U" && atual == "U'") || (anterior == "U'" && atual == "U") ||
           (anterior == "R" && atual == "R'") || (anterior == "R'" && atual == "R") ||
           (anterior == "F" && atual == "F'") || (anterior == "F'" && atual == "F");
}

int BuscaAEstrela::heuristica(const Cubo& cubo) const {
    int adesivosErrados = 0;
    for (int i = 0; i < static_cast<int>(cubo.faces.size()); ++i) {
        if (cubo.faces[i] != i / 4) {
            adesivosErrados++;
        }
    }
    return (adesivosErrados + 7) / 8;
}

ResultadoBuscaAEstrela BuscaAEstrela::resolver(Cubo cuboInicial, int limiteMaximo) {
    ResultadoBuscaAEstrela resultado;
    std::vector<std::string> movimentos = {"U", "U'", "R", "R'", "F", "F'"};
    std::priority_queue<NoAEstrela, std::vector<NoAEstrela>, CompararNoAEstrela> fila;
    std::unordered_map<std::string, int> melhorCusto;

    fila.push({cuboInicial, {}, 0, heuristica(cuboInicial)});
    melhorCusto[serializarCuboAEstrela(cuboInicial)] = 0;

    while (!fila.empty()) {
        NoAEstrela atual = fila.top();
        fila.pop();
        resultado.estadosVisitados++;

        if (atual.estado.estaResolvido()) {
            resultado.sucesso = true;
            resultado.passos = atual.passos;
            return resultado;
        }

        if (atual.g >= limiteMaximo) {
            continue;
        }

        std::string ultimoMovimento = atual.passos.empty() ? "" : atual.passos.back();
        for (const std::string& movimento : movimentos) {
            if (ehMovimentoInverso(ultimoMovimento, movimento)) {
                continue;
            }

            Cubo proximo = atual.estado.aplicarMovimento(movimento);
            int novoG = atual.g + 1;
            std::string chave = serializarCuboAEstrela(proximo);

            if (!melhorCusto.count(chave) || novoG < melhorCusto[chave]) {
                melhorCusto[chave] = novoG;
                std::vector<std::string> passos = atual.passos;
                passos.push_back(movimento);
                fila.push({proximo, passos, novoG, heuristica(proximo)});
            }
        }
    }

    return resultado;
}
