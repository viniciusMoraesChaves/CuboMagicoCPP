#include "profundidade.hpp"
#include <algorithm>

ResultadoBusca BuscaProfundidade::resolver(Cubo cuboInicial, int limiteMaximo) {
    ResultadoBusca resultado;
    resultado.sucesso = false;
    resultado.estadosVisitados = 0;

    std::vector<std::string> movimentos;
    movimentos.push_back("U");
    movimentos.push_back("U'");
    movimentos.push_back("R");
    movimentos.push_back("R'");
    movimentos.push_back("F");
    movimentos.push_back("F'");

    for (int limite = 0; limite <= limiteMaximo; ++limite) {
        std::stack<No*> pilha;

        // 1. Adicionar estado na estrutura
        No* inicial = new No(cuboInicial, nullptr, "", 0);
        pilha.push(inicial);

        // 2. Enquanto a estrutura nao estiver vazia:
        while (!pilha.empty()) {
            
            // 3. Remover proximo estado da estrutura
            No* atual = pilha.top();
            pilha.pop();
            resultado.estadosVisitados++;

            // 4. Avaliar estado
            if (atual->estado.estaResolvido()) {
                resultado.sucesso = true;

                No* temp = atual;
                while (temp->pai != nullptr) {
                    resultado.passos.push_back(temp->movimento);
                    temp = temp->pai;
                }
                std::reverse(resultado.passos.begin(), resultado.passos.end());
                return resultado;
            }

            // 5. Adicionar estados seguintes na estrutura
            if (atual->profundidade < limite) {
                for (size_t i = 0; i < movimentos.size(); ++i) {
                    std::string m = movimentos[i];

                    // Evita cancelamentos imediatos simples (U depois U', etc.)
                    if (!atual->movimento.empty()) {
                        if ((atual->movimento == "U" && m == "U'") || (atual->movimento == "U'" && m == "U") ||
                            (atual->movimento == "R" && m == "R'") || (atual->movimento == "R'" && m == "R") ||
                            (atual->movimento == "F" && m == "F'") || (atual->movimento == "F'" && m == "F")) {
                            continue;
                        }
                    }

                    Cubo proximo = atual->estado.aplicarMovimento(m);
                    No* filho = new No(proximo, atual, m, atual->profundidade + 1);
                    pilha.push(filho);
                }
            }
        }
    }

    return resultado;
}