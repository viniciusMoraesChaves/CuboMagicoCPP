#include "busca_comum.hpp"

std::string serializarCubo(const Cubo& cubo) {
    std::string chave;
    chave.reserve(cubo.faces.size());
    for (int cor : cubo.faces) {
        chave.push_back(static_cast<char>('0' + cor));
    }
    return chave;
}

std::vector<NoBusca> gerarSucessores(const NoBusca& atual) {
    static const std::vector<std::string> movimentos = {"U", "U'", "R", "R'", "F", "F'"};
    std::vector<NoBusca> sucessores;
    sucessores.reserve(movimentos.size());

    for (const std::string& movimento : movimentos) {
        NoBusca proximo;
        proximo.estado = atual.estado.aplicarMovimento(movimento);
        proximo.passos = atual.passos;
        proximo.passos.push_back(movimento);
        proximo.profundidade = atual.profundidade + 1;
        proximo.g = atual.g + 1;
        sucessores.push_back(proximo);
    }
    return sucessores;
}

ResultadoBusca executarBusca(const Cubo& cuboInicial, FronteiraBusca& fronteira) {
    ResultadoBusca resultado;
    fronteira.limpar();

    NoBusca inicial;
    inicial.estado = cuboInicial;
    fronteira.aceitar(inicial);
    fronteira.adicionar(inicial);

    while (!fronteira.vazia()) {
        NoBusca atual = fronteira.removerProximo();
        resultado.estadosVisitados++;

        if (atual.estado.estaResolvido()) {
            resultado.sucesso = true;
            resultado.passos = atual.passos;
            return resultado;
        }

        if (!fronteira.podeExpandir(atual)) {
            continue;
        }

        for (NoBusca sucessor : gerarSucessores(atual)) {
            if (fronteira.aceitar(sucessor)) {
                fronteira.adicionar(sucessor);
            }
        }
    }

    return resultado;
}
