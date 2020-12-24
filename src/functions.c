#include "functions.h"

char* substr(char* Src, int Start, int End){
    char* string;

    string = (char*)malloc(sizeof(char)*(End - Start + 1));
    memcpy(string, &Src[Start], End - Start);
    string[End - Start] = '\0';

    return string;
}

int fileCopy(char* Src, char* Dst){
	FILE* SrcFile;
	FILE* DstFile;
	size_t SrcFileSize;
	char* FileData;

	SrcFile = fopen(Src, "rb"); // Try to open the source file in "read binary" mode
	if (SrcFile == NULL){
		fprintf(stderr, ErrorLabels[ERROR_FileNotFound], Src);
		printf(ErrorLabels[ERROR_FileNotFound], Src);
		goto Error;
	}
	fseek(SrcFile, 0, SEEK_END); // Go to the end of the file
	SrcFileSize = ftell(SrcFile); // Get the file size
	rewind(SrcFile); // Go back to the beginin of the file

	FileData = (char*)malloc(sizeof(char)*SrcFileSize);
	if (FileData == NULL){
		fprintf(stderr, ErrorLabels[ERROR_CantAllocateMemory]);
		printf(ErrorLabels[ERROR_CantAllocateMemory]);
		goto Error;
	}

	DstFile = fopen(Dst, "wb"); // Open (and create) the destination file in "write binary" mode

	if (fread(FileData, 1, SrcFileSize, SrcFile) != SrcFileSize){ // Tries to load the content of the source file into memory
		fprintf(stderr, ErrorLabels[ERROR_CantReadFile], Src);
		printf(ErrorLabels[ERROR_CantReadFile], Src);
		goto Error;
	}

	if (fwrite(FileData, 1, SrcFileSize, DstFile) != SrcFileSize){ // Trices to write the data into the destination file from memory
		fprintf(stderr, ErrorLabels[ERROR_CantReadFile], Dst);
		printf(ErrorLabels[ERROR_CantReadFile], Dst);
		goto Error;
	}
	fclose(SrcFile);
	fclose(DstFile);

	return 0;
Error:
	if (SrcFile)
		fclose(SrcFile);
	if (DstFile)
		fclose(DstFile);
	if (FileData)
		free(FileData);

	remove(Dst); // In case of error we remove the residual file
	return -1;
}

int loadConfigFile(ini** configFile, char* configFilePath){
	if (*configFile == NULL){
		printf(Labels[LBL_LoadingConfig], configFilePath);
		*configFile = iniLoadFromFile(configFilePath);
	}
	if (*configFile == NULL){
		fprintf(stderr, ErrorLabels[ERROR_FileNotFound], configFilePath);
		printf(ErrorLabels[ERROR_FileNotFound], configFilePath);
		return -1;
	}
	return 0;
}

int getSoundSettings(BOOL* UseSound, double* SoundVolume, double* MusicVolume, gameConfig* config){
	char* configFilePath[MAX_PATH];
	char* UseSoundValue, *SoundVolumeValue, *MusicVolumeValue;
	
	if (config->configFile == NULL){
		sprintf((char*)configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, (char*)configFilePath) != 0)
			return -1;
	}
	
	UseSoundValue = iniGetKey(config->configFile, "Engine.GameEngine", "UseSound");
	SoundVolumeValue = iniGetKey(config->configFile, "ALAudio.ALAudioSubsystem", "SoundVolume");
	MusicVolumeValue = iniGetKey(config->configFile, "ALAudio.ALAudioSubsystem", "MusicVolume");

	if (!(UseSoundValue && SoundVolumeValue && MusicVolumeValue))
		goto Error;

	if (strcmp(UseSoundValue, "True") == 0){
		*UseSound = TRUE;
	}else{
		*UseSound = FALSE;
	}

    *SoundVolume = atof(SoundVolumeValue);
	*MusicVolume = atof(MusicVolumeValue);

	return 0;

Error:
	fprintf(stderr, ErrorLabels[ERROR_CantReadSoundSettings]);
	printf(ErrorLabels[ERROR_CantReadSoundSettings]);
	return -1;
}

int setSoundSettings(BOOL UseSound, double SoundVolume, double MusicVolume, gameConfig* config){
	char* CUseSound;
	char* CSoundVolume;
	char* CMusicVolume;
	char* configFilePath[MAX_PATH];

	if (config->configFile == NULL){
		sprintf((char*)configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, (char*)configFilePath) != 0)
			return -1;
	}

	CUseSound = (char*)malloc(sizeof(char)*6);
	CSoundVolume = (char*)malloc(sizeof(char)*9);
	CMusicVolume = (char*)malloc(sizeof(char)*9);

	if (UseSound){
		sprintf(CUseSound, "True");
	} else {
		sprintf(CUseSound, "False");
	}

	sprintf(CSoundVolume, "%lf", SoundVolume);
	sprintf(CMusicVolume, "%lf", MusicVolume);

	printf(Labels[LBL_SettingSoundSet]);

	iniSetKey(config->configFile, "Engine.GameEngine", "UseSound", CUseSound);
	iniSetKey(config->configFile, "ALAudio.ALAudioSubsystem", "SoundVolume", CSoundVolume);
	iniSetKey(config->configFile, "ALAudio.ALAudioSubsystem", "MusicVolume", CMusicVolume);

	return 0;
}

