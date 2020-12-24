#include "mainWindow.h"
#include "Mmsystem.h"

#define WM_CHANGEUISTATE 0x0127
//#define UIS_SET 1
//#define UISF_HIDEFOCUS 0x1

enum WindowId{
	ID_SAVEBUTTON = 1,
    ID_BACKGROUND = ID_SAVEBUTTON + 6,
    ID_BOTTOMTEXT,
	ID_SAVETITLE,
	ID_SECTITLE,
	ID_OPSECTITLE,
	ID_SECTION1,
	ID_SECTION2,
    ID_NEWGAME,
	ID_VIDEO = ID_NEWGAME,
    ID_LOADGAME,
	ID_SOUND = ID_LOADGAME,
    ID_OPTIONS,
	ID_JOYSTICK = ID_OPTIONS,
    ID_QUIT,
	ID_OK = ID_QUIT,
	ID_BACK,
	ID_resSelect, 
	ID_DX7, 
	ID_DX11,
	ID_VD,
	ID_UD_CUSTOMRESX,
	ID_UD_CUSTOMRESY,
	ID_E_CUSTOMRESX,
	ID_E_CUSTOMRESY,
	ID_CB_BPP,
	ID_TB_SndVol,
	ID_TB_MusVol,
	ID_CK_Mute
};

gameConfig *config;

// Labels
HWND bottomText;
HWND L_SecTitle, L_OpSecTitle, L_Section1, L_Section2;
HWND L_SaveTitle;

// Main Menu Buttons
HWND B_NEWGAME, B_LOADGAME, B_OPTIONS, B_QUIT;

// Option Menu Buttons
HWND B_VIDEO, B_SOUND, B_JOYSTICK, B_BACK, R_DX7, R_DX11, R_VD;

HWND B_OK;

// Saves Menu Buttons
HWND B_SAVE[6];

// List Boxes
HWND resSelect;

// ComboBoxs
HWND CB_BPP;

// CustomRes Edits
HWND E_CUSTOMRESX, E_CUSTOMRESY;

// CustomRes UpDowns
HWND UD_CUSTOMRESX, UD_CUSTOMRESY;

// Sound Trackbars
HWND TB_SndVol, TB_MusVol;
HWND TB_SndVolBuddy[2];
HWND TB_MusVolBuddy[2];

// Check Boxes
HWND CK_Mute;

// Game Icon
HICON Icon;

// Background
HBITMAP Background;

// Bitmap buttons
HBITMAP ButtonUpTexture;
HBITMAP ButtonDownTexture;

HBITMAP SaveButtonUpTexture;
HBITMAP SaveButtonDownTexture;

HBITMAP SaveThumbTexture[6];

// Colors
COLORREF ForegroundColor;
COLORREF BackgroundColor;

// Font
HFONT hFont;

// Sounds
char* Button[3];
char* Rollover[2];

RECT FrameRct;

HWND AboutWindow;

HDC displayDeviceHandle;

