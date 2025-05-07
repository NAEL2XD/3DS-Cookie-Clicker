#include <3ds.h>
#include <citro2d.h>
#include <math.h>

// Prework those things.
C2D_Text renderText;
C2D_Font defaultFont;
C2D_TextBuf g_staticBuf;
u32 borderColor;
u32 textColor;

// Use custom function to init first before starting process.
void initBeforeProcess() {
    // If some are NULL, set their default value.
    if (defaultFont == NULL) defaultFont = C2D_FontLoadSystem(CFG_REGION_USA);
    if (g_staticBuf == NULL) g_staticBuf = C2D_TextBufNew(4096);
    borderColor = C2D_Color32(0x00, 0x00, 0x00, 0xAA);
    textColor   = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);

    C2D_TextBufClear(g_staticBuf);
}

void UTILS_renderBorderText(char *text, float x, float y, float borderSize, float size) {
    initBeforeProcess();

    // Clear and Parse to make text work.
    C2D_TextFontParse(&renderText, defaultFont, g_staticBuf, text);
    C2D_TextOptimize(&renderText);

    // If the x variable is -1, then it will be set to middle.
    if (x == -1) x = (-(renderText.width * size) / 2) + 200;

    // Do the trick.
    int pos[8][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 8; i++) {
        float finalMult = borderSize * size;
        C2D_DrawText(&renderText, C2D_WithColor, x + (pos[i][0] * finalMult), y + (pos[i][1] * finalMult), 0.5f, size, size, borderColor);
    }
    C2D_DrawText(&renderText, C2D_WithColor, x, y, 0.5f, size, size, textColor);
}

void UTILS_quickRenderText(char *text, float x, float y, float alpha, float size) {
    initBeforeProcess();

    C2D_TextFontParse(&renderText, defaultFont, g_staticBuf, text);
    C2D_TextOptimize(&renderText);
    C2D_DrawText(&renderText, C2D_WithColor, x, y, 0.5f, size, size, C2D_Color32(0xFF, 0xFF, 0xFF, (u8)alpha));
}