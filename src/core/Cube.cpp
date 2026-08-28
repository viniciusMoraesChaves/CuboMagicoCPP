#include "Cube.hpp"

#include <stdexcept>

Cube::Cube()
{
    reset();
}

void Cube::reset()
{
    const std::array<CubeColor, 6> colors = {
        CubeColor::White,
        CubeColor::Yellow,
        CubeColor::Green,
        CubeColor::Blue,
        CubeColor::Orange,
        CubeColor::Red,
    };

    for (int faceIndex = 0; faceIndex < 6; ++faceIndex) {
        for (auto& row : faces_[faceIndex]) {
            row.fill(colors[faceIndex]);
        }
    }
}

void Cube::moveU()
{
    rotateClockwise(face(Face::Up));

    auto front = face(Face::Front)[0];
    face(Face::Front)[0] = face(Face::Right)[0];
    face(Face::Right)[0] = face(Face::Back)[0];
    face(Face::Back)[0] = face(Face::Left)[0];
    face(Face::Left)[0] = front;
}

void Cube::moveD()
{
    rotateClockwise(face(Face::Down));

    auto front = face(Face::Front)[2];
    face(Face::Front)[2] = face(Face::Left)[2];
    face(Face::Left)[2] = face(Face::Back)[2];
    face(Face::Back)[2] = face(Face::Right)[2];
    face(Face::Right)[2] = front;
}

void Cube::moveF()
{
    rotateClockwise(face(Face::Front));

    std::array<CubeColor, 3> up = face(Face::Up)[2];
    std::array<CubeColor, 3> left = {
        face(Face::Left)[0][2],
        face(Face::Left)[1][2],
        face(Face::Left)[2][2],
    };
    std::array<CubeColor, 3> down = face(Face::Down)[0];
    std::array<CubeColor, 3> right = {
        face(Face::Right)[0][0],
        face(Face::Right)[1][0],
        face(Face::Right)[2][0],
    };

    for (int i = 0; i < 3; ++i) {
        face(Face::Up)[2][i] = left[2 - i];
        face(Face::Left)[i][2] = down[i];
        face(Face::Down)[0][i] = right[2 - i];
        face(Face::Right)[i][0] = up[i];
    }
}

void Cube::moveB()
{
    rotateClockwise(face(Face::Back));

    std::array<CubeColor, 3> up = face(Face::Up)[0];
    std::array<CubeColor, 3> right = {
        face(Face::Right)[0][2],
        face(Face::Right)[1][2],
        face(Face::Right)[2][2],
    };
    std::array<CubeColor, 3> down = face(Face::Down)[2];
    std::array<CubeColor, 3> left = {
        face(Face::Left)[0][0],
        face(Face::Left)[1][0],
        face(Face::Left)[2][0],
    };

    for (int i = 0; i < 3; ++i) {
        face(Face::Up)[0][i] = right[i];
        face(Face::Right)[i][2] = down[2 - i];
        face(Face::Down)[2][i] = left[i];
        face(Face::Left)[i][0] = up[2 - i];
    }
}

void Cube::moveL()
{
    rotateClockwise(face(Face::Left));

    std::array<CubeColor, 3> up = {
        face(Face::Up)[0][0],
        face(Face::Up)[1][0],
        face(Face::Up)[2][0],
    };
    std::array<CubeColor, 3> back = {
        face(Face::Back)[0][2],
        face(Face::Back)[1][2],
        face(Face::Back)[2][2],
    };
    std::array<CubeColor, 3> down = {
        face(Face::Down)[0][0],
        face(Face::Down)[1][0],
        face(Face::Down)[2][0],
    };
    std::array<CubeColor, 3> front = {
        face(Face::Front)[0][0],
        face(Face::Front)[1][0],
        face(Face::Front)[2][0],
    };

    for (int i = 0; i < 3; ++i) {
        face(Face::Up)[i][0] = back[2 - i];
        face(Face::Back)[i][2] = down[2 - i];
        face(Face::Down)[i][0] = front[i];
        face(Face::Front)[i][0] = up[i];
    }
}

void Cube::moveR()
{
    rotateClockwise(face(Face::Right));

    std::array<CubeColor, 3> up = {
        face(Face::Up)[0][2],
        face(Face::Up)[1][2],
        face(Face::Up)[2][2],
    };
    std::array<CubeColor, 3> front = {
        face(Face::Front)[0][2],
        face(Face::Front)[1][2],
        face(Face::Front)[2][2],
    };
    std::array<CubeColor, 3> down = {
        face(Face::Down)[0][2],
        face(Face::Down)[1][2],
        face(Face::Down)[2][2],
    };
    std::array<CubeColor, 3> back = {
        face(Face::Back)[0][0],
        face(Face::Back)[1][0],
        face(Face::Back)[2][0],
    };

    for (int i = 0; i < 3; ++i) {
        face(Face::Up)[i][2] = front[i];
        face(Face::Front)[i][2] = down[i];
        face(Face::Down)[i][2] = back[2 - i];
        face(Face::Back)[i][0] = up[2 - i];
    }
}

CubeColor Cube::sticker(Face cubeFace, int row, int col) const
{
    if (row < 0 || row >= 3 || col < 0 || col >= 3) {
        throw std::out_of_range("sticker position must be between 0 and 2");
    }

    return face(cubeFace)[row][col];
}

Cube::FaceGrid& Cube::face(Face cubeFace)
{
    return faces_[static_cast<int>(cubeFace)];
}

const Cube::FaceGrid& Cube::face(Face cubeFace) const
{
    return faces_[static_cast<int>(cubeFace)];
}

void Cube::rotateClockwise(FaceGrid& grid)
{
    FaceGrid rotated = grid;

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            rotated[col][2 - row] = grid[row][col];
        }
    }

    grid = rotated;
}
