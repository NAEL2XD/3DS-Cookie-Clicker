#include <3ds.h>
#include <citro2d.h>
#include <math.h>
#include <stdlib.h>
#include "utils.h"

typedef struct {
    char text[16]; // Text to display
    float x;       // X-coordinate
    float y;       // Y-coordinate
    int alpha;     // Transparency (alpha value)
} SpawnedText;

typedef struct {
	int cookies;
	float textSize;
	int cookiePerPress;
} gameState;

#define MAX_TEXT_COUNT 100

int main() {
    gfxInitDefault();
    romfsInit();
    cfguInit();
    consoleInit(GFX_BOTTOM, NULL);

    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Prepare();

    C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    C2D_SpriteSheet temp = C2D_SpriteSheetLoad("romfs:/assets/cookie.t3x");
    C2D_Image cookie = C2D_SpriteSheetGetImage(temp, 0);
    free(temp);

    SpawnedText spawnedText[MAX_TEXT_COUNT];
    int stCount = 0;

    gameState game;
    game.cookies  =       0;
    game.textSize =       0;
    game.cookiePerPress = 1;

    int state = 1;
    u64 curTime = osGetTime();
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
        C2D_SceneBegin(top);

        UTILS_quickRenderText("DEMO", 0, 0, 120, 0.4);

        bool exit = false;
        switch (state) {
            case 1:
                // Controls
                if (kDown & KEY_A) {
                    game.cookies += game.cookiePerPress;
                    game.textSize = 0.5;

                    if (stCount < MAX_TEXT_COUNT) {
                        char final[16];
                        snprintf(final, sizeof(final), "+%d", game.cookiePerPress);

                        // Add new text to spawnedText array
                        snprintf(spawnedText[stCount].text, sizeof(spawnedText[stCount].text), "%s", final);
                        spawnedText[stCount].x = (rand() % 120) + 120;
                        spawnedText[stCount].y = (rand() % 80) + 80;
                        spawnedText[stCount].alpha = 255;

                        stCount++;
                    }
                }
                if (kDown & KEY_START) {
                    exit = true;
                    break;
                }

                char cookieShit[64];
                snprintf(cookieShit, sizeof(cookieShit), "%d", game.cookies);
                UTILS_renderBorderText("Press [START] to exit.", -1, 210, 1, 0.8);

                float finalSize = -(game.textSize / 6);
                UTILS_renderBorderText(cookieShit, -1, -10 + (20 / (game.textSize + 1)), 1, game.textSize + 1);
                C2D_DrawImageAtRotated(cookie, 200 + (finalSize * 6), 128 + (finalSize * 6), 0, sin((osGetTime() - curTime) / 800) / 8, NULL, finalSize + 1.2, finalSize + 1.2);
                game.textSize /= 1.1;

                for (int i = 0; i < stCount; i++) {
                    UTILS_quickRenderText(spawnedText[i].text, spawnedText[i].x, spawnedText[i].y, spawnedText[i].alpha, 1);
                    spawnedText[i].alpha -= 5;
					spawnedText[i].y -= 0.8;

                    if (spawnedText[i].alpha < 0) {
                        // Remove the text by shifting remaining entries
                        for (int j = i; j < stCount - 1; j++) {
                            spawnedText[j] = spawnedText[j + 1];
                        }

                        stCount--;
                        i--; // Adjust index after removal
                    }
                }

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