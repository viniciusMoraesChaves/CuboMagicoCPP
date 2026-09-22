#include "astar.hpp"

#include <queue>
#include <unordered_map>
#include <vector>
#include <string>



struct NoAEstrela {

    Cubo estado;

    std::vector<std::string> passos;

    int g;

    int h;
};


struct CompararNoAEstrela {

    bool operator()(
        const NoAEstrela& a,
        const NoAEstrela& b
    ) const {

        int fA = a.g + a.h;

        int fB = b.g + b.h;

        return fA > fB;
    }
};


std::string serializarCubo(const Cubo& cubo) {

    std::string chave = "";

    for (int cor : cubo.faces) {

        chave += std::to_string(cor);
    }

    return chave;
}


int BuscaAEstrela::heuristica(const Cubo& cubo) const {

    int cantosErrados = 0;


    int cantos[8][3] = {

        {3, 9, 20},
        {2, 8, 17},
        {1, 12, 21},
        {0, 13, 16},

        {5, 11, 22},
        {4, 10, 19},
        {7, 14, 23},
        {6, 15, 18}

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


ResultadoBuscaAEstrela BuscaAEstrela::resolver(
    Cubo cuboInicial,
    int limiteMaximo
) {

    ResultadoBuscaAEstrela resultado;


    std::vector<std::string> movimentos = {

        "U", "U'",
        "R", "R'",
        "F", "F'"

    };


    std::priority_queue<
        NoAEstrela,
        std::vector<NoAEstrela>,
        CompararNoAEstrela
    > fila;


    std::unordered_map<std::string, int> melhorCusto;


    NoAEstrela inicial;

    inicial.estado = cuboInicial;

    inicial.passos = {};

    inicial.g = 0;

    inicial.h = heuristica(cuboInicial);

    fila.push(inicial);


    std::string chaveInicial =
        serializarCubo(cuboInicial);

    melhorCusto[chaveInicial] = 0;


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


        for (const std::string& movimento : movimentos) {

            Cubo proximo =
                atual.estado.aplicarMovimento(movimento);

            int novoG =
                atual.g + 1;

            std::string chave =
                serializarCubo(proximo);


            bool estadoNovo =
                melhorCusto.find(chave) ==
                melhorCusto.end();


            bool caminhoMelhor = false;


            if (!estadoNovo) {

                if (novoG < melhorCusto[chave]) {

                    caminhoMelhor = true;
                }
            }

            if (estadoNovo || caminhoMelhor) {

                melhorCusto[chave] =
                    novoG;


               
                std::vector<std::string> novosPassos =
                    atual.passos;


                novosPassos.push_back(movimento);


              
                NoAEstrela novoNo;

                novoNo.estado =
                    proximo;

                novoNo.passos =
                    novosPassos;

                novoNo.g =
                    novoG;

                novoNo.h =
                    heuristica(proximo);


            
                fila.push(novoNo);
            }
        }
    }


    
    return resultado;
}