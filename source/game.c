#include <math.h>
#include <stdlib.h>
#include "utils.h"
#include "main.h"

typedef struct {
    char text[16]; // Text to display
    float x;       // X-coordinate
    float y;       // Y-coordinate
    int alpha;     // Transparency (alpha value)
} SpawnedText;
SpawnedText spawnedText[50];
int stCount = 0;

typedef struct {
	float cookies;
	float textSize;
	float cookiePerPress;
    float cookiePerSecond;

    struct {
        C2D_Image cookie;
        C2D_Image background;
    } sprites;
} gameState;
gameState game;

int game_init() {
    // Initialize Variables
    game.cookies  = 0;
    game.textSize = 0;
    game.cookiePerPress = 1.0;
    game.cookiePerSecond = 0.0;

    // Initialize Images
    game.sprites.cookie     = UTILS_loadImage("romfs:/assets/cookie.t3x");
    game.sprites.background = UTILS_loadImage("romfs:/assets/background.t3x");
    return 0;
}

u64 timerCPS = 0;
bool game_update() {
    u64 runningTime = UTILS_getRunningTime();

    // Controls
    if (kDown & KEY_A) {
        game.cookies += game.cookiePerPress;
        game.textSize = 0.5;

        if (stCount < 50) {
            char final[16];
            snprintf(final, sizeof(final), "+%d", (int)game.cookiePerPress);

            // Add new text to spawnedText array
            snprintf(spawnedText[stCount].text, sizeof(spawnedText[stCount].text), "%s", final);
            spawnedText[stCount].x = (rand() % 133) + 115;
            spawnedText[stCount].y = (rand() % 80) + 80;
            spawnedText[stCount].alpha = 255;

            stCount++;
        }
    }

    if (kDown & KEY_START) {
        return true;
    }

    C2D_DrawImageAt(game.sprites.background, 0, 0, 0, NULL, 1, 1);

    char cookieShit[64];
    snprintf(cookieShit, sizeof(cookieShit), "%d", (int)game.cookies);

    char cpsShit[64];
    snprintf(cpsShit, sizeof(cpsShit), "%d cookies per second.", (int)game.cookiePerSecond);

    float finalSize = -(game.textSize / 6);
    UTILS_renderBorderText(cookieShit, -1, -10 + (20 / (game.textSize + 1)), 1, game.textSize + 1);
    UTILS_quickRenderText(cpsShit, -1, 40 + (game.textSize * 8), 255, 0.5);
    C2D_DrawImageAtRotated(game.sprites.cookie, 200 + (finalSize * 6), 128 + (finalSize * 6), 0, sin((float)runningTime / 512) / 8, NULL, finalSize + 1.2, finalSize + 1.2);
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

    if (runningTime - timerCPS > 1000) {
        game.cookies += game.cookiePerSecond;
        timerCPS = runningTime;
    }

    return false;
}