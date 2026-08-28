#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , cubeWidget_(new CubeWidget(this))
{
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);
    auto* title = new QLabel("Cubo Magico", central);
    auto* subtitle = new QLabel("Interface 2D inicial para testar movimentos basicos.", central);
    auto* controls = new QHBoxLayout();
    auto* resetButton = new QPushButton("Reset", central);

    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 28px; font-weight: 700; color: #111827;");

    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size: 14px; color: #4b5563;");

    controls->setSpacing(8);
    controls->addStretch();
    controls->addWidget(makeMoveButton("U", &Cube::moveU));
    controls->addWidget(makeMoveButton("D", &Cube::moveD));
    controls->addWidget(makeMoveButton("F", &Cube::moveF));
    controls->addWidget(makeMoveButton("B", &Cube::moveB));
    controls->addWidget(makeMoveButton("L", &Cube::moveL));
    controls->addWidget(makeMoveButton("R", &Cube::moveR));
    controls->addWidget(resetButton);
    controls->addStretch();

    connect(resetButton, &QPushButton::clicked, cubeWidget_, &CubeWidget::resetCube);

    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(cubeWidget_, 1);
    layout->addLayout(controls);

    setCentralWidget(central);
    setWindowTitle("Cubo Magico CPP");
    resize(780, 620);
}

QPushButton* MainWindow::makeMoveButton(const QString& label, void (Cube::*move)())
{
    auto* button = new QPushButton(label, this);
    button->setMinimumSize(48, 40);

    connect(button, &QPushButton::clicked, this, [this, move]() {
        (cubeWidget_->cube().*move)();
        cubeWidget_->update();
    });

    return button;
}

