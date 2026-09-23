# Cubo Mágico 2x2 com IA

Simulador visual de cubo mágico 2x2 desenvolvido em C++ e SDL2. O programa embaralha o cubo de forma reproduzível e encontra uma solução usando uma das três estratégias:

- Busca em Largura (BFS);
- Busca em Profundidade Limitada Iterativa (IDDFS);
- Busca A* com heurística de cantos fora do lugar.

A interface mostra o estado atual do cubo, a sequência da solução, a quantidade de estados visitados e a aplicação dos movimentos passo a passo.

## Requisitos

- compilador com suporte a C++17;
- SDL2;
- `pkg-config`.

### Arch Linux

```bash
sudo pacman -S base-devel sdl2 pkgconf
```

### Ubuntu e Debian

```bash
sudo apt install build-essential libsdl2-dev pkg-config
```

## Compilação

Na pasta do projeto, execute:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
  main.cpp cubo.cpp busca_comum.cpp \
  profundidade.cpp largura.cpp astar.cpp interface.cpp \
  $(pkg-config --cflags --libs sdl2) \
  -o cubo_magico
```

## Execução

```bash
./cubo_magico
```

## Como usar

1. Escolha uma seed ou mantenha o valor padrão `42`.
2. Clique em **EMBARALHAR**. O programa aplica seis movimentos gerados pela seed.
3. Escolha **PROFUNDIDADE**, **LARGURA** ou **A***.
4. Clique em **RESOLVER**.
5. Observe a sequência encontrada e a quantidade de estados visitados.
6. Clique em **PASSO** para aplicar cada movimento da solução até o cubo ficar resolvido.

Usar a mesma seed produz o mesmo embaralhamento. Isso permite comparar os algoritmos usando exatamente o mesmo estado inicial.

### Atalhos

| Tecla | Ação |
|---|---|
| `1` | Selecionar Profundidade |
| `2` | Selecionar Largura |
| `3` | Selecionar A* |
| `E` | Embaralhar |
| `R` | Resolver |
| `Espaço` | Aplicar o próximo passo |
| `Backspace` ou `Delete` | Resetar o cubo |

## Representação do problema

### Estado

A classe `Cubo`, em `cubo.hpp`, representa o estado usando um vetor com 24 posições:

```text
6 faces × 4 adesivos = 24 posições
```

As cores são representadas por números de `0` a `5`. Um estado também pode ser serializado como texto para identificar configurações já conhecidas durante as buscas.

### Função sucessora

A função `gerarSucessores`, em `busca_comum.cpp`, aplica os movimentos:

```text
U, U', R, R', F, F'
```

Cada movimento gera um novo estado sem modificar o estado anterior. O sucessor guarda:

- o novo estado do cubo;
- a sequência de movimentos usada para alcançá-lo;
- sua profundidade;
- o custo acumulado `g`.

### Função avaliadora

`Cubo::estaResolvido()` verifica se os quatro adesivos de cada face possuem a mesma cor. Todas as buscas usam essa função ao remover um estado da fronteira.

O A* também utiliza uma avaliação de prioridade:

```text
f(n) = g(n) + h(n)
```

## Laço compartilhado

As três buscas utilizam o mesmo laço, implementado em `executarBusca`, no arquivo `busca_comum.cpp`:

```text
Adicionar o estado inicial na fronteira

Enquanto a fronteira não estiver vazia:
    Remover o próximo estado
    Contabilizar o estado visitado
    Avaliar se o cubo está resolvido
    Gerar os estados sucessores
    Adicionar os sucessores aceitos na fronteira

Retornar sem solução
```

O laço não conhece `queue`, `stack` ou `priority_queue`. Ele utiliza a interface `FronteiraBusca`. Cada algoritmo muda somente a implementação da fronteira e sua regra de aceitação.

## Busca em Largura — BFS

A Busca em Largura utiliza `std::queue`:

```text
primeiro estado adicionado = primeiro estado removido
```

Consequências:

- visita os estados por nível de profundidade;
- encontra uma solução com o menor número de movimentos dentro do limite configurado;
- pode consumir bastante memória porque mantém vários estados na fila.

Um `std::unordered_set` armazena os estados já encontrados. Um estado repetido não é adicionado novamente.

Na interface, a BFS usa limite máximo de profundidade `7`.

## Busca em Profundidade Limitada Iterativa — IDDFS

A busca em profundidade utiliza `std::stack`:

```text
último estado adicionado = primeiro estado removido
```

O algoritmo executa repetidamente o mesmo motor com limites crescentes:

```text
limite 0
limite 1
limite 2
...
limite máximo
```

Isso combina a exploração em profundidade com a capacidade de encontrar soluções rasas. A quantidade exibida de estados visitados é a soma de todas as iterações executadas.

Um `std::unordered_map` guarda a menor profundidade conhecida para cada estado. O mesmo estado só volta à pilha quando é encontrado por um caminho mais curto.

Na interface, a profundidade usa limite máximo `7`.

## Busca A*

A busca A* utiliza `std::priority_queue`. O próximo estado removido é aquele com menor valor de:

```text
f(n) = g(n) + h(n)
```

Onde:

- `g(n)` é a quantidade de movimentos desde o estado inicial;
- `h(n)` é a estimativa de movimentos restantes;
- `f(n)` é a prioridade total.

### Heurística

O cubo possui oito cantos. A heurística verifica quantos deles possuem pelo menos um adesivo fora da posição esperada:

```text
h(n) = teto(cantos errados / 4)
```

A divisão por quatro produz uma estimativa conservadora porque um movimento pode afetar até quatro cantos. A heurística é simples, rápida e compatível com a representação atual por adesivos.

Um `std::unordered_map` guarda o menor custo `g` conhecido para cada estado. O A* aceita um estado repetido somente quando o novo caminho possui custo menor.

Na interface, o A* usa limite máximo `12`.

## Estados visitados e solução

Um estado é contado como visitado quando é removido da fronteira para avaliação. Ao encontrar o objetivo, o resultado contém:

- sucesso ou falha;
- sequência de movimentos;
- quantidade de estados visitados.

Se o limite for atingido sem encontrar uma solução, a interface informa que não houve solução dentro do limite utilizado.

## Organização dos arquivos

| Arquivo | Responsabilidade |
|---|---|
| `main.cpp` | Inicialização da SDL2 e laço da interface |
| `cubo.hpp/.cpp` | Estado, movimentos e teste de objetivo |
| `busca_comum.hpp/.cpp` | Nó, resultado, sucessores e laço compartilhado |
| `largura.hpp/.cpp` | Fronteira FIFO da BFS |
| `profundidade.hpp/.cpp` | Fronteira LIFO e aprofundamento iterativo |
| `astar.hpp/.cpp` | Fila de prioridade e heurística do A* |
| `interface.hpp/.cpp` | Interface gráfica, seed e apresentação dos resultados |
