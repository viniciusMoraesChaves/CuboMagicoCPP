#include "core/Cube.hpp"

#include <array>
#include <cstdlib>
#include <iostream>

namespace {

using Move = void (Cube::*)();

bool cubesMatch(const Cube& left, const Cube& right)
{
    const std::array<Face, 6> faces = {
        Face::Up,
        Face::Down,
        Face::Front,
        Face::Back,
        Face::Left,
        Face::Right,
    };

    for (Face face : faces) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (left.sticker(face, row, col) != right.sticker(face, row, col)) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool returnsToInitialAfterFourTurns(Move move)
{
    Cube cube;
    const Cube initial;

    for (int i = 0; i < 4; ++i) {
        (cube.*move)();
    }

    return cubesMatch(cube, initial);
}

}

int main()
{
    const std::array<Move, 6> moves = {
        &Cube::moveU,
        &Cube::moveD,
        &Cube::moveF,
        &Cube::moveB,
        &Cube::moveL,
        &Cube::moveR,
    };
    const std::array<const char*, 6> names = {"U", "D", "F", "B", "L", "R"};

    for (std::size_t i = 0; i < moves.size(); ++i) {
        if (!returnsToInitialAfterFourTurns(moves[i])) {
            std::cerr << "Move " << names[i] << " did not return to the initial state\n";
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
