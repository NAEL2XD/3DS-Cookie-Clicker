#include <3ds.h>
#include <citro2d.h>
#include <math.h>
#include <stdlib.h>
#include "utils.h"
#include "main.h"
#include "game.h"

int state = 1;
u64 curTime = 0;
u32 kDown = 0;
int main() {
    gfxInitDefault();
    romfsInit();
    cfguInit();
    consoleInit(GFX_BOTTOM, NULL);

    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Prepare();
    C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    printf("Done Precaching.");

    curTime = osGetTime();
    while (aptMainLoop()) {
        hidScanInput();
        kDown = hidKeysDown();

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
        C2D_SceneBegin(top);

        UTILS_quickRenderText("DEMO", 0, 0, 120, 0.4);

        bool exit = false;
        switch (state) {
            case 1:
                exit = game_update();
                break;
        }

        if (exit) break;

        C3D_FrameEnd(0);
    }

    C3D_Fini();
    C2D_Fini();
    romfsInit();
    cfguExit();
    gfxExit();
    return 0;
}