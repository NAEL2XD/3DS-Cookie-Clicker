#include <3ds.h>
#include <citro2d.h>
#include <math.h>

// Prework those things.
// Text Functions
C2D_Text renderText;
C2D_Font defaultFont;
C2D_TextBuf g_staticBuf;
u32 borderColor;
u32 textColor;

// Time Functions
u64 oldTime = 0;

// Use custom function to init first before starting process.
bool initialized = false;
void initBeforeProcess() {
    if (initialized) return;

    // TEXT
    // If some are NULL, set their default value.
    if (defaultFont == NULL) defaultFont = C2D_FontLoadSystem(CFG_REGION_USA);
    if (g_staticBuf == NULL) g_staticBuf = C2D_TextBufNew(4096);
    borderColor = C2D_Color32(0,   0,   0,  0xAA);
    textColor   = C2D_Color32(255, 255, 255, 255);
    
    // TIME
    oldTime = osGetTime();
    
    initialized = true;
}

// If the x variable is -1, then it will be set to middle.
float centerX(float xCheck, C2D_Text text, float size) {
    return (int)xCheck == -1 ? (-(text.width * size) / 2) + 200 : xCheck;
}

void UTILS_Init() {
    initBeforeProcess();
}

void UTILS_renderBorderText(const char *text, float x, float y, float borderSize, float size) {
    initBeforeProcess();
    
    // Clear and Parse to make text work.
    C2D_TextBufClear(g_staticBuf);
    C2D_TextFontParse(&renderText, defaultFont, g_staticBuf, text);
    C2D_TextOptimize(&renderText);

    // Center X 
    x = centerX(x, renderText, size);
    
    // Do the trick.
    int pos[8][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 8; i++) {
        float finalMult = borderSize * size;
        C2D_DrawText(&renderText, C2D_WithColor, x + (pos[i][0] * finalMult), y + (pos[i][1] * finalMult), 0.5f, size, size, borderColor);
    }
    C2D_DrawText(&renderText, C2D_WithColor, x, y, 0.5f, size, size, textColor);
}

void UTILS_quickRenderText(const char *text, float x, float y, u32 col, float size) {
    initBeforeProcess();
    
    // Clear and Parse to make text work.
    C2D_TextBufClear(g_staticBuf);
    C2D_TextFontParse(&renderText, defaultFont, g_staticBuf, text);
    C2D_TextOptimize(&renderText);

    // Center X 
    x = centerX(x, renderText, size);

    // Check col
    if (col == 0) col = C2D_Color32(255, 255, 255, 255);

    // Draw!
    C2D_DrawText(&renderText, C2D_WithColor, x, y, 0.5f, size, size, col);
}

u64 UTILS_getRunningTime() {
    initBeforeProcess();

    u64 newTime   = osGetTime();
    return newTime - oldTime;
}

C2D_Image UTILS_loadImage(char *file) {
    initBeforeProcess();

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad(file);
    if (!spriteSheet) {
        printf("Failed to load sprite sheet: %s\n", file);
        return (C2D_Image){0}; // Return an empty image on failure
    }
    return C2D_SpriteSheetGetImage(spriteSheet, 0);
}

double UTILS_angleToRadians(double angle) {
    initBeforeProcess();

    return angle * (M_PI / 180.0);
}

bool UTILS_isTouchingImage(C2D_Image img, float x, float y, float size) {
    touchPosition touch;
    hidTouchRead(&touch);

    bool collision = (x < touch.px && x + img.tex->width > touch.px && y < touch.py && y + img.tex->height > touch.py);
    return collision;
}