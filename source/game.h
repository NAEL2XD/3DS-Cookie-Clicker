#ifndef GAME_H
#define GAME_H

#include "main.h"

#define MAX_PRODUCTS 3
#define GAME_VER "v0.4.0"

typedef struct {
    const char* name;
    float price;
    float multiplier;
    bool unlocked;
    int own;
    float cpsInc;

    C2D_Image img;
} ShopItem;

// Add a variable in, and it'll be automatically saved!
// If you're adding an array, you will need to modify some of the code! :p  -Nael
typedef struct {
    float cookies;
	float cookiePerPress;
    float cookiePerSecond;
    bool shopUnlocked;
    const char* ver;

    struct {
        int   clicks;
        float totals;
        int   time;
    } stats;

    ShopItem shopProps[MAX_PRODUCTS];
} SaveData;

typedef struct {
    // Game Properties
	float textSize;
    bool  onStats;

    // Color
    u32 red;
    u32 green;
    u32 black;
    u32 white;
    float upgradeColor;

    // Shop Properties
    bool onShop;
    int shopChoice;

    struct {
        C2D_Image cookie;
        C2D_Image background;
        C2D_Image productInfo;
        C2D_Image smallCookie;
    } sprites;

    struct {
        bool cookie;
        bool statButton;
    } touching;

    struct {
        C2D_Font vcr;
    } fonts;
} gameState;

void game_init();
bool game_updateTOP();
bool game_updateBOTTOM();
SaveData* game_getSave(); // Change return type to pointer
C2D_Font game_getVCRFont();
void game_save();

#endif