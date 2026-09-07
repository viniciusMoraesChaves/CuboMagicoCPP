#include <iostream>
#include "cubo.hpp"
#include "profundidade.hpp"
#include "largura.hpp"
#include "astar.hpp"
#include <random>

int main() {
    std::vector<std::string> possibleMoves = {"R", "R'", "U", "U'", "F", "F'"};
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, possibleMoves.size() - 1);

    std::cout << "--- TESTANDO BUSCAS DO CUBO 2x2 ---\n\n";

    // 1. Cria o cubo resolvido
    Cubo c;

    // 2. Embaralha com movimentos aleatorios dentro do limite usado pelas buscas
    int quantidadeMovimentos = 6;
    std::cout << "Embaralhando o cubo com " << quantidadeMovimentos << " movimentos aleatorios: ";

    for (int i = 0; i < quantidadeMovimentos; ++i) {
        int random_index = dis(gen);
        std::string movimentoSorteado = possibleMoves[random_index];

        std::cout << movimentoSorteado << (i == quantidadeMovimentos - 1 ? "" : " -> ");
        c = c.aplicarMovimento(movimentoSorteado);
    }
    std::cout << "\nEsta resolvido agora? " << (c.estaResolvido() ? "Sim" : "Nao") << "\n\n";

    // 3. Executa a sua IA
    std::cout << "Iniciando a busca...\n";
    BuscaProfundidade solver;
    ResultadoBusca res = solver.resolver(c, 6);

    BuscaPorLargura solverLargura;
    ResultadoBuscaLargura resLargura = solverLargura.resolver(c, 6);

    BuscaAEstrela solverAEstrela;
    ResultadoBuscaAEstrela resAEstrela = solverAEstrela.resolver(c, 6);

    // 4. Exibe o resultado da profundidade
    if (res.sucesso) {
        std::cout << "\n[SUCESSO] | [Profundidade] !\n";
        std::cout << "Passos encontrados: ";
        for (const auto& passo : res.passos) {
            std::cout << passo << " ";
        }
        std::cout << "\nTotal de movimentos: " << res.passos.size() << "\n";
        std::cout << "Estados visitados pela IA: " << res.estadosVisitados << "\n";
    } else {
        std::cout << "\n[FALHA] Nao encontrou solucao dentro do limite.\n";
    }
    // 5. Exibe o resultado da largura
    if(resLargura.sucesso) {
        std::cout << "\n[SUCESSO] | [Largura] !\n";
        std::cout << "Passos encontrados: ";
        for (const auto& passo : resLargura.passos) {
            std::cout << passo << " ";
        }
        std::cout << "\nTotal de movimentos: " << resLargura.passos.size() << "\n";
        std::cout << "Estados visitados pela IA: " << resLargura.estadosVisitados << "\n";
    } else {
        std::cout << "\n[FALHA] Nao encontrou solucao [Largura].\n";
    }

    // 6. Exibe o resultado da A*
    if(resAEstrela.sucesso) {
        std::cout << "\n[SUCESSO] | [A*] !\n";
        std::cout << "Passos encontrados: ";
        for (const auto& passo : resAEstrela.passos) {
            std::cout << passo << " ";
        }
        std::cout << "\nTotal de movimentos: " << resAEstrela.passos.size() << "\n";
        std::cout << "Estados visitados pela IA: " << resAEstrela.estadosVisitados << "\n";
    } else {
        std::cout << "\n[FALHA] Nao encontrou solucao [A*].\n";
    }

    return 0;
}
