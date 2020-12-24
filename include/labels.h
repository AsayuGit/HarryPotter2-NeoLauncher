#ifndef _LABELS_H
#define _LABELS_H

#include "Windows.h"

enum WindowLabelId{
	LID_MainMenu = 0,
	LID_MainMenuTitle,
	LID_OptionsMenu,
	LID_OptionTitle,
	LID_VideoMenu,
	LID_StartMenu,
	LID_LoadMenu,
	LID_SoundMenu,
	LID_VidOp,
	LID_SndOp,
	LID_CustomRes,
	LID_DesRes,
	LID_NewGame,
	LID_LoadGame,
	LID_SndVol,
	LID_MusVol,
	LID_Mute,
	LID_LB_CustomRes,
	LID_MaxRes,
	NBOFWINDOWLABELS
};

extern LPWSTR WindowLabels[NBOFWINDOWLABELS];

enum ErrorLabelId{
	ERROR_FileNotFound = 0,
	ERROR_CantReadFile,
	ERROR_CantWriteFile,
	ERROR_InvalidRenderDevice,
	ERROR_CantPatchSettings,
	ERROR_CantReadSoundSettings,
	ERROR_CantReadVideoSettings,
	ERROR_CantReadRenderSettings,
	ERROR_CantAllocateMemory,
	NBOFERRORLABELS	
};

extern char* ErrorLabels[NBOFERRORLABELS];

enum LabelID{
	LBL_LoadingConfig = 0,
	LBL_SettingGameRes,
	LBL_SettingVoodooRes,
	LBL_SettingRenderDevice,
	LBL_PatchingSettings,
	LBL_GameStarting,
	LBL_Free,
	LBL_SettingSoundSet,
	LBL_ConfigToDefault,
	NBOFLABELS
};

extern char* Labels[NBOFLABELS];

#endif