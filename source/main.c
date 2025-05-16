#include <math.h>
#include <stdlib.h>
#include <dirent.h>
#include "utils.h"
#include "main.h"
#include "game.h"

bool debugCPU = false;

int state = 1;
u32 kDown = 0;

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
                case 1:
                    exit = (i == 0) ? game_updateTOP() : game_updateBOTTOM();
                    break;
            }

            if (i == 1 && debugCPU) {
                char sex[69];
                snprintf(sex, sizeof(sex), "CPU: %5.2f%%  GPU:%5.2f%%  BUF:%5.2f%%\n", C3D_GetProcessingTime()*6, C3D_GetDrawingTime()*6, C3D_GetCmdBufUsage()*100);
                UTILS_quickRenderText(sex, -1, 0, C2D_Color32(255, 255, 255, 127), 0.5, NULL);
            }

            if (exit) break;
        }
        if (exit) break;

        C3D_FrameEnd(0);
    }

    C3D_Fini();
    C2D_Fini();
    romfsExit();
    cfguExit();
    gfxExit();
    return 0;
}