LRESULT CALLBACK videoWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK optionsWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK savesWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK soundWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void MW_LoadAssets(void){
    int i;
	char Path[MAX_PATH];

	displayDeviceHandle = CreateCompatibleDC (NULL);

	Icon = LoadImageA(NULL, ICONPATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    Background = LoadImageW(NULL, L"../help/Background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    ButtonUpTexture = LoadImageW(NULL, L"../help/buttonUp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    ButtonDownTexture = LoadImageW(NULL, L"../help/buttonDown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    SaveButtonUpTexture = LoadImageW(NULL, L"../help/buttonThumbUp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SaveButtonDownTexture = LoadImageW(NULL, L"../help/buttonThumbDown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	for (i = 0; i < 6; i++){
		sprintf(Path, SAVETHUMBPATH, config->configRoot, i + 1, 0);
		SaveThumbTexture[i] = LoadImageA(NULL, Path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

    ForegroundColor = RGB(255, 255, 255);
    BackgroundColor = RGB(12, 11, 9);
    hFont = CreateFont (13, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);

	FrameRct.top = (long)7;
	FrameRct.bottom = (long)87;
	FrameRct.left = (long)4;
	FrameRct.right = (long)120;

	for (i = 0; i < 3; i++){
		sprintf(Path, "../help/Button%d.wav", i);
		Button[i] = loadWave(Path);
	}
	
	for (i = 0; i < 2; i++){
		sprintf(Path, "../help/Rollover%d.wav", i);
		Rollover[i] = loadWave(Path);
	}
}

void MW_CleanUpAssets(){
	int i;

	for (i = 0; i < 3; i++){
		free(Button[i]);
	}
	for (i = 0; i < 2; i++){
		free(Rollover[i]);
	}
}

void MW_CreateLabels(HWND hwnd){
    bottomText = CreateWindowW(L"static", WindowLabels[LID_MainMenu], WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 485, 410, 35, hwnd, (HMENU) ID_BOTTOMTEXT, NULL, NULL);
	SendMessage(bottomText, WM_SETFONT, (WPARAM) hFont, TRUE);

	L_SaveTitle = CreateWindowW(L"static", NULL, WS_CHILD | SS_LEFT, 30, 180, 90, 35, hwnd, (HMENU) ID_SAVETITLE, NULL, NULL);

	L_SecTitle = CreateWindowW(L"static", NULL, WS_CHILD | SS_LEFT, 30, 200, 150, 35, hwnd, (HMENU) ID_SECTITLE, NULL, NULL);
	L_OpSecTitle = CreateWindowW(L"static", NULL, WS_CHILD | SS_LEFT, 310, 200, 150, 35, hwnd, (HMENU) ID_OPSECTITLE, NULL, NULL);
	L_Section1 = CreateWindowW(L"static", NULL, WS_CHILD | SS_LEFT, 55, 280, 150, 35, hwnd, (HMENU) ID_SECTION1, NULL, NULL);
	L_Section2 = CreateWindowW(L"static", NULL, WS_CHILD | SS_LEFT, 55, 340, 150, 35, hwnd, (HMENU) ID_SECTION2, NULL, NULL);

	SendMessage(L_SaveTitle, WM_SETFONT, (WPARAM) hFont, TRUE);
	SendMessage(L_SecTitle, WM_SETFONT, (WPARAM) hFont, TRUE);
	SendMessage(L_OpSecTitle, WM_SETFONT, (WPARAM) hFont, TRUE);
	SendMessage(L_Section1, WM_SETFONT, (WPARAM) hFont, TRUE);
	SendMessage(L_Section2, WM_SETFONT, (WPARAM) hFont, TRUE);
}

void MW_CreateButtons(HWND hwnd){
	int i, j;
	LPWSTR BArrayNameBuffer[30];

	B_VIDEO = B_NEWGAME = CreateWindowW(L"Button", L"New Game", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 60, 200, 128, 64, hwnd, (HMENU) ID_NEWGAME, NULL, NULL);
    B_SOUND = B_LOADGAME = CreateWindowW(L"Button", L"Load Game", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 60, 269, 128, 64, hwnd, (HMENU) ID_LOADGAME, NULL, NULL);
    B_JOYSTICK = B_OPTIONS = CreateWindowW(L"Button", L"Options", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 60, 338, 128, 64, hwnd, (HMENU) ID_OPTIONS, NULL, NULL);
    
	B_OK = B_QUIT = CreateWindowW(L"Button", L"Quit", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 250, 415, 128, 64, hwnd, (HMENU) ID_QUIT, NULL, NULL);
	B_BACK = CreateWindowW(L"Button", L"Back", WS_CHILD | BS_OWNERDRAW, 90, 415, 128, 64, hwnd, (HMENU) ID_BACK, NULL, NULL);

	R_DX7 = CreateWindowW(L"Button", L"Direct3D Renderer", WS_CHILD | BS_AUTORADIOBUTTON, 55, 215, 150, 30, hwnd, (HMENU) ID_DX7, NULL, NULL);
	R_DX11 = CreateWindowW(L"Button", L"DirectX 11 Renderer", WS_CHILD | BS_AUTORADIOBUTTON, 55, 235, 150, 30, hwnd, (HMENU) ID_DX11, NULL, NULL);
	R_VD = CreateWindowW(L"Button", L"DGVoodoo2 Renderer", WS_CHILD | BS_AUTORADIOBUTTON, 55, 255, 150, 30, hwnd, (HMENU) ID_VD, NULL, NULL);

	CK_Mute = CreateWindowW(L"Button", WindowLabels[LID_Mute], WS_CHILD | BS_CHECKBOX, 55, 225, 130, 30, hwnd, (HMENU) ID_CK_Mute, NULL, NULL);

	for (i = 0; i < 2; i++){
		for (j = 0; j < 3; j++){
			wsprintfW((LPWSTR)BArrayNameBuffer, L"%ls", L"DEFAULT");
			if (SaveThumbTexture[i * 3 + j] == NULL){
				StringCbPrintfW((wchar_t*)BArrayNameBuffer, 30, L"%d - Empty", (i * 3 + j + 1));
			} else {
				StringCbPrintfW((wchar_t*)BArrayNameBuffer, 30, L"%d - Used", (i * 3 + j + 1));
			}
			B_SAVE[i * 3 + j] = CreateWindowW(L"Button", (LPWSTR)BArrayNameBuffer, WS_CHILD | BS_OWNERDRAW, 30 + 140 * j, 204 + 105 * i, 124, 94, hwnd, (HMENU)(UINT_PTR)(ID_SAVEBUTTON + (i * 3 + j)), NULL, NULL);
		}
	}

	SendMessage(R_DX7, WM_SETFONT, (WPARAM) hFont, TRUE);
	SendMessage(R_DX11, WM_SETFONT, (WPARAM) hFont, TRUE);
	SendMessage(R_VD, WM_SETFONT, (WPARAM) hFont, TRUE);
	
	SendMessage(CK_Mute, WM_SETFONT, (WPARAM) hFont, TRUE);
}

void MW_CreateUpDowns(HWND hwnd){
	UD_CUSTOMRESX = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, (HMENU) ID_UD_CUSTOMRESX, NULL, NULL);
	E_CUSTOMRESX = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | ES_RIGHT, 195, 225, 60, 21, hwnd, (HMENU) ID_E_CUSTOMRESX, NULL, NULL); 

	SendMessageW(UD_CUSTOMRESX, UDM_SETBUDDY, (WPARAM) E_CUSTOMRESX, 0);
	SendMessageW(UD_CUSTOMRESX, UDM_SETRANGE, 0, MAKELPARAM(9999, 0));

	UD_CUSTOMRESY = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, (HMENU) ID_UD_CUSTOMRESY, NULL, NULL);
	E_CUSTOMRESY = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, NULL, WS_CHILD | ES_RIGHT, 255, 225, 60, 21, hwnd, (HMENU) ID_E_CUSTOMRESY, NULL, NULL);

	SendMessageW(UD_CUSTOMRESY, UDM_SETBUDDY, (WPARAM) E_CUSTOMRESY, 0);
	SendMessageW(UD_CUSTOMRESY, UDM_SETRANGE, 0, MAKELPARAM(9999, 0));

	SendMessageW(E_CUSTOMRESX, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessageW(E_CUSTOMRESY, WM_SETFONT, (WPARAM)hFont, 0);
}

void MW_CreateComboBoxes(HWND hwnd){
	CB_BPP = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | UDS_ALIGNRIGHT | CBS_DROPDOWNLIST, 315, 225, 70, 110, hwnd, (HMENU) ID_CB_BPP, NULL, NULL);
	
	SendMessageW(CB_BPP, CB_ADDSTRING, 0, (LPARAM)L"32 Bpp");
	SendMessageW(CB_BPP, CB_ADDSTRING, 0, (LPARAM)L"16 Bpp");

	SendMessageW(CB_BPP, WM_SETFONT, (WPARAM)hFont, 0);
}

void MW_CreateListBox(HWND hwnd, res_t resolutions[], int nbOfRes){
	int i;

	resSelect = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTBOXW, NULL, WS_CHILD | LBS_NOTIFY, 55, 300, 320, 110, hwnd, (HMENU) ID_resSelect, NULL, NULL);
	for (i = 0; i < nbOfRes; i++){
		SendMessageW(resSelect, LB_ADDSTRING, 0, (LPARAM)resolutions[i].ResLabel);
	}
	SendMessageW(resSelect, LB_ADDSTRING, 0, (LPARAM)WindowLabels[LID_LB_CustomRes]);
	SendMessageW(resSelect, WM_SETFONT, (WPARAM)hFont, 0);
}

