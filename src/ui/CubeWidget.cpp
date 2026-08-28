#include "CubeWidget.hpp"

#include <algorithm>
#include <QPainter>

CubeWidget::CubeWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(520, 390);
}

QSize CubeWidget::sizeHint() const
{
    return {640, 480};
}

Cube& CubeWidget::cube()
{
    return cube_;
}

void CubeWidget::resetCube()
{
    cube_.reset();
    update();
}

void CubeWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#f4f6f8"));

    const int stickerSize = std::max(28, std::min(width() / 14, height() / 11));
    const int netWidth = stickerSize * 12;
    const int netHeight = stickerSize * 9;
    const int originX = (width() - netWidth) / 2;
    const int originY = (height() - netHeight) / 2;

    painter.translate(originX, originY);

    drawFace(painter, Face::Up, 3, 0, stickerSize);
    drawFace(painter, Face::Left, 0, 3, stickerSize);
    drawFace(painter, Face::Front, 3, 3, stickerSize);
    drawFace(painter, Face::Right, 6, 3, stickerSize);
    drawFace(painter, Face::Back, 9, 3, stickerSize);
    drawFace(painter, Face::Down, 3, 6, stickerSize);
}

QColor CubeWidget::toQColor(CubeColor color)
{
    switch (color) {
    case CubeColor::White:
        return QColor("#f8fafc");
    case CubeColor::Yellow:
        return QColor("#facc15");
    case CubeColor::Red:
        return QColor("#ef4444");
    case CubeColor::Orange:
        return QColor("#f97316");
    case CubeColor::Blue:
        return QColor("#2563eb");
    case CubeColor::Green:
        return QColor("#22c55e");
    }

    return QColor("#111827");
}

void CubeWidget::drawFace(QPainter& painter, Face face, int netCol, int netRow, int stickerSize) const
{
    const int gap = 3;
    const int faceX = netCol * stickerSize;
    const int faceY = netRow * stickerSize;

    painter.setPen(QPen(QColor("#111827"), 2));

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            const QRect tile(
                faceX + col * stickerSize + gap,
                faceY + row * stickerSize + gap,
                stickerSize - gap * 2,
                stickerSize - gap * 2);

            painter.setBrush(toQColor(cube_.sticker(face, row, col)));
            painter.drawRoundedRect(tile, 6, 6);
        }
    }
}
