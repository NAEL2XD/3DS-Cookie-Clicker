#include <math.h>
#include <stdlib.h>
#include <dirent.h>
#include "utils.h"
#include "main.h"
#include "game.h"
#include "menu.h"

int state = 1;
int oldSt = 1;
u32 kDown = 0;

void switchState(int state2Switch) {
    state = state2Switch;
}

int main() {
    gfxInitDefault();
    romfsInit();
    cfguInit();

    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Prepare();

    UTILS_Init();
    game_init();

    C3D_RenderTarget* pos[2] = {
        C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT),
        C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT)
    };

    mkdir("sdmc:/Nael2xd", 0777);
    mkdir("sdmc:/Nael2xd/CookieClicker", 0777);
    while (aptMainLoop()) {
        hidScanInput();
        kDown = hidKeysDown();

        bool exit = false;
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        for (int i = 0; i < 2; i++) {
            C2D_TargetClear(pos[i], C2D_Color32(60, 60, 60, 0xFF));
            C2D_SceneBegin(pos[i]);
            
            switch (state) {
                case 1: // Game
                    exit = i == 0 ? game_updateTOP() : game_updateBOTTOM();
                    break;
                case 2: // Menu
                if (oldSt != state) menu_init();
                    exit = i == 0 ? menu_updateTOP() : menu_updateBOTTOM();
                    break;
            }

            if (exit) break;
        }
        if (exit) break;

        if (oldSt != state) {
            oldSt = state;
        }

        C3D_FrameEnd(0);
    }

    C3D_Fini();
    C2D_Fini();
    romfsExit();
    cfguExit();
    gfxExit();
    return 0;
}