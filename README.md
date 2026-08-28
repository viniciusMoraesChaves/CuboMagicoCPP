# CuboMagicoCPP

Interface inicial em C++/Qt para visualizar e manipular um cubo magico em 2D.

## Requisitos

- CMake 3.16+
- Qt 6 com o modulo Widgets
- Compilador C++17

## Como compilar

```bash
cmake -S . -B build
cmake --build build
```

## Como executar

```bash
./build/CuboMagicoCPP
```

## Como testar

```bash
ctest --test-dir build --output-on-failure
```

## Estrutura

```text
src/
├── core/
│   ├── Cube.hpp
│   └── Cube.cpp
├── ui/
│   ├── CubeWidget.hpp
│   ├── CubeWidget.cpp
│   ├── MainWindow.hpp
│   └── MainWindow.cpp
└── main.cpp
```

## Estado atual

- `core/Cube` guarda o estado do cubo e aplica movimentos basicos.
- `ui/CubeWidget` desenha as seis faces em uma rede 2D.
- `ui/MainWindow` cria a janela principal com botoes `U`, `D`, `F`, `B`, `L`, `R` e `Reset`.
- `tests/CubeCoreTests.cpp` valida que cada movimento basico aplicado quatro vezes volta ao estado inicial.
