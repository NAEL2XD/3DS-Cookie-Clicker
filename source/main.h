#ifndef MAIN_H
#define MAIN_H

#include <3ds.h>
#include <citro2d.h>
#include "stdio.h"

// Declare variables as extern
extern int state;
extern u32 kDown;

void switchState(int state2Switch);

#endif