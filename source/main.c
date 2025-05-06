#include <3ds.h>
#include <citro2d.h>
#include "utils.h"

typedef struct {
	int cookies;
	float textSize;
} gameState;

int main() {
	gfxInitDefault();
	romfsInit();
	cfguInit();
	consoleInit(GFX_BOTTOM, NULL);

	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Prepare();

	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	gameState game;
	game.cookies  = 0;
	game.textSize = 0;
	
	printf("Cookie!!\n");

	// Main loop
	int state = 1;
	while (aptMainLoop()) {
		hidScanInput();

		u32 kDown = hidKeysDown();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
		C2D_SceneBegin(top);
		
		bool exit = false;
		switch (state) {
			case 1:
				C2D_Text fire;
				char cookieShit[64];
				snprintf(cookieShit, sizeof(cookieShit), "%d", game.cookies);
				UTILS_renderBorderText(fire, cookieShit, -1, 20 / (game.textSize + 1), 1, game.textSize + 1);
				game.textSize /= 1.1;
			
				// Your code goes here
				if (kDown & KEY_A) {
					game.cookies++;
					game.textSize = 0.5;
				}
				if (kDown & KEY_START) exit = true;

				break;
		}

		if (exit) break;

		C3D_FrameEnd(0);
	}

	C3D_Fini();
	C2D_Fini();
	romfsInit();
	cfguExit();
	gfxExit();
	return 0;
}