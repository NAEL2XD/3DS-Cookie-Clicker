#include <math.h>
#include <stdlib.h>
#include "utils.h"
#include "main.h"

#define MAX_PRODUCTS 2

typedef struct {
    char text[16]; // Text to display
    float x;       // X-coordinate
    float y;       // Y-coordinate
    int alpha;     // Transparency (alpha value)
} SpawnedText;
SpawnedText spawnedText[50];
int stCount = 0;

typedef struct {
    // Game Properties
	float cookies;
	float textSize;
	float cookiePerPress;
    float cookiePerSecond;

    // Color
    u32 red;
    u32 green;
    u32 black;
    u32 white;
    float upgradeColor;

    // Shop Properties
    bool shopUnlocked;
    bool onShop;
    int shopChoice;

    struct {
        C2D_Image cookie;
        C2D_Image background;
        C2D_Image productInfo;
        C2D_Image smallCookie;
    } sprites;
} gameState;
gameState game;

typedef struct {
    const char* name;
    float price;
    float multiplier;
    bool unlocked;
    int own;
    float cpsInc;

    C2D_Image img;
} ShopItem;
ShopItem shopProps[MAX_PRODUCTS] = {
    // Name,    Price, Multiply, Unlocked, Owns, CPS Increment
    {"Cursor",  10,    1.1,      false,    0,   0.1},
    {"Grandma", 50,    1.05,     false,    0,   1}
};

float updateCPS() {
    float total = 0;
    for (int i = 0; i < (sizeof(shopProps)/sizeof(shopProps[0])); i++) {
        total += (float)shopProps[i].own * shopProps[i].cpsInc; // Example formula
    }
    return total;
}

int game_init() {
    // Initialize Variables
    game.cookies  = 0;
    game.textSize = 0;
    game.cookiePerPress = 1.0;
    game.cookiePerSecond = 0.0;

    // Shop Properties
    game.shopUnlocked = false;
    game.onShop       = false;
    game.shopChoice   = 0;

    // Initialize Colors
    game.red          = C2D_Color32(255,   0, 0,   255);
    game.green        = C2D_Color32(0,   255, 0,   255);
    game.black        = C2D_Color32(0,   0,   0,   255);
    game.white        = C2D_Color32(255, 255, 255, 255);
    game.upgradeColor = 0;

    // Initialize Images
    game.sprites.cookie      = UTILS_loadImage("romfs:/assets/cookie.t3x");
    game.sprites.background  = UTILS_loadImage("romfs:/assets/background.t3x");
    game.sprites.productInfo = UTILS_loadImage("romfs:/assets/productInfo.t3x");
    game.sprites.smallCookie = UTILS_loadImage("romfs:/assets/smallCookie.t3x");
    return 0;
}

