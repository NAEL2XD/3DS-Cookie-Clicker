#ifndef _utils_h_
#define _utils_h_

#include <3ds.h>
#include <citro2d.h>

/**
    Initializes `UTILS`, giving you useful functions.
    You can rather use the other functions to initialize but it's not recommended to use since it can make other options far off.
**/
void UTILS_Init();

/**
    Renders a bordered text quickly, no `C2D_Text` required
    @param text String text to write as.
    @param x The x position to draw in. If `x` is set to `-1` then it will make the text go in middle.
    @param y The y position to draw in.
    @param borderSize Size for the border in pixels to draw.
    @param size Size for the text to draw.
                `NOTE` that `borderSize` will be multiplied by `size`!
**/
void UTILS_renderBorderText(char *text, float x, float y, float borderSize, float size);

/**
    Quickly renders a bordered text without having to use any special variables.

    You cannot set any colored text since it's for "quick" rendering.

    @param text String text to write as.
    @param x The x position to draw in. If `x` is set to `-1` then it will make the text go in middle.
    @param y The y position to draw in.
    @param alpha Visibility to set as, can be 0 - 255.
    @param size Size for the text to draw. NOTE that `borderSize` will be multiplied by `size`!
**/
void UTILS_quickRenderText(char *text, float x, float y, float alpha, float size);

/**
    @brief Gets the current time the application has ran.
    @return A u64 (float) variable by how many milliseconds it has been ran.
**/
u64 UTILS_getRunningTime();

/**
    Loads an image without having to use special properties (like `C2D_SpriteSheet`)

    ### You will need to initialize ROMFS using `romfsInit()` next to `gfxInitDefault()`!!
    @param file The file name to load in romfs, make sure the image is a .t3x! Example: `romfs:/image.t3x`
    @return The image variable stored in there.
**/
C2D_Image UTILS_loadImage(char *file);

#endif