#include <iostream>
#include "Cubo.hpp"
#include "profundidade.hpp"
#include <random>

int main() {
    std::vector<std::string> possibleMoves = {"R", "R'", "U", "U'", "F", "F'"};
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, possibleMoves.size() - 1);

    std::cout << "--- TESTANDO BUSCA EM PROFUNDIDADE (IDDFS) ---\n\n";

    // 1. Cria o cubo resolvido
    Cubo c;

    // 2. Embaralha com 3 movimentos conhecidos: R, U, F'
    int quantidadeMovimentos = 10;
    std::cout << "Embaralhando o cubo com " << quantidadeMovimentos << " movimentos aleatorios: ";
    
    for (int i = 0; i < quantidadeMovimentos; ++i) {
        int random_index = dis(gen);
        std::string movimentoSorteado = possibleMoves[random_index];
        
        std::cout << movimentoSorteado << (i == quantidadeMovimentos - 1 ? "" : " -> ");
        c = c.aplicarMovimento(movimentoSorteado);
    }
    std::cout << "Esta resolvido agora? " << (c.estaResolvido() ? "Sim" : "Nao") << "\n\n";

    // 3. Executa a sua IA
    std::cout << "Iniciando a busca...\n";
    BuscaProfundidade solver;
    ResultadoBusca res = solver.resolver(c, 6);

    // 4. Exibe o resultado
    if (res.sucesso) {
        std::cout << "\n[SUCESSO] Cubo resolvido!\n";
        std::cout << "Passos encontrados: ";
        for (const auto& passo : res.passos) {
            std::cout << passo << " ";
        }
        std::cout << "\nTotal de movimentos: " << res.passos.size() << "\n";
        std::cout << "Estados visitados pela IA: " << res.estadosVisitados << "\n";
    } else {
        std::cout << "\n[FALHA] Nao encontrou solucao dentro do limite.\n";
    }

    return 0;
}