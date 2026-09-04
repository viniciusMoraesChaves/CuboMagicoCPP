# CuboMagicoCPP

Interface em C++/Qt para visualizar, manipular e resolver um cubo magico 2x2 em 2D.

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
│   ├── Cube.cpp
│   ├── DepthFirstSearch.hpp
│   └── DepthFirstSearch.cpp
├── ui/
│   ├── CubeWidget.hpp
│   ├── CubeWidget.cpp
│   ├── MainWindow.hpp
│   └── MainWindow.cpp
└── main.cpp
```

## Estado atual

- `core/Cube` guarda o estado 2x2 do cubo, aplica movimentos basicos e inversos, serializa estados e avalia se o cubo esta resolvido.
- `core/DepthFirstSearch` implementa busca em profundidade limitada iterativa, retornando passos, sucesso e estados visitados.
- `ui/CubeWidget` desenha as seis faces em uma rede 2D.
- `ui/MainWindow` permite jogar com movimentos `U`, `D`, `F`, `B`, `L`, `R` e inversos, reiniciar, embaralhar e resolver por profundidade.
- `tests/CubeCoreTests.cpp` valida movimentos, inversos, estado final e solucao de um embaralhamento curto pela busca.
