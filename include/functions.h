#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "include.h"
#include "Shlobj.h"

char* substr(char* Src, int Start, int End);
int fileCopy(char* Src, char* Dst);

int getSoundSettings(BOOL* UseSound, double* SoundVolume, double* MusicVolume, gameConfig* config);
int setSoundSettings(BOOL UseSound, double SoundVolume, double MusicVolume, gameConfig* config);

int getGameRes(int* resX, int* resY, int* bpp, gameConfig* config);
int setGameRes(int resX, int resY, int bpp, gameConfig* config);

int getVoodooRes(int* resX, int* resY, gameConfig* config);
int setVoodooRes(int resX, int resY, gameConfig* config);

int getGameRenderDevice(gameConfig* config);
int setGameRenderDevice(gameConfig* config, int RenderDevice);

int getMaxPreset(gameConfig* config, int CustomResX, int CustomResY);

int fixGameSettings(gameConfig* config);
void RunGame(int argc, char *argv[], int saveFile, char load);
void PatchAndRun(gameConfig* config);

void CenterWindow(HWND hwnd);
void DrawBUTTON(LPDRAWITEMSTRUCT pDIS, HDC displayDeviceHandle, HBITMAP ButtonUpTexture, HBITMAP ButtonDownTexture, HBITMAP Frame, RECT* FrameRct, HFONT hFont);
void redrawControl(HWND control, HWND window);

char* loadWave(char* Filename);

#endif