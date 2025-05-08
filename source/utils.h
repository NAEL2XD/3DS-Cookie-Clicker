#ifndef UTILS_H
#define UTILS_H

#include <citro2d.h>

void UTILS_renderBorderText(char *text, float x, float y, float borderSize, float size);
void UTILS_quickRenderText(char *text, float x, float y, float alpha, float size);
C2D_Image UTILS_loadImage(char *file);

#endif