int getGameRes(int* resX, int* resY, int* bpp, gameConfig* config){
	char* configFilePath[MAX_PATH];
	char* resXValue, *resYValue, *bppValue;

	if (config->configFile == NULL){
		sprintf((char*)configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, (char*)configFilePath) != 0)
			return -1;
	}
	resXValue = iniGetKey(config->configFile, "WinDrv.WindowsClient", "FullscreenViewportX");
	resYValue = iniGetKey(config->configFile, "WinDrv.WindowsClient", "FullscreenViewportY");
	bppValue = iniGetKey(config->configFile, "WinDrv.WindowsClient", "FullscreenColorBits");

	if (!(resXValue && resYValue && bppValue))
		goto Error;

    *resX = atoi(resXValue);
    *resY = atoi(resYValue);
	*bpp = atoi(bppValue);

	return 0;

Error:
	fprintf(stderr, ErrorLabels[ERROR_CantReadVideoSettings]);
	printf(ErrorLabels[ERROR_CantReadVideoSettings]);
	return -1;
}

int setGameRes(int resX, int resY, int bpp, gameConfig* config){
    char* CresX;
    char* CresY;
	char* Cbpp;
	char* configFilePath[MAX_PATH];

	if (config->configFile == NULL){
		sprintf((char*)configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, (char*)configFilePath) != 0)
			return -1;
	}

    CresX = (char*)malloc(sizeof(char)*10);
    CresY = (char*)malloc(sizeof(char)*10);
	Cbpp = (char*)malloc(sizeof(char)*10);
    sprintf(CresX, "%d", resX);
    sprintf(CresY, "%d", resY);
	sprintf(Cbpp, "%d", bpp);
    
	printf(Labels[LBL_SettingGameRes], resX, resY, bpp);

    iniSetKey(config->configFile, "WinDrv.WindowsClient", "FullscreenViewportX", CresX);
    iniSetKey(config->configFile, "WinDrv.WindowsClient", "FullscreenViewportY", CresY);
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "FullscreenColorBits", Cbpp);

	return 0;
}

int getVoodooRes(int* resX, int* resY, gameConfig* config){
	char* Resolution;

	if (config->voodooConf == NULL){
		if (loadConfigFile(&config->voodooConf, DGVOODOOCONF) != 0)
			return -1;
	}

	Resolution = iniGetKey(config->voodooConf, "DirectX", "Resolution");

	if (!Resolution)
		goto Error;

	if (sscanf(Resolution, "h:%d, v:%d", resX, resY) != 2)
		return 1;
	
	return 0;

Error:
	fprintf(stderr, ErrorLabels[ERROR_CantReadVideoSettings]);
	printf(ErrorLabels[ERROR_CantReadVideoSettings]);
	return -1;
}

int setVoodooRes(int resX, int resY, gameConfig* config){
    char* Resolution;

	if (config->voodooConf == NULL){
		if (loadConfigFile(&config->voodooConf, DGVOODOOCONF) != 0)
			return -1;
	}
	Resolution = (char*)malloc(sizeof(char)*20);

	printf(Labels[LBL_SettingVoodooRes]);
	if ((resX == 0) && (resY == 0)){
		sprintf(Resolution, "max");
	}else{
		sprintf(Resolution, "h:%d, v:%d", resX, resY);
	}
	printf("%s\n", Resolution);
	iniSetKey(config->voodooConf, "DirectX", "Resolution", Resolution);
	return 0;
}

int getGameRenderDevice(gameConfig* config){
	char* GameRenderDevice;
	char* passtrough;
	char* configFilePath[MAX_PATH];

	if (config->configFile == NULL){
		sprintf((char*)configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, (char*)configFilePath) != 0)
			return -1;
	}

	if (config->voodooConf == NULL){
		if (loadConfigFile(&config->voodooConf, DGVOODOOCONF) != 0)
			return -1;
	}

	GameRenderDevice = iniGetKey(config->configFile, "Engine.Engine", "GameRenderDevice");
	passtrough = iniGetKey(config->voodooConf, "DirectX", "DisableAndPassThru");
	if (!(GameRenderDevice && passtrough))
		goto Error;

	if (strcmp(GameRenderDevice, "D3DDrv.D3DRenderDevice") == 0){
		if (strcmp(passtrough, "true") == 0){
			return DX7RENDERDEVICE;
		} else {
			return DGVOODOO2RENDERDEVICE;
		}
	} else if (strcmp(GameRenderDevice, "D3D11Drv.D3D11RenderDevice") == 0){
		return DX11RENDERDEVICE;
	} else {
		return 0;
	}

