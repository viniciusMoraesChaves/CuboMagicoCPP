#pragma once

#include <array>

enum class CubeColor {
    White,
    Yellow,
    Red,
    Orange,
    Blue,
    Green
};

enum class Face {
    Up = 0,
    Down = 1,
    Front = 2,
    Back = 3,
    Left = 4,
    Right = 5
};

class Cube {
public:
    Cube();

    void reset();

    void moveU();
    void moveD();
    void moveF();
    void moveB();
    void moveL();
    void moveR();

    CubeColor sticker(Face face, int row, int col) const;

private:
    using FaceGrid = std::array<std::array<CubeColor, 3>, 3>;

    std::array<FaceGrid, 6> faces_;

    FaceGrid& face(Face face);
    const FaceGrid& face(Face face) const;

    static void rotateClockwise(FaceGrid& grid);
};

