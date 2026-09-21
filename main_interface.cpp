#include "interface.hpp"

int main() {
    Interface app;
    if (!app.iniciar()) return 1;
    app.loop();
    return 0;
}
