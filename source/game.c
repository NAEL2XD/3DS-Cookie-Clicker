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
	int cookies;
	float textSize;
	int cookiePerPress;

    struct {
        C2D_Image cookie;
    } sprites;
} gameState;
gameState game;

int game_init() {
    game.cookies  = 0;
    game.textSize = 0;
    game.cookiePerPress = 1;

    printf("PLEASE WORK");
    game.sprites.cookie = UTILS_loadImage("romfs:/assets/cookie.t3x");
    return 0;
}

bool game_update() {
    // Controls
    if (kDown & KEY_A) {
        game.cookies += game.cookiePerPress;
        game.textSize = 0.5;

        if (stCount < 50) {
            char final[16];
            snprintf(final, sizeof(final), "+%d", game.cookiePerPress);

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

    char cookieShit[64];
    snprintf(cookieShit, sizeof(cookieShit), "%d", game.cookies);
    UTILS_renderBorderText("Press [START] to exit.", -1, 210, 1, 0.8);

    float finalSize = -(game.textSize / 6);
    UTILS_renderBorderText(cookieShit, -1, -10 + (20 / (game.textSize + 1)), 1, game.textSize + 1);
    C2D_DrawImageAtRotated(game.sprites.cookie, 200 + (finalSize * 6), 128 + (finalSize * 6), 0, sin((osGetTime() - curTime) / 800) / 8, NULL, finalSize + 1.2, finalSize + 1.2);
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

    return false;
}