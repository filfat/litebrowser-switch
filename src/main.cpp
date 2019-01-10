#include <stdio.h>
#include <switch.h>

#include "../litehtml/include/litehtml.h"
#include "sdl_container.h"

int main (int argc, char* argv[]) {
    litehtml::context html_context;
    //html_context.load_master_stylesheet(master_css);

    consoleInit(NULL);

    printf("Hello World!\n");

    while (appletMainLoop()) {
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}