void MW_CreateTrackbars(HWND hwnd){
	int LabelWidth, LabelHeight;
	int TrackbarOffsetY;
	int LabelOffsetY, LabelOffsetLeft, LabelOffsetRight;
	Vector2i TrackbarSize;
	Vector2i TrackbarPos;

	TrackbarOffsetY = 60;
	LabelOffsetLeft = -10;
	LabelOffsetRight = -35;
	LabelOffsetY = 30;
	TrackbarPos.x = 60;
	TrackbarPos.y = 295;

	TrackbarSize.x = 210;
	TrackbarSize.y = 30;

	
	LabelWidth = 30;
	LabelHeight = 20;

	TB_SndVol = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | TBS_NOTICKS | TBS_BOTH, TrackbarPos.x, TrackbarPos.y, TrackbarSize.x, TrackbarSize.y, hwnd, (HMENU) ID_TB_SndVol, NULL, NULL);
	TB_MusVol = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | TBS_NOTICKS | TBS_BOTH, TrackbarPos.x, TrackbarPos.y + TrackbarOffsetY, TrackbarSize.x, TrackbarSize.y, hwnd, (HMENU) ID_TB_MusVol, NULL, NULL);
	
	TB_SndVolBuddy[0] = CreateWindowW(L"STATIC", L"Low", WS_CHILD | SS_RIGHT, TrackbarPos.x + LabelOffsetLeft, TrackbarPos.y + LabelOffsetY, LabelWidth, LabelHeight, hwnd, NULL, NULL, NULL);
	TB_MusVolBuddy[0] = CreateWindowW(L"STATIC", L"Low", WS_CHILD | SS_RIGHT, TrackbarPos.x + LabelOffsetLeft, TrackbarPos.y + TrackbarOffsetY + LabelOffsetY, LabelWidth, LabelHeight, hwnd, NULL, NULL, NULL);

	TB_SndVolBuddy[1] = CreateWindowW(L"STATIC", L"High", WS_CHILD | SS_RIGHT, TrackbarPos.x + TrackbarSize.x + LabelOffsetRight, TrackbarPos.y + LabelOffsetY, LabelWidth, LabelHeight, hwnd, NULL, NULL, NULL);
	TB_MusVolBuddy[1] = CreateWindowW(L"STATIC", L"High", WS_CHILD | SS_RIGHT, TrackbarPos.x + TrackbarSize.x + LabelOffsetRight, TrackbarPos.y + TrackbarOffsetY + LabelOffsetY, LabelWidth, LabelHeight, hwnd, NULL, NULL, NULL);

	// Set font
	SendMessageW(TB_SndVolBuddy[0], WM_SETFONT, (WPARAM)hFont, 0);
	SendMessageW(TB_SndVolBuddy[1], WM_SETFONT, (WPARAM)hFont, 0);
	SendMessageW(TB_MusVolBuddy[0], WM_SETFONT, (WPARAM)hFont, 0);
	SendMessageW(TB_MusVolBuddy[1], WM_SETFONT, (WPARAM)hFont, 0);
}

