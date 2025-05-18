#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "main.h"
#include "game.h"

#define SAVE_PATH "sdmc:/Nael2xd/CookieClicker/save.txt"

typedef struct {
    char text[16]; // Text to display
    float x;       // X-coordinate
    float y;       // Y-coordinate
    int alpha;     // Transparency (alpha value)
} SpawnedText;
SpawnedText spawnedText[50];
int stCount = 0;

gameState game;
SaveData save;

float updateCPS() {
    float total = 0;
    for (int i = 0; i < (sizeof(save.shopProps)/sizeof(save.shopProps[0])); i++) {
        total += (float)save.shopProps[i].own * save.shopProps[i].cpsInc; // Example formula
    }
    return total;
}

C2D_TextBuf duh;
void game_init() {
    duh = C2D_TextBufNew(4096);

    // Initialize Variables
    save.cookies = 0;
    game.textSize = 0;
    save.cookiePerPress = 1.0;
    save.cookiePerSecond = 0.0;
    game.onStats = false;
    game.fonts.vcr = C2D_FontLoad("romfs:/fonts/vcr.bcfnt");
    save.ver = GAME_VER;

    // Initialize Stats
    save.stats.clicks = 0;
    save.stats.totals = 0;
    save.stats.time   = 0;

    // Shop Properties
    save.shopUnlocked = false;
    game.onShop       = false;
    game.shopChoice   = 0;
    ShopItem initialShopProps[MAX_PRODUCTS] = {
        {"Cursor",  10,  1.1,  false, 0, 0.1},
        {"Grandma", 50,  1.05, false, 0, 1},
        {"Farm",    500, 1.01, false, 0, 8}
    };
    memcpy(save.shopProps, initialShopProps, sizeof(initialShopProps));

    // Initialize Colors
    game.red          = 0xFFFF0000;
    game.green        = 0xFF00FF00;
    game.black        = 0xFF000000;
    game.white        = 0xFFFFFFFF;
    game.upgradeColor = 0;

    // Initialize Images
    game.sprites.cookie      = UTILS_loadImage("romfs:/assets/cookie.t3x");
    game.sprites.background  = UTILS_loadImage("romfs:/assets/background.t3x");
    game.sprites.productInfo = UTILS_loadImage("romfs:/assets/productInfo.t3x");
    game.sprites.smallCookie = UTILS_loadImage("romfs:/assets/smallCookie.t3x");

    FILE *fp = fopen(SAVE_PATH, "rb");
    if (!fp) {
        UTILS_sendNotification("Welcome to Cookie Clicker.", UTILS_getContentFromFile("romfs:/data/welcome.txt"));
        return; // If file doesn't exist, return to stop the other functions.
    }
    unsigned char checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
        checksum ^= fgetc(fp);
    }

    unsigned char checksumCheck = 0;
    const char* csPath = "sdmc:/Nael2xd/CookieClicker/checksum.txt";
    FILE *c = fopen(csPath, "rb");
    if (!c) goto corrupt;
    fread(&checksumCheck, sizeof(checksumCheck), 1, c);
    fclose(c);

    if (checksum == checksumCheck) { // Matches
        FILE *file = fopen(SAVE_PATH, "rb");
        fread(&save, sizeof(SaveData), 1, file);
        fclose(file);

        if (strcmp(save.ver, GAME_VER) != 0) {
            save.ver = GAME_VER;

            char vNum[32];
            snprintf(vNum, sizeof(vNum), "Cookie Clicker %s", GAME_VER);
            UTILS_sendNotification(vNum, UTILS_getContentFromFile("romfs:/data/release.txt"));
        }
    } else { // Doesn't match (corrupted!)
        corrupt:
        remove(SAVE_PATH);
        remove(csPath);

        UTILS_popupError(UTILS_getContentFromFile("romfs:/data/corrupted.txt"));
    }
}

