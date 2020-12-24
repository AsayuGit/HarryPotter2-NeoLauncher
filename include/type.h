#ifndef _TYPE_H
#define _TYPE_H

#include "windows.h"
#include "ini.h"
//typedef struct gameConfig gameConfig;

typedef struct{
	int ResX;
	int ResY;
	int bpp;
	LPWSTR ResLabel;
} res_t;

typedef struct{
	char run;
    char saveFile;
    char loadSave;
	int nbOfRes;
	int maxInternalResID;
	char* configRoot;
	ini* configFile;
	ini* userFile;
	ini* voodooConf;
	res_t availableRes[];
} gameConfig;

enum {
	DX7RENDERDEVICE = 1,
	DX11RENDERDEVICE,
	DGVOODOO2RENDERDEVICE
} gameRenderDevice;

typedef struct{
	int x;
	int y;
} Vector2i;

#endif