INT_PTR setWindowTheme(HWND hwnd, HDC hdc){
	SetTextColor(hdc, ForegroundColor); // Change the text color
	SetBkMode(hdc, TRANSPARENT);
	return (INT_PTR)GetStockObject(HOLLOW_BRUSH);
}

void playRandButton(){
	PlaySound(Button[rand()%3], NULL, SND_MEMORY | SND_ASYNC);
}

void playRandRollover(int Options){
	PlaySound(Rollover[rand()%2], NULL, SND_MEMORY | Options);
}

void showAboutWindow(HWND hwnd){
	playRandRollover(SND_ASYNC);
	if (!IsWindow(AboutWindow)){
		AboutWindow = CreateWindowW(L"About", L"About", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 0, 0, 320, 150, 0, 0, NULL, NULL);
	} else {
		SetFocus(AboutWindow);
	}
}
void setMainWindowProc(int nCmdShow, HWND hwnd){
	SetWindowTextW(hwnd, WindowLabels[LID_MainMenuTitle]);
	if (nCmdShow == SW_SHOW){
		SetWindowTextW(B_NEWGAME, L"New Game");
		SetWindowTextW(B_LOADGAME, L"Load Game");
		SetWindowTextW(B_OPTIONS, L"Options");
		SetWindowTextW(B_QUIT, L"Quit");
		
		SetWindowTextW(bottomText, WindowLabels[LID_MainMenu]);
		
		redrawControl(bottomText, hwnd);
	}
	ShowWindow(B_NEWGAME, nCmdShow);
	ShowWindow(B_LOADGAME, nCmdShow);
	ShowWindow(B_OPTIONS, nCmdShow);
	ShowWindow(B_QUIT, nCmdShow);
}

void setOptionsWindowProc(int nCmdShow, HWND hwnd){
	SetWindowTextW(hwnd, WindowLabels[LID_OptionTitle]);
	if (nCmdShow == SW_SHOW){
		SetWindowTextW(B_VIDEO, L"Video");
		SetWindowTextW(B_SOUND, L"Sound");
		SetWindowTextW(B_JOYSTICK, L"Joystick");
		SetWindowTextW(B_QUIT, L"Quit");
		
		SetWindowTextW(bottomText, WindowLabels[LID_OptionsMenu]);

		redrawControl(bottomText, hwnd);
	}
	ShowWindow(B_VIDEO, nCmdShow);
	ShowWindow(B_SOUND, nCmdShow);
	ShowWindow(B_JOYSTICK, nCmdShow);
	ShowWindow(B_BACK, nCmdShow);
}

