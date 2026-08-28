#pragma once

#include "../core/Cube.hpp"

#include <QColor>
#include <QWidget>

class QPainter;

class CubeWidget : public QWidget {
public:
    explicit CubeWidget(QWidget* parent = nullptr);

    QSize sizeHint() const override;

    Cube& cube();
    void resetCube();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Cube cube_;

    static QColor toQColor(CubeColor color);
    void drawFace(QPainter& painter, Face face, int netCol, int netRow, int stickerSize) const;
};
