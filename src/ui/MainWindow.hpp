#pragma once

#include "CubeWidget.hpp"

#include <QMainWindow>

class QPushButton;
class QString;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    CubeWidget* cubeWidget_;

    QPushButton* makeMoveButton(const QString& label, void (Cube::*move)());
};
