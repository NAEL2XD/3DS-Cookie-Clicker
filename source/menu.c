#include "utils.h"
#include "game.h"

typedef struct {
    struct {
        bool back;
    } touching;

    struct {
        C2D_Font vcr;
    } fonts;

    int type;
} Menu;
Menu menu;

typedef struct {
    char* name;
    float value;
    bool  isInt;
} statUI;

const char* strType[2] = {"Stats", "Exit"};

C2D_TextBuf duhM;
int frame = 0;
void menu_init() {
    duhM = C2D_TextBufNew(4096);
    menu.fonts.vcr = game_getVCRFont();

    for (int i = 0; i < 1000; i++) menu.type = -1;
    menu.touching.back = true;

    frame = 0;
}

bool menu_updateTOP() {
    char d[64];
    snprintf(d, 64, "Cookie Clicker %s", GAME_VER);
    UTILS_renderBorderText(d, -1, 60, 1, 1, NULL);
    return false;
}

C2D_Text textM;
bool menu_updateBOTTOM() {
    C2D_TextBufClear(duhM);
    C2D_DrawRectSolid(0, 210, 0, 400, 30, 0xFFFFFFFF);
    C2D_DrawRectSolid(0, 212, 0, 400, 30, 0xFF000000);
    UTILS_quickRenderText("Go Back", 115, 211, 0xFFFFFFFF, 1, NULL);

    // Corrected loop condition
    if (menu.type == -1) {
        for (int i = 0; i < sizeof(strType)/sizeof(strType[0]); i++) {
            int s[4] = {24 + ((i % 2) * 150), 20, 124, 44};
            C2D_DrawRectSolid(s[0] - 2, s[1] - 2, 0, s[2] + 4, s[3] + 4, 0xFFFFFFFF);
            C2D_DrawRectSolid(s[0], s[1], 0, s[2], s[3], 0xFF000000);
            UTILS_quickRenderText(strType[i], 54 + ((i % 2) * 150), 26, 0xFFFFFFFF, 1, NULL);
    
            if (UTILS_isTouchingHitbox(s[0], s[1], s[2], s[3])) {
                menu.type = i;
                break;
            }
        }
    }

    if (UTILS_isTouchingHitbox(0, 212, 400, 30) && !menu.touching.back) {
        if (menu.type == -1) {
            switchState(1); // Game
        } else {
            menu.type = -1;
        }
    }
    menu.touching.back = UTILS_isTouchingHitbox(0, 212, 400, 30);

    if (frame > 10) {
        switch(menu.type) {
            case 0: { // Stats (enclosed in braces)
                /*statUI blocks[3] = {
                    {"Cookie Clicks", saveD.stats.clicks, true},
                    {"Total Cookies", saveD.stats.totals, false},
                    {"Time Wasted",   saveD.stats.time,   true}
                };
    
                for (int i = 0; i < 3; i++) {
                    char var[24];
                    snprintf(var, sizeof(var), "%.1f", blocks[i].value);
                    if (blocks[i].isInt) {
                        snprintf(var, sizeof(var), "%i", (int)blocks[i].value);
                    }
                
                    if (strcmp(blocks[i].name, "Time Wasted") == 0) {
                        int t = (int)blocks[i].value;
                        int m = t / 60;
                        int s = t % 60;
                        snprintf(var, sizeof(var), "%02d:%02d", m, s);
                    }
                
                    C2D_TextFontParse(&textM, menu.fonts.vcr, duhM, var);
                    C2D_TextOptimize(&textM);
                
                    float xPos = 306 - (textM.width * 1.14);
                
                    C2D_DrawRectSolid(6, 33 + (34 * i), 0, 307, 2, 0xFFFFFFFF);
                    UTILS_quickRenderText(blocks[i].name, 8, 10 + (34 * i), 0xFFFFFFFF, 0.8, NULL);
                    UTILS_quickRenderText(var, xPos, 2 + (34 * i), 0xFFFFFFFF, 1, menu.fonts.vcr);
                }*/
                UTILS_sendNotification("About STATS Button", UTILS_getContentFromFile("romfs:/data/statNotImplemented.txt"));
                break;
            }
            case 1: {
                game_save();
                return true;
            }
        }
    }

    frame++;
    
    return false;
}