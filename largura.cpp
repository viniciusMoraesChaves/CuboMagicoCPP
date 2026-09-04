#include "largura.hpp"
#include <algorithm>
#include <queue>
#include <unordered_set>

// converte o estado do cubo pra string
static std::string serializarCubo(const Cubo& cubo) {
    std::string s;
    s.reserve(cubo.faces.size());
    for (int cor : cubo.faces) {
        s.push_back(static_cast<char>('0' + cor));
    }
    return s;
}

ResultadoBuscaLargura BuscaPorLargura::resolver(Cubo cuboInicial, int limiteMaximo) {
    ResultadoBuscaLargura resultado;
    resultado.sucesso = false;
    resultado.estadosVisitados = 0;

    std::vector<std::string> movimentos = {"U", "U'", "R", "R'", "F", "F'"};

    std::queue<NoLargura*> fila;
    std::unordered_set<std::string> visitados;

    // Guarda todos os ponteiros criados para desalocar no final
    std::vector<NoLargura*> todosOsNos;

    NoLargura* raiz = new NoLargura(cuboInicial, nullptr, "", 0);
    fila.push(raiz);
    todosOsNos.push_back(raiz);
    visitados.insert(serializarCubo(cuboInicial));

    while (!fila.empty()) {
        NoLargura* atual = fila.front();
        fila.pop();

        resultado.estadosVisitados++;

        // verifica se p cubo ta resolvido
        if (atual->estado.estaResolvido()) {
            resultado.sucesso = true;

            //folha até raiz
            NoLargura* temp = atual;
            while (temp->pai != nullptr) {
                resultado.passos.push_back(temp->movimento);
                temp = temp->pai;
            }
            std::reverse(resultado.passos.begin(), resultado.passos.end());
            break;
        }

        // Limite de profundidade
        if (atual->profundidade >= limiteMaximo) {
            continue;
        }

        // Expansão dos próximos nós
        for (const std::string& mov : movimentos) {
            // aplicarMovimento devolve uma nova instância do Cubo
            Cubo proximoCubo = atual->estado.aplicarMovimento(mov);
            std::string idEstado = serializarCubo(proximoCubo);

            if (visitados.find(idEstado) == visitados.end()) {
                visitados.insert(idEstado);

                NoLargura* novoNo = new NoLargura(proximoCubo, atual, mov, atual->profundidade + 1);
                fila.push(novoNo);
                todosOsNos.push_back(novoNo);
            }
        }
    }

    // Liberação de memória para evitar vazamentos (memory leak)
    for (NoLargura* no : todosOsNos) {
        delete no;
    }

    return resultado;
}