Error:
	fprintf(stderr, ErrorLabels[ERROR_CantReadRenderSettings]);
	printf(ErrorLabels[ERROR_CantReadRenderSettings]);
	return -1;
}

int setGameRenderDevice(gameConfig* config, int RenderDevice){
    char* passtrough;
	char* GameRenderDevice;
	char* configFilePath[MAX_PATH];

	if (config->configFile == NULL){
		sprintf((char*)configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, (char*)configFilePath) != 0)
			return -1;
	}

	if (config->voodooConf == NULL){
		if (loadConfigFile(&config->voodooConf, DGVOODOOCONF) != 0)
			return -1;
	}

    GameRenderDevice = (char*)malloc(sizeof(char)*27);
	passtrough = (char*)malloc(sizeof(char)*6);

	printf(Labels[LBL_SettingRenderDevice]);
	switch (RenderDevice){
		case DX7RENDERDEVICE:
			printf("Direct3D\n");
			sprintf(GameRenderDevice, "D3DDrv.D3DRenderDevice");
			sprintf(passtrough, "true"); // Enable DGVoodoo2 directX Passtrough
			break;

		case DX11RENDERDEVICE:
			printf("DirectX 11\n");
			sprintf(GameRenderDevice, "D3D11Drv.D3D11RenderDevice");
			break;

		case DGVOODOO2RENDERDEVICE:
			printf("DGVoodoo2\n");
			sprintf(GameRenderDevice, "D3DDrv.D3DRenderDevice");
			sprintf(passtrough, "false"); // Disable DGVoodoo2 directX Passtrough
			break;
		
		default:
			printf(ErrorLabels[ERROR_InvalidRenderDevice]);
			break;
	}
    iniSetKey(config->configFile, "Engine.Engine", "GameRenderDevice", GameRenderDevice);
	iniSetKey(config->voodooConf, "DirectX", "DisableAndPassThru", passtrough);
	
	return 0;
}

int getMaxPreset(gameConfig* config, int CustomResX, int CustomResY){
	int i;

	for (i = 1; i <= config->maxInternalResID; i++){
		if ((config->availableRes[i].ResX > CustomResX) || (config->availableRes[i].ResY > CustomResY)){
			break;
		}
	}
	return i - 1;
}

int fixGameSettings(gameConfig* config){
	char configFilePath[MAX_PATH];
	char userFilePath[MAX_PATH];
	
	if (config->configFile == NULL){
		sprintf(configFilePath, GAMEINI, config->configRoot);
		if (loadConfigFile(&config->configFile, configFilePath) != 0)
			return -1;
	}

	if (config->userFile == NULL){
		sprintf(userFilePath, USERINI, config->configRoot);
		if (loadConfigFile(&config->userFile, userFilePath) != 0)
			return -1;
	}

	printf(Labels[LBL_PatchingSettings]);

	// Set texture and object details to the highest settings
	iniSetKey(config->userFile, "Engine.PlayerPawn", "ObjectDetail", "ObjectDetailVeryHigh");
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "SkinDetail", "High");
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "TextureDetail", "High");
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "LowDetailTextures", "False");
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "NoDynamicLights", "False");

	// Fix the transparency issues
	iniSetKey(config->configFile, "D3DDrv.D3DRenderDevice", "UsePrecache", "False");

	// Fix the camea spin
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "DeadZoneRUV", "True");
	iniSetKey(config->configFile, "WinDrv.WindowsClient", "DeadZoneXYZ", "True");
	
	// CD Quality audio
	iniSetKey(config->configFile, "Audio.GenericAudioSubsystem", "OutputRate", "44100Hz");
	return 0;
}

void RunGame(int argc, char *argv[], int saveFile, char load){
	char command[MAX_PATH];
	char* args;
    size_t cmdLength;

	if (load){
		args = LOAD;
	}else{
		args = START;
	}
	cmdLength = strlen(args) + 2; // +1 Savefile number, +1 '\0'
	sprintf(command, "start /wait %s%s%d", RUN_COMMAND, args, saveFile);

    printf("%s", command);
	system(command);
	//spawnl(P_NOWAIT, RUN_COMMAND, command, NULL);
}

