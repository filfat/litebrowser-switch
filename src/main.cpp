#include <stdio.h>
#include <switch.h>

#include "../litehtml/include/litehtml.h"
#include "sdl_container.h"

int main (int argc, char* argv[]) {
    consoleInit(NULL);

    printf("Hello World!\n");

    while (appletMainLoop()) {
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}