#include "include.h"
#include "io.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    int argc;
	int argi;
	int i;
	char* configFilePath[MAX_PATH];
	char* userFilePath[MAX_PATH];
	LPWSTR* argv;
	gameConfig config = {
		0,
		1, 
		1,
		5,
		2,
		NULL,
		NULL,
		NULL,
		NULL,
		{
			{
				640,
				480, 
				32,
				L"640x480:32 Bpp [4/3]"
			},
			{
				800,
				600, 
				32,
				L"800x600:32 Bpp [4/3]"
			},
			{
				1024,
				768, 
				32,
				L"1024x768:32 bpp [4/3]"
			},
			{
				1440,
				1080, 
				32,
				L"1440x1080:32 Bpp [4/3]"
			},
			{
				1920,
				1440, 
				32,
				L"1920x1440:32 Bpp [4/3]"
			}
		}
	};

#ifdef _DEBUG
	BOOL CMD;
	// Open Console
	CMD = AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
	// Init
	srand((unsigned int)time(NULL));
	config.configRoot = (char*)malloc(sizeof(char)*MAX_PATH);
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, config.configRoot); // Get doccument path
	i = 0;
	while (config.configRoot[i] != '\0'){
		i++;
		if (config.configRoot[i] == '\\'){
			config.configRoot[i] = '/';
		}
	}
	sprintf(config.configRoot, "%s%s", config.configRoot, CONFIGPATH);
	
	sprintf((char*)configFilePath, GAMEINI, config.configRoot);
	sprintf((char*)userFilePath, USERINI, config.configRoot);
    printf("Config path :\n>%s<\n>%s<\n>%s<\n>%s<\n\n", config.configRoot, configFilePath, userFilePath, DGVOODOOCONF);
	
	if (access((const char*)config.configRoot, 0) != 0){ // Check if the Config directory exists
		printf(Labels[LBL_BuildConfig]);
		CreateDirectory((const char*)config.configRoot, NULL);
	}
	if (access((const char*)configFilePath, 0) != 0){ // Check if the config file exists
		printf(Labels[LBL_ConfigToDefault], "game");
		fileCopy(DEF_GAMEINI, (char*)configFilePath); // Copy the default config file to "My Documents/Harry Potter II" Directory to be used by the game
	}
	if (access((const char*)userFilePath, 0) != 0){
		printf(Labels[LBL_ConfigToDefault], "user");
		fileCopy(DEF_USERINI, (char*)userFilePath);
	}
	
	//access(DGVOODOOCONF, 0));

    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv == NULL){
        fprintf(stderr, "Couldn't parse the command line arguments !\n");
    }

    // Handle arguments
    if (argc > 1){ 
        config.configFile = iniLoadFromFile((const char*)configFilePath);
        if (config.configFile == NULL){
            fprintf(stderr, "Couldn't oppen the config file !\n");
            return -1;
        }

        argi = 1;
        while (argi < argc){
            
            if (lstrcmpW(argv[argi], L"-SetResolution") == 0){ // Change the resolution
                if (argi + 4 <= argc){
                    SettingsMenu(&config, _wtoi(argv[argi + 1]), _wtoi(argv[argi + 2]), _wtoi(argv[argi + 3]));
                    argi += 4;
                }else{
                    argi++;
                }
            }else if (lstrcmpW(argv[argi], L"-SetSave") == 0){ // Change the savefile
                if (argi + 2 <= argc){
                    config.saveFile = _wtoi(argv[argi + 1]);
                    argi += 2;
                }else{
                    argi++;
                }
            }else if (lstrcmpW(argv[argi], L"-NewGame") == 0){ // Start a new game on the selected save
                config.loadSave = 0;
                argi++;
            }else{
                argi++;
            }
        }
        iniSaveToFile(config.configFile);
        iniFree(config.configFile);
    }
    LocalFree(argv);

    MainMenu(hInstance, &config);
    return 0;
}