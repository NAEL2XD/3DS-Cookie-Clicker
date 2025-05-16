#ifndef _utils_h_
#define _utils_h_

#include <3ds.h>
#include <citro2d.h>

/**
    Initializes `UTILS`, giving you useful functions.
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
    @param font *(Optional)* Font to use, can be NULL for default nintendo font.
**/
void UTILS_renderBorderText(const char *text, float x, float y, float borderSize, float size, C2D_Font font);

/**
    Quickly renders a bordered text without having to use any special variables.

    @param text String text to write as.
    @param x The x position to draw in. If `x` is set to `-1` then it will make the text go in middle.
    @param y The y position to draw in.
    @param col u32 color to set as. Optional, can be `NULL` (will be white).
    @param size Size for the text to draw. NOTE that `borderSize` will be multiplied by `size`!
    @param font *(Optional)* Font to use, can be NULL for default nintendo font.
**/
void UTILS_quickRenderText(const char *text, float x, float y, u32 col, float size, C2D_Font font);

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

/**
    @brief Returns converted angle to radians. Also the same as [this](https://www.rapidtables.com/convert/number/degrees-to-radians.html).
    @param angle The angle to convert to radians.
    @return The converted radian.
**/
double UTILS_angleToRadians(double angle);

/**
    @brief Returns whatever if it's touching the image or not in the bottom screen.
    @param img The image to use and check if it's touching.
    @param x The X Position of the image.
    @param y The Y Position of the image.
    @param size The Size of the image.
    @return `true` if it collides and is touching the image, `false` otherwise.
**/
bool UTILS_isTouchingImage(C2D_Image img, float x, float y, float size);

/**
    @brief Same as `UTILS_isTouchingImage` but it's a hitbox instead.

    Returns whatever if it's touching the hitbox or not in the bottom screen.

    @param x The X Position of the hitbox.
    @param y The Y Position of the hitbox.
    @param width The Width of the hitbox.
    @param height The Height of the hitbox.
    @return `true` if it collides and is touching the hitbox, `false` otherwise.
**/
bool UTILS_isTouchingHitbox(float x, float y, float width, float height);

/**
    @brief Reads the input text from user and returns the char result.
    @return Char String with text inputted to user.
**/
char* UTILS_swkbdGetInputText();

#endif