int unlocks = -1;
float finalSize = 0;
u64 runningTime;
bool game_updateTOP() {
    runningTime = UTILS_getRunningTime();

    if ((kDown & KEY_DLEFT) && game.shopUnlocked && !game.onShop) {
        game.shopChoice = 0;
        game.onShop = true;
    } else if ((kDown & KEY_DRIGHT) && game.onShop) {
        game.onShop = false;
    }

    if (kDown & KEY_START) {
        return true;
    }

    C2D_DrawImageAt(game.sprites.background, 0, 0, 0, NULL, 1, 1);

    char cookieShit[64];
    snprintf(cookieShit, sizeof(cookieShit), "%d", (int)game.cookies);

    char cpsShit[64];
    snprintf(cpsShit, sizeof(cpsShit), "%.1f cookies per second.", game.cookiePerSecond);

    finalSize = -(game.textSize / 6);
    UTILS_renderBorderText(cookieShit, -1, -10 + (20 / (game.textSize + 1)), 1, game.textSize + 1);
    UTILS_quickRenderText(cpsShit, -1, 40 + (game.textSize * 8), C2D_Color32(255 - game.upgradeColor, 255, 255 - game.upgradeColor, 255), 0.5);
    game.textSize     /= 1.1;
    game.upgradeColor /= 1.075;
    game.cookies += game.cookiePerSecond / 60;

    if (game.onShop) {
        C2D_DrawRectSolid(0, 14 + (30 * game.shopChoice), 0, 114, 32, C2D_Color32(255, 255, 0, 255));
        if (kDown & KEY_A && game.cookies >= (int)shopProps[game.shopChoice].price) {
            game.cookies -= shopProps[game.shopChoice].price;
            shopProps[game.shopChoice].price *= shopProps[game.shopChoice].multiplier;
            shopProps[game.shopChoice].own++;

            game.cookiePerSecond = updateCPS();
            game.upgradeColor = 255;
        }

        // Scrolling
        if (unlocks != 0) {
            if (kDown & KEY_DOWN) {
                game.shopChoice++;
                if (game.shopChoice == unlocks + 1) {
                    game.shopChoice = 0;
                }
            } else if (kDown & KEY_UP) {
                game.shopChoice--;
                if (game.shopChoice == -1) {
                    game.shopChoice = unlocks;
                }
            }
        }
    }

    int productSpawn = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        bool unlocked = game.cookies >= (int)shopProps[i].price;
        if (!shopProps[i].unlocked && unlocked) {
            shopProps[i].unlocked = true;
            game.shopUnlocked     = true;
        } else if (shopProps[i].unlocked) {
            // Check if the image hasn't been loaded yet
            if (shopProps[i].img.tex == NULL) {
                char path[256];
                snprintf(path, sizeof(path), "romfs:/assets/product/%s.t3x", shopProps[i].name);
                shopProps[i].img = UTILS_loadImage(path);
                unlocks++;
            }

            // Draw the things.
            C2D_DrawImageAtRotated(game.sprites.productInfo, 0, 30 + (30 * productSpawn), 0, UTILS_angleToRadians(180), NULL, 2, 1.25);
            C2D_DrawImageAt(shopProps[i].img, 2, 19 + (30 * productSpawn), 0, NULL, 0.75, 0.9);
            C2D_DrawImageAt(game.sprites.smallCookie, 21, 32.5 + (30 * productSpawn), 0, NULL, 0.175, 0.175);
            
            char cost[32];
            snprintf(cost, sizeof(cost), "%d", (int)shopProps[i].price);
            UTILS_quickRenderText(cost, 31, 31 + (30 * productSpawn), unlocked ? game.green : game.red, 0.35);
            UTILS_quickRenderText(shopProps[i].name, 21, 18 + (30 * productSpawn), game.black, 0.5);

            // Increment.
            productSpawn++;
        }
    }

    return false;
}

bool alreadyTouching = false;
bool game_updateBOTTOM() {
    // Controls
    if (UTILS_isTouchingImage(game.sprites.cookie, 160 + (finalSize * 6) - 62, 128 + (finalSize * 6) - 65, finalSize + 1.33) && !game.onShop && !alreadyTouching) {
        game.cookies += game.cookiePerPress;
        game.textSize += 0.2;
            
        if (stCount < 50) {
            char final[16];
            snprintf(final, sizeof(final), "+%d", (int)game.cookiePerPress);

            touchPosition touch;
            hidTouchRead(&touch);
            
            // Add new text to spawnedText array
            snprintf(spawnedText[stCount].text, sizeof(spawnedText[stCount].text), "%s", final);
            spawnedText[stCount].x = touch.px - 14;
            spawnedText[stCount].y = touch.py - 6;
            spawnedText[stCount].alpha = 255;
            
            stCount++;
        }
    }
    alreadyTouching = UTILS_isTouchingImage(game.sprites.cookie, 160 + (finalSize * 6) - 62, 128 + (finalSize * 6) - 65, finalSize + 1.33);

    C2D_DrawImageAt(game.sprites.background, -4, 0, 0, NULL, 1, 1);
    C2D_DrawImageAtRotated(game.sprites.cookie, 160 + (finalSize * 6), 128 + (finalSize * 6), 0, sin((float)runningTime / 512) / 8, NULL, finalSize + 1.33, finalSize + 1.33);

    for (int i = 0; i < stCount; i++) {
        UTILS_quickRenderText(spawnedText[i].text, spawnedText[i].x, spawnedText[i].y, C2D_Color32(255, 255, 255, spawnedText[i].alpha), 1);
        spawnedText[i].alpha -= 3;
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