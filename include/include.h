#ifndef _INCLUDE_H
#define _INCLUDE_H

#define STRSAFE_NO_DEPRECATE // sprintf

#include <windows.h>
#include <wchar.h>
#include <strsafe.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <time.h>

#include "labels.h"
#include "type.h"
#include "ini.h"
#include "functions.h"
#include "mainWindow.h"
#include "aboutWindow.h"
#include "menus.h"

#define CONFIGPATH "/Harry Potter II"
#define GAMEINI "%s/Game.ini"
#define USERINI "%s/User.ini"

#define DEF_GAMEINI "Default.ini"
#define DEF_USERINI "DefUser.ini"

#ifdef _DEBUG
#define DGVOODOOCONF "C:/Program Files/EA Games/Harry Potter and the Chamber of Secrets/system/dgVoodoo.conf"
#else
#define DGVOODOOCONF "dgVoodoo.conf"
#endif

#define SAVETHUMBPATH "%s/Save/Slot%d/Save%d.bmp"
#define ICONPATH "Game.ico"

#define ASSETS "../help/"

#define RUN_COMMAND "Game.exe"
#define LOAD " -LOADGAME=0 -SAVESLOT="
#define START " PrivetDr.unr -SAVESLOT="

#define VERSION L"Version v1.2"
#define GITLINK L"https://github.com/AsayuGit"

#endif