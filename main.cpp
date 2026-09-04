#include <iostream>
#include "Cubo.hpp"
#include "profundidade.hpp"

int main() {
    std::cout << "--- TESTANDO BUSCA EM PROFUNDIDADE (IDDFS) ---\n\n";

    // 1. Cria o cubo resolvido
    Cubo c;

    // 2. Embaralha com 3 movimentos conhecidos: R, U, F'
    std::cout << "Embaralhando o cubo com: R -> U -> F'\n";
    c = c.aplicarMovimento("R");
    c = c.aplicarMovimento("U");
    c = c.aplicarMovimento("F'");

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