#ifndef CUBO_HPP
#define CUBO_HPP

#include <vector>
#include <string>

class Cubo {
public:
    // 6 faces x 4 peças = 24 posições
    // Cores: 0:Branco, 1:Amarelo, 2:Verde, 3:Azul, 4:Laranja, 5:Vermelho
    std::vector<int> faces;

    Cubo();

    bool estaResolvido() const;
    Cubo aplicarMovimento(const std::string& movimento) const;

private:
    void girarFaceHorario(int f);
};

#endif