void setVideoWindowProc(int nCmdShow, HWND hwnd){
	int GameRenderDevice;
	int CurrentResX, CurrentResY, bpp;

	SetWindowTextW(hwnd, WindowLabels[LID_VidOp]);
	if (nCmdShow == SW_SHOW){
		SetWindowTextW(B_OK, L"Ok");

		SetWindowTextW(L_SecTitle, WindowLabels[LID_VidOp]);
		SetWindowTextW(L_OpSecTitle, WindowLabels[LID_CustomRes]);
		SetWindowTextW(L_Section1, WindowLabels[LID_DesRes]);
		SetWindowTextW(bottomText, WindowLabels[LID_VideoMenu]);

		redrawControl(L_SecTitle, hwnd);
		redrawControl(L_OpSecTitle, hwnd);
		redrawControl(L_Section1, hwnd);
		redrawControl(bottomText, hwnd);

		SendMessage(R_DX7, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
		SendMessage(R_DX11, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
		SendMessage(R_VD, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);

		getGameRes(&CurrentResX, &CurrentResY, &bpp, config);
		GameRenderDevice = getGameRenderDevice(config);

		switch (GameRenderDevice){
			case DX7RENDERDEVICE:
				SendMessage(R_DX7, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
				break;

			case DX11RENDERDEVICE:
				SendMessage(R_DX11, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
				break;

			case DGVOODOO2RENDERDEVICE:
				SendMessage(R_VD, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
				SendMessageW(resSelect, LB_ADDSTRING, 0, (LPARAM)WindowLabels[LID_MaxRes]);
				
				if (getVoodooRes(&CurrentResX, &CurrentResY, config)){
					CurrentResY = GetSystemMetrics(SM_CYSCREEN);
					CurrentResX = (int)((CurrentResY / 3.0f) * 4.0f);
				}
				break;
		}

		SendMessageW(UD_CUSTOMRESX, UDM_SETPOS32, 0, CurrentResX);
		SendMessageW(UD_CUSTOMRESY, UDM_SETPOS32, 0, CurrentResY);

		if (bpp == 32){
			SendMessageW(CB_BPP, CB_SETCURSEL, 0, 0);
		} else {
			SendMessageW(CB_BPP, CB_SETCURSEL, 1, 0);
		}
	}
	ShowWindow(B_BACK, nCmdShow);
	ShowWindow(B_OK, nCmdShow);
	
	ShowWindow(L_SecTitle, nCmdShow);
	ShowWindow(L_OpSecTitle, nCmdShow);
	ShowWindow(L_Section1, nCmdShow);
	ShowWindow(resSelect, nCmdShow);

	ShowWindow(R_DX7, nCmdShow);
	ShowWindow(R_DX11, nCmdShow);
	ShowWindow(R_VD, nCmdShow);

	ShowWindow(E_CUSTOMRESX, nCmdShow);
	ShowWindow(UD_CUSTOMRESX, nCmdShow);
	ShowWindow(E_CUSTOMRESY, nCmdShow);
	ShowWindow(UD_CUSTOMRESY, nCmdShow);

	ShowWindow(CB_BPP, nCmdShow);
}

void setSavesWindowProc(int nCmdShow, HWND hwnd){
	int i;

	if (nCmdShow == SW_SHOW){
		if (config->loadSave){
			SetWindowTextW(hwnd, WindowLabels[LID_LoadGame]);
			SetWindowTextW(L_SaveTitle, WindowLabels[LID_LoadGame]);
			SetWindowTextW(bottomText, WindowLabels[LID_LoadMenu]);
		}else{
			SetWindowTextW(hwnd, WindowLabels[LID_NewGame]);
			SetWindowTextW(L_SaveTitle, WindowLabels[LID_NewGame]);
			SetWindowTextW(bottomText, WindowLabels[LID_StartMenu]);
		}
		redrawControl(L_SaveTitle, hwnd);
		redrawControl(bottomText, hwnd);
	}
	for (i = 0; i < 6; i++){
		ShowWindow(B_SAVE[i], nCmdShow);
	}

	ShowWindow(L_SaveTitle, nCmdShow);
	ShowWindow(B_BACK, nCmdShow);
}

void setSoundWindowProc(int nCmdShow, HWND hwnd){
	int i;
	double SoundVolume, MusicVolume;
	BOOL UseSound;
	
	SetWindowTextW(hwnd, WindowLabels[LID_SndOp]);
	if (nCmdShow == SW_SHOW){
		SetWindowTextW(B_OK, L"Ok");

		SetWindowTextW(L_SecTitle, WindowLabels[LID_SndOp]);
		SetWindowTextW(L_Section1, WindowLabels[LID_SndVol]);
		SetWindowTextW(L_Section2, WindowLabels[LID_MusVol]);
		SetWindowTextW(bottomText, WindowLabels[LID_SoundMenu]);
		
		getSoundSettings(&UseSound, &SoundVolume, &MusicVolume, config);

		SendMessage(CK_Mute, BM_SETCHECK, !UseSound, 0);
		SendMessageW(TB_SndVol, TBM_SETPOS, 1, (LPARAM)(SoundVolume * 100));
		SendMessageW(TB_MusVol, TBM_SETPOS, 1, (LPARAM)(MusicVolume * 100));
		
		redrawControl(L_SecTitle, hwnd);
		redrawControl(L_Section1, hwnd);
		redrawControl(L_Section2, hwnd);
		redrawControl(bottomText, hwnd);
	}

	for (i = 0; i < 2; i++){
		ShowWindow(TB_SndVolBuddy[i], nCmdShow);
		ShowWindow(TB_MusVolBuddy[i], nCmdShow);
	}
	ShowWindow(CK_Mute, nCmdShow);

	ShowWindow(TB_SndVol, nCmdShow);
	ShowWindow(TB_MusVol, nCmdShow);

	ShowWindow(L_SecTitle, nCmdShow);
	ShowWindow(L_Section1, nCmdShow);
	ShowWindow(L_Section2, nCmdShow);

	ShowWindow(B_BACK, nCmdShow);
	ShowWindow(B_OK, nCmdShow);
}

LRESULT CALLBACK soundWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    LPDRAWITEMSTRUCT pDIS; // Owner drawn control structure
	BOOL UseSound;
	double SoundVolume, MusicVolume;
    
    switch (msg)
    {

    case WM_CTLCOLORSTATIC: // Called when a static control is about to be drawn
		if (((HWND)lParam == TB_SndVol) || ((HWND)lParam == TB_MusVol)){ //Mmmh
			SetTextColor((HDC)wParam, ForegroundColor); // Change the text color
			SetBkColor((HDC)wParam, BackgroundColor);
			return (INT_PTR) CreateSolidBrush(BackgroundColor);
		}
		return setWindowTheme((HWND)lParam, (HDC)wParam);
        break;

    case WM_DRAWITEM:
        pDIS = (LPDRAWITEMSTRUCT)lParam;
		switch (pDIS->CtlID)
		{
		case ID_OK:
		case ID_BACK:
			DrawBUTTON(pDIS, displayDeviceHandle, ButtonUpTexture, ButtonDownTexture, NULL, NULL, hFont); // We draw our custom button ourselves
			break;

		default:
			break;
		}
    break;

    case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			switch (LOWORD(wParam)) {
				case ID_CK_Mute:
					UseSound = (BOOL)SendMessage(CK_Mute, BM_GETCHECK, 0, 0);
					if (UseSound){
						SendMessage(CK_Mute, BM_SETCHECK, BST_UNCHECKED, 0);
					} else {
						SendMessage(CK_Mute, BM_SETCHECK, BST_CHECKED, 0);
					}
					break;

				case ID_OK:
					playRandRollover(SND_ASYNC);
					UseSound = !SendMessage(CK_Mute, BM_GETCHECK, 0, 0);
					SoundVolume = SendMessageW(TB_SndVol, TBM_GETPOS, 0, 0) / 100.0f;
					MusicVolume = SendMessageW(TB_MusVol, TBM_GETPOS, 0, 0) / 100.0f;
					setSoundSettings(UseSound, SoundVolume, MusicVolume, config);
					break;

				case ID_BACK:
					playRandButton();
					setSoundWindowProc(SW_HIDE, hwnd);
					setOptionsWindowProc(SW_SHOW, hwnd);
					SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)optionsWindowProc);
					break;
			}
			SetFocus(hwnd);
        }
        break;

	case WM_KEYDOWN:
		if (wParam == VK_F1){
			showAboutWindow(hwnd);
		}
		break;

    case WM_DESTROY:
		MW_CleanUpAssets();
        PostQuitMessage(0); // The "return" of our window
        break;
    
    default:
        break;
    }

    // Call the default window procedure to prossess all the remaining window messages (event)
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK savesWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    LPDRAWITEMSTRUCT pDIS; // Owner drawn control structure
    
    switch (msg)
    {

    case WM_CTLCOLORSTATIC: // Called when a static control is about to be drawn
		return setWindowTheme((HWND)lParam, (HDC)wParam);
        break;

    case WM_DRAWITEM:
        pDIS = (LPDRAWITEMSTRUCT)lParam;
		if ((pDIS->CtlID >= ID_SAVEBUTTON) && (pDIS->CtlID <= ID_SAVEBUTTON + 5)){
			if ((SaveThumbTexture[pDIS->CtlID - 1] == NULL) && (config->loadSave)){
				DrawBUTTON(pDIS, displayDeviceHandle, SaveButtonDownTexture, SaveButtonDownTexture, NULL, NULL, hFont); // We draw our custom button ourselves
			} else {
				DrawBUTTON(pDIS, displayDeviceHandle, SaveButtonUpTexture, SaveButtonDownTexture, SaveThumbTexture[pDIS->CtlID - 1], &FrameRct, hFont); // We draw our custom button ourselves
			}
		}else{
			switch (pDIS->CtlID)
			{
			case ID_BACK:
				DrawBUTTON(pDIS, displayDeviceHandle, ButtonUpTexture, ButtonDownTexture, NULL, NULL, hFont); // We draw our custom button ourselves
				break;

			default:
				break;
			}
		}
    break;

    case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			if ((LOWORD(wParam) >= ID_SAVEBUTTON) && (LOWORD(wParam) <= ID_SAVEBUTTON + 5)){
				if ((config->loadSave == 0) || (SaveThumbTexture[LOWORD(wParam) - 1] != NULL)){
					playRandRollover(SND_SYNC);
					config->run = 1;
					config->saveFile = (char)LOWORD(wParam);
					MW_CleanUpAssets();
					PostQuitMessage(0);
					//PatchAndRun(config);
				}
			} else {
				switch (LOWORD(wParam)) {
					case ID_BACK:
						playRandButton();
						setSavesWindowProc(SW_HIDE, hwnd);
						setMainWindowProc(SW_SHOW, hwnd);
						SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)mainWindowProc);
						break;
				}
			}
			SetFocus(hwnd);
        }
        break;

	case WM_KEYDOWN:
		if (wParam == VK_F1){
			showAboutWindow(hwnd);
		}
		break;

    case WM_DESTROY:
		MW_CleanUpAssets();
        PostQuitMessage(0); // The "return" of our window
        break;
    
    default:
        break;
    }

    // Call the default window procedure to prossess all the remaining window messages (event)
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK videoWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	int ret, gameRenderDevice;
	int CustomResX, CustomResY;
    LPDRAWITEMSTRUCT pDIS; // Owner drawn control structure
    
    switch (msg)
    {

    case WM_CTLCOLORSTATIC: // Called when a static control is about to be drawn
		return setWindowTheme((HWND)lParam, (HDC)wParam);
        break;

    case WM_DRAWITEM:
        pDIS = (LPDRAWITEMSTRUCT)lParam;
        switch (pDIS->CtlID)
        {
        case ID_BACK:
		case ID_OK:
            DrawBUTTON(pDIS, displayDeviceHandle, ButtonUpTexture, ButtonDownTexture, NULL, NULL, hFont); // We draw our custom button ourselves
            break;

        default:
            break;
        }
    break;

    case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
            switch (LOWORD(wParam)) {
				case ID_DX7:
				case ID_DX11:
					SendMessageW(resSelect, LB_DELETESTRING, config->nbOfRes + 1, 0);
					break;

				case ID_VD:
					SendMessageW(resSelect, LB_ADDSTRING, 0, (LPARAM)WindowLabels[LID_MaxRes]);
					break;

                case ID_OK:
					playRandRollover(SND_ASYNC);
					ret = (int)SendMessageW(resSelect, LB_GETCURSEL, 0, 0);
					
					if (SendMessage(R_DX7, BM_GETCHECK, 0, 0)){
						gameRenderDevice = DX7RENDERDEVICE;
					} else if (SendMessage(R_DX11, BM_GETCHECK, 0, 0)){
						gameRenderDevice = DX11RENDERDEVICE;
					} else if (SendMessage(R_VD, BM_GETCHECK, 0, 0)){
						gameRenderDevice = DGVOODOO2RENDERDEVICE;
					}

					if (ret > -1){
						if (ret == config->nbOfRes){ // Custom Res
							if (gameRenderDevice == DGVOODOO2RENDERDEVICE){
								CustomResX = (int)SendMessageW(UD_CUSTOMRESX, UDM_GETPOS32, 0, 0);
								CustomResY = (int)SendMessageW(UD_CUSTOMRESY, UDM_GETPOS32, 0, 0);
								ret = getMaxPreset(config, CustomResX, CustomResY);

								setGameRes(config->availableRes[ret].ResX, config->availableRes[ret].ResY, 32 - (int)SendMessageW(CB_BPP, CB_GETCURSEL, 0, 0) * 16, config);
								setVoodooRes(CustomResX, CustomResY, config);
							}else{
								setGameRes((int)SendMessageW(UD_CUSTOMRESX, UDM_GETPOS32, 0, 0), (int)SendMessageW(UD_CUSTOMRESY, UDM_GETPOS32, 0, 0), 32 - (int)SendMessageW(CB_BPP, CB_GETCURSEL, 0, 0) * 16, config);
							}
						} else if (ret == config->nbOfRes + 1) { // Max Res
							CustomResY = GetSystemMetrics(SM_CYSCREEN);
							CustomResX = (int)((CustomResY / 3.0f) * 4.0f);
							SendMessageW(UD_CUSTOMRESX, UDM_SETPOS32, 0, CustomResX);
							SendMessageW(UD_CUSTOMRESY, UDM_SETPOS32, 0, CustomResY);

							setGameRes(1024, 768, 32, config);
							setVoodooRes(0 , 0, config);
						} else { // Preset Res
							SendMessageW(UD_CUSTOMRESX, UDM_SETPOS32, 0, config->availableRes[ret].ResX);
							SendMessageW(UD_CUSTOMRESY, UDM_SETPOS32, 0, config->availableRes[ret].ResY);
							
							if (config->availableRes[ret].bpp == 32){
								SendMessageW(CB_BPP, CB_SETCURSEL, 0, 0);
							} else {
								SendMessageW(CB_BPP, CB_SETCURSEL, 1, 0);
							}
							if (gameRenderDevice == DGVOODOO2RENDERDEVICE){
								setVoodooRes(config->availableRes[ret].ResX, config->availableRes[ret].ResY, config);
								ret = getMaxPreset(config, config->availableRes[ret].ResX, config->availableRes[ret].ResY);
								setGameRes(config->availableRes[ret].ResX, config->availableRes[ret].ResY, config->availableRes[ret].bpp, config);
							}else{
								setGameRes(config->availableRes[ret].ResX, config->availableRes[ret].ResY, config->availableRes[ret].bpp, config);
							}
						}
					}
					
					setGameRenderDevice(config, gameRenderDevice);
					break;

				case ID_BACK:
					playRandButton();
					setVideoWindowProc(SW_HIDE, hwnd);
					setOptionsWindowProc(SW_SHOW, hwnd);
					SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)optionsWindowProc);
					break;
            }
			SetFocus(hwnd);
        }
        break;

	case WM_KEYDOWN:
		if (wParam == VK_F1){
			showAboutWindow(hwnd);
		}
		break;

    case WM_DESTROY:
		MW_CleanUpAssets();
        PostQuitMessage(0); // The "return" of our window
        break;
    
    default:
        break;
    }

    // Call the default window procedure to prossess all the remaining window messages (event)
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK optionsWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    LPDRAWITEMSTRUCT pDIS; // Owner drawn control structure
    
    switch (msg)
    {

    case WM_CTLCOLORSTATIC: // Called when a static control is about to be drawn
		return setWindowTheme((HWND)lParam, (HDC)wParam);
        break;

    case WM_DRAWITEM:
        pDIS = (LPDRAWITEMSTRUCT)lParam;
        switch (pDIS->CtlID)
        {
		case ID_VIDEO:
		case ID_SOUND:
		case ID_JOYSTICK:
        case ID_BACK:
            DrawBUTTON(pDIS, displayDeviceHandle, ButtonUpTexture, ButtonDownTexture, NULL, NULL, hFont); // We draw our custom button ourselves
            break;

        default:
            break;
        }
    break;

    case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			if (LOWORD(wParam) == ID_VIDEO){
				playRandButton();
				setOptionsWindowProc(SW_HIDE, hwnd);
				setVideoWindowProc(SW_SHOW, hwnd);
				SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)videoWindowProc);    
			} else if (LOWORD(wParam) == ID_SOUND){
				playRandButton();
				setOptionsWindowProc(SW_HIDE, hwnd);
				setSoundWindowProc(SW_SHOW, hwnd);
				SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)soundWindowProc); 
			} else if (LOWORD(wParam) == ID_JOYSTICK){
				playRandButton();
				system("start joy.cpl");
				//system("start JoyToKey.exe");
			} else if (LOWORD(wParam) == ID_BACK){
				playRandButton();
				setOptionsWindowProc(SW_HIDE, hwnd);
				setMainWindowProc(SW_SHOW, hwnd);
				SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)mainWindowProc);    
			}
			SetFocus(hwnd);
		}
        break;

	case WM_KEYDOWN:
		if (wParam == VK_F1){
			showAboutWindow(hwnd);
		}
		break;

    case WM_DESTROY:
		MW_CleanUpAssets();
        PostQuitMessage(0); // The "return" of our window
        break;
    
    default:
        break;
    }

    // Call the default window procedure to prossess all the remaining window messages (event)
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Window Procedure
LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    HWND BackgroundWindow;

    LPDRAWITEMSTRUCT pDIS; // Owner drawn control structure
    
    switch (msg)
    {
    case WM_CREATE:
		AboutWindow = NULL;
        config = (gameConfig*) (((LPCREATESTRUCT) lParam)->lpCreateParams);
		CenterWindow(hwnd);
        SetWindowTextW(hwnd, WindowLabels[LID_MainMenuTitle]);
		
		MW_LoadAssets();
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)Icon);
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)Icon);

        BackgroundWindow = CreateWindowW(L"Static", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, 0, 0, 0, hwnd, (HMENU) ID_BACKGROUND, NULL, NULL);
        SendMessage(BackgroundWindow, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) Background);
		
		MW_CreateLabels(hwnd);
        MW_CreateButtons(hwnd);
		MW_CreateListBox(hwnd, config->availableRes, config->nbOfRes);
		MW_CreateUpDowns(hwnd);
		MW_CreateComboBoxes(hwnd);
		MW_CreateTrackbars(hwnd);

		SendMessage(hwnd, WM_CHANGEUISTATE, (WPARAM)(0x10001),(LPARAM)(0)); // Dissable selection rect
        break;

    case WM_CTLCOLORSTATIC: // Called when a static control is about to be drawn
		return setWindowTheme((HWND)lParam, (HDC)wParam);
        break;

    case WM_DRAWITEM:
        pDIS = (LPDRAWITEMSTRUCT)lParam;
        switch (pDIS->CtlID)
        {
        case ID_NEWGAME:
        case ID_LOADGAME:
        case ID_OPTIONS:
        case ID_QUIT:
            DrawBUTTON(pDIS, displayDeviceHandle, ButtonUpTexture, ButtonDownTexture, NULL, NULL, hFont); // We draw our custom button ourselves
            break;

        default:
            break;
        }
    break;

    case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			if (LOWORD(wParam) == ID_NEWGAME){
				playRandButton();
				config->loadSave = 0;
				setMainWindowProc(SW_HIDE, hwnd);
				setSavesWindowProc(SW_SHOW, hwnd);
				SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)savesWindowProc);
			} else if (LOWORD(wParam) == ID_LOADGAME){
				playRandButton();
				config->loadSave = 1;
				setMainWindowProc(SW_HIDE, hwnd);
				setSavesWindowProc(SW_SHOW, hwnd);
				SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)savesWindowProc);
			} else if (LOWORD(wParam) == ID_OPTIONS){
				playRandButton();
				setMainWindowProc(SW_HIDE, hwnd);
				setOptionsWindowProc(SW_SHOW, hwnd);
				SetWindowLongPtrW(hwnd, GWL_WNDPROC, (LONG_PTR)optionsWindowProc);
			} else if (LOWORD(wParam) == ID_QUIT){
				playRandRollover(SND_SYNC);
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
			SetFocus(hwnd);
		}
        break;

	case WM_KEYDOWN:
		if (wParam == VK_F1){
			showAboutWindow(hwnd);
		}
		break;

    case WM_DESTROY:
		MW_CleanUpAssets();
        PostQuitMessage(0); // The "return" of our window
        break;
    
    default:
        break;
    }

    // Call the default window procedure to prossess all the remaining window messages (event)
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}