void PatchAndRun(gameConfig* config){
		if (fixGameSettings(config) != -1){
		printf(Labels[LBL_Free]);
		if (config->configFile){
			iniSaveToFile(config->configFile);
			iniFree(config->configFile);
		}
		if (config->userFile){
			iniSaveToFile(config->userFile);
			iniFree(config->userFile);
		}
		if (config->voodooConf){
			iniSaveToFile(config->voodooConf);
			iniFree(config->voodooConf);
		}
		if (config->run)
			RunGame(0, NULL, config->saveFile, config->loadSave);
	}else{
		fprintf(stderr, ErrorLabels[ERROR_CantPatchSettings]);
		printf(ErrorLabels[ERROR_CantPatchSettings]);
	}
}

void CenterWindow(HWND hwnd){
    RECT rc = {0};
    int win_w, win_h;
    int screen_w, screen_h;

    // Get the window's with and height
    GetWindowRect(hwnd, &rc);
    win_w = rc.right - rc.left;
    win_h = rc.bottom - rc.top;

    // Get the screen's resolution
    screen_w = GetSystemMetrics(SM_CXSCREEN);
    screen_h = GetSystemMetrics(SM_CYSCREEN);

    // Set the windows x and y corrdinates relative to the screen (calculated to be centered)
    SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w) / 2, (screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}

void DrawBUTTON(LPDRAWITEMSTRUCT pDIS, HDC displayDeviceHandle, HBITMAP ButtonUpTexture, HBITMAP ButtonDownTexture, HBITMAP Frame, RECT* FrameRct, HFONT hFont){
    int buttonTextLength;
    BITMAP bm ;
    LPWSTR buttonText;
    HBITMAP buttonStateTexture;
	RECT ButtonRect;
	Vector2i Offset;

    buttonTextLength = GetWindowTextLengthW(pDIS->hwndItem) + 1;
	buttonText = (LPWSTR)malloc(sizeof(WCHAR)*buttonTextLength); // We reserve some memory to store the text
    GetWindowTextW(pDIS->hwndItem, buttonText, buttonTextLength);
    buttonTextLength--;

    if(pDIS->itemState & ODS_SELECTED){
        buttonStateTexture = ButtonDownTexture;
    }else{
        buttonStateTexture = ButtonUpTexture;
    }

	GetWindowRect(pDIS->hwndItem, &ButtonRect);
    GetObject(buttonStateTexture, sizeof(BITMAP), &bm); // Get the texture information
    
	Offset.x = ((ButtonRect.right - ButtonRect.left) - bm.bmWidth) >> 1;
	Offset.y = ((ButtonRect.bottom - ButtonRect.top) - bm.bmHeight) >> 1;

	//SetBkColor(pDIS->hDC, RGB(0, 0, 0));
	FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(0));
	
	SelectObject(displayDeviceHandle, buttonStateTexture); // Select an object to be used with the display device
	BitBlt(pDIS->hDC, Offset.x, Offset.y, bm.bmWidth, bm.bmHeight, displayDeviceHandle, 0, 0, SRCCOPY); // Draw the texture to the button

	if ((Frame != NULL) && (FrameRct != NULL)){
		SelectObject(displayDeviceHandle, Frame);
		BitBlt(pDIS->hDC, FrameRct->left, FrameRct->top, FrameRct->right - FrameRct->left, FrameRct->bottom - FrameRct->top, displayDeviceHandle, FrameRct->left, FrameRct->top, SRCCOPY);
	}

    SetBkMode(pDIS->hDC, TRANSPARENT);
    SetTextColor(pDIS->hDC, RGB(255, 255, 255));
    SelectObject(pDIS->hDC, hFont);
    DrawTextW(pDIS->hDC, buttonText, buttonTextLength, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	free(buttonText); // We free the memory we allocated to prevent memory leaks
}

void redrawControl(HWND control, HWND window){
	RECT rect;

	GetClientRect(control, &rect);
	InvalidateRect(control, &rect, TRUE);
	MapWindowPoints(control, window, (POINT*) &rect, 2);
	RedrawWindow(window, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);
}

char* loadWave(char* Filename){
	char* wavBuffer;
	size_t FileSize;
	FILE* wavFile;

	wavFile = fopen(Filename, "rb");
	if (wavFile == NULL){
		fprintf(stderr, ErrorLabels[ERROR_FileNotFound], Filename);
		printf(ErrorLabels[ERROR_FileNotFound], Filename);
        goto Error;
	}

	fseek(wavFile, 0, SEEK_END);
	FileSize = ftell(wavFile);
	rewind(wavFile);

	wavBuffer = (char*)malloc(sizeof(char)*(FileSize));
	if (fread(wavBuffer, 1, FileSize, wavFile) != FileSize){
		fprintf(stderr, ErrorLabels[ERROR_CantReadFile], Filename);
		printf(ErrorLabels[ERROR_CantReadFile], Filename);
        goto Error;
	}
	
	fclose(wavFile);
	return wavBuffer;
Error:
	if(wavFile)
		fclose(wavFile);
	return NULL;
}