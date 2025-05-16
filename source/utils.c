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

// Touch Functions
touchPosition touch;

// Use custom function to init first before starting process.
bool initialized = false;
void initBeforeProcess() {
    if (initialized) return;

    // TEXT
    // If some are NULL, set their default value.
    defaultFont = C2D_FontLoadSystem(CFG_REGION_USA);
    g_staticBuf = C2D_TextBufNew(4096);
    borderColor = C2D_Color32(0,   0,   0,   170);
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

void UTILS_renderBorderText(const char *text, float x, float y, float borderSize, float size, C2D_Font font) {
    // Clear and Parse to make text work.
    C2D_TextBufClear(g_staticBuf);
    C2D_TextFontParse(&renderText, font ? font : defaultFont, g_staticBuf, text);
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

void UTILS_quickRenderText(const char *text, float x, float y, u32 col, float size, C2D_Font font) {
    // Clear and Parse to make text work.
    C2D_TextBufClear(g_staticBuf);
    C2D_TextFontParse(&renderText, font ? font : defaultFont, g_staticBuf, text);
    C2D_TextOptimize(&renderText);

    // Center X 
    x = centerX(x, renderText, size);

    // Check col
    if (col == 0) col = C2D_Color32(255, 255, 255, 255);

    // Draw!
    C2D_DrawText(&renderText, C2D_WithColor, x, y, 0.5f, size, size, col);
}

u64 UTILS_getRunningTime() {
    return osGetTime() - oldTime;
}

C2D_Image UTILS_loadImage(char *file) {
    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad(file);
    if (!spriteSheet) {
        printf("Failed to load sprite sheet: %s\n", file);
        return (C2D_Image){0}; // Return an empty image on failure
    }
    return C2D_SpriteSheetGetImage(spriteSheet, 0);
}

double UTILS_angleToRadians(double angle) {
    return angle * (M_PI / 180.0);
}

bool UTILS_isTouchingImage(C2D_Image img, float x, float y, float size) {
    hidTouchRead(&touch);
    return (x < touch.px && x + img.tex->width > touch.px && y < touch.py && y + img.tex->height > touch.py);
}

bool UTILS_isTouchingHitbox(float x, float y, float width, float height) {
    hidTouchRead(&touch);
    return (x < touch.px && x + width > touch.px && y < touch.py && y + height > touch.py);
}

char* UTILS_swkbdGetInputText() { // Return char pointer
    static char text[512];
    SwkbdState swkbd;

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, -1);
	swkbdSetFeatures(&swkbd, SWKBD_MULTILINE | SWKBD_DARKEN_TOP_SCREEN | SWKBD_FIXED_WIDTH);
	swkbdSetHintText(&swkbd, "Enter Text Here.");
    swkbdSetInitialText(&swkbd, text);
	swkbdInputText(&swkbd, text, sizeof(text));

    return text;
}

void UTILS_popupError(const char* text) {
    // Use this inc
    errorConf errorCtx;
    
    // Initialize error context
    errorInit(&errorCtx, ERROR_TEXT, CFG_LANGUAGE_EN);
    
    // Configure error text (newlines will work in ERROR_TEXT mode)
    errorText(&errorCtx, text);
    
    // Disable home button
    errorCtx.homeButton = false;
    errorCtx.errorCode = 20102; // piplup would love this apparently
    
    // Display the error (blocks until dismissed)
    errorDisp(&errorCtx);
}

void UTILS_sendNotification(const char* titleText, const char* descText) {
    // Initialize News
    newsInit();

    // Convert strings to UTF-16
    u16 title[128] = {0};
    u16 message[1024] = {0};
    
    // Convert ASCII to UTF-16
    for (size_t i = 0; i < strlen(titleText); i++) title[i] = titleText[i];
    for (size_t i = 0; i < strlen(descText); i++)  message[i] = descText[i];

    // Create notification
    NEWS_AddNotification(title, strlen(titleText) + 2, message, strlen(descText) + 2, NULL, 0, false);
    newsExit();
}