C2D_Text text;
int      unlocks = -1;
float    finalSize = 0;
u64      runningTime;
u64      oldGTime = 0; // For stats.time
bool game_updateTOP() {
    C2D_TextBufClear(duh);
    runningTime = UTILS_getRunningTime();
    if (oldGTime == 0) oldGTime = runningTime;

    if ((kDown & KEY_DLEFT) && save.shopUnlocked && !game.onShop) {
        game.shopChoice = 0;
        game.onShop = true;
    } else if ((kDown & KEY_DRIGHT) && game.onShop) {
        game.onShop = false;
    }

    C2D_DrawImageAt(game.sprites.background, 0, 0, 0, NULL, 1, 1);

    char cookieShit[64];
    snprintf(cookieShit, sizeof(cookieShit), "%d", (int)save.cookies);

    char cpsShit[64];
    snprintf(cpsShit, sizeof(cpsShit), "%.1f cookies per second.", save.cookiePerSecond);

    finalSize = -(game.textSize / 6);
    UTILS_renderBorderText(cookieShit, -1, -10 + (20 / (game.textSize + 1)), 1, game.textSize + 1, NULL);
    UTILS_quickRenderText(cpsShit, -1, 40 + (game.textSize * 8), C2D_Color32(255 - game.upgradeColor, 255, 255 - game.upgradeColor, 255), 0.5, NULL);
    game.textSize     /= 1.1;
    game.upgradeColor /= 1.075;
    save.cookies      += save.cookiePerSecond / 60;
    save.stats.totals += save.cookiePerSecond / 60;

    if (game.onShop) {
        C2D_DrawRectSolid(0, 14 + (31 * game.shopChoice), 0, 114, 32, C2D_Color32(255, 255, 0, 255));
        if (kDown & KEY_A && save.cookies >= (int)save.shopProps[game.shopChoice].price) {
            save.cookies -= save.shopProps[game.shopChoice].price;
            save.shopProps[game.shopChoice].price *= save.shopProps[game.shopChoice].multiplier;
            save.shopProps[game.shopChoice].own++;

            save.cookiePerSecond = updateCPS();
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
        bool unlocked = save.cookies >= (int)save.shopProps[i].price;
        if (!save.shopProps[i].unlocked && unlocked) {
            save.shopProps[i].unlocked = true;
            save.shopUnlocked          = true;
        } else if (save.shopProps[i].unlocked) {
            // Check if the image hasn't been loaded yet
            if (save.shopProps[i].img.tex == NULL) {
                char path[256];
                snprintf(path, sizeof(path), "romfs:/assets/product/%s.t3x", save.shopProps[i].name);
                save.shopProps[i].img = UTILS_loadImage(path);
                unlocks++;
            }

            char cost[32];
            snprintf(cost, sizeof(cost), "%d", (int)save.shopProps[i].price);
            char have[32];
            snprintf(have, sizeof(have), "%d", (int)save.shopProps[i].own);
            
            // Draw the things.
            C2D_DrawImageAtRotated(game.sprites.productInfo, 0, 30 + (31 * productSpawn), 0, UTILS_angleToRadians(180), NULL, 2, 1.25);
            C2D_DrawImageAt(save.shopProps[i].img, 2, 19 + (31 * productSpawn), 0, NULL, 0.75, 0.9);
            C2D_DrawImageAt(game.sprites.smallCookie, 21, 32.5 + (31 * productSpawn), 0, NULL, 0.175, 0.175);

            // Calculate text width
            C2D_TextFontParse(&text, game.fonts.vcr, duh, have);
            C2D_TextOptimize(&text);
        
            float xPos = 102 - (text.width * .875);

            UTILS_quickRenderText(have, xPos, 17 + (31 * productSpawn), C2D_Color32(150, 75, 0, 80), 0.8, game.fonts.vcr);
            UTILS_quickRenderText(cost, 31, 31 + (31 * productSpawn), unlocked ? game.green : game.red, 0.35, NULL);
            UTILS_quickRenderText(save.shopProps[i].name, 21, 18 + (31 * productSpawn), game.black, 0.5, NULL);

            // Increment.
            productSpawn++;
        }
    }

    while (runningTime - oldGTime > 1000) {
        save.stats.time++;
        oldGTime += 1000;
    }

    return false;
}

typedef struct {
    char* name;
    float value;
    bool  isInt;
} statUI;
bool game_updateBOTTOM() {
    if (game.onStats) { // Unused, but will be used later.
        
    } else {
        // Controls
        if (UTILS_isTouchingImage(game.sprites.cookie, 160 + (finalSize * 6) - 62, 128 + (finalSize * 6) - 65, finalSize + 1.33) && !game.touching.cookie) {
            save.cookies      += save.cookiePerPress;
            save.stats.totals += save.cookiePerPress;
            save.stats.clicks++;
            game.textSize += 0.2;

            if (stCount < 50) {
                char final[16];
                snprintf(final, sizeof(final), "+%d", (int)save.cookiePerPress);

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
        game.touching.cookie = UTILS_isTouchingImage(game.sprites.cookie, 160 + (finalSize * 6) - 62, 128 + (finalSize * 6) - 65, finalSize + 1.33);

        C2D_DrawImageAt(game.sprites.background, -4, 0, 0, NULL, 1, 1);
        C2D_DrawImageAtRotated(game.sprites.cookie, 160 + (finalSize * 6), 128 + (finalSize * 6), 0, sin((float)runningTime / 512) / 8, NULL, finalSize + 1.33, finalSize + 1.33);
    }

    for (int i = 0; i < stCount; i++) {
        if (!game.onStats) UTILS_quickRenderText(spawnedText[i].text, spawnedText[i].x, spawnedText[i].y, C2D_Color32(255, 255, 255, spawnedText[i].alpha), 1, NULL);
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
    
    C2D_DrawRectSolid(238, 203, 0, 90, 90, game.white);
    C2D_DrawRectSolid(240, 205, 0, 90, 90, C2D_Color32(0,   0,   0,   255));
    UTILS_quickRenderText("Menu", 247, 208, game.white, 1, NULL);
    
    if (UTILS_isTouchingHitbox(238, 203, 90, 90) && !game.touching.statButton) {
        //game.onStats = !game.onStats;
        switchState(2);
    }
    game.touching.statButton = UTILS_isTouchingHitbox(235, 200, 90, 90);
    
    return false;
}

SaveData* game_getSave() {
    return &save; // Return address of global 'save'
}

C2D_Font game_getVCRFont() {
    return game.fonts.vcr;
}

void game_save() {
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        save.shopProps[i].img.tex = NULL; // Setting img tex to NULL so that if you relaunch it'll load image instead of doing nothing.
    }

    const char* n = SAVE_PATH;
    FILE *file = fopen(n, "wb");
    fwrite(&save, sizeof(SaveData), 1, file);
    fclose(file);

    FILE *fp = fopen(n, "rb");
    unsigned char checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
        checksum ^= fgetc(fp);
    }
    fclose(fp);

    FILE *c = fopen("sdmc:/Nael2xd/CookieClicker/checksum.txt", "wb");
    fwrite(&checksum, sizeof(checksum), 1, c);
    fclose(c);
}