#include "menus.h"

void SettingsMenu(gameConfig* config, int resX, int resY, int bpp){
    if ((resX == 0) && ( resY == 0)){ // Prompt Mode
        getGameRes(&resX, &resY, &bpp, config);
        printf("Current resolution %dx%d\n", resX, resY);

        printf("Please enter the new resolution (Ex: 1920 1440):\n");
        scanf("%d %d", &resX, &resY);
    }
    setGameRes(resX, resY, bpp, config);
    getGameRes(&resX, &resY, &bpp, config);
	printf("Resolution set to %dx%d:%d\n", resX, resY, bpp);
}

void MainMenu(HINSTANCE hInstance, gameConfig* config){
    MSG msg; // Window messages (events)
    // Window class declaration
    WNDCLASSW MainWindow = {0}; // We declare and initialise at 0 our Window class
	WNDCLASSW AboutWindow = {0};

	// Main Window
	MainWindow.lpszClassName = L"MainWindow"; // We set the name of out window class
    MainWindow.hInstance = hInstance; // We pass on the window handle of our app to the window
    MainWindow.hbrBackground = CreateSolidBrush(RGB(12, 11, 9));
    MainWindow.lpfnWndProc = mainWindowProc; // Pointer to the window procedure
    MainWindow.hCursor = LoadCursor(0, IDC_ARROW); // Set the cursor used when hovering above the window.
    RegisterClassW(&MainWindow); // Register our windows within the Win32 api for further use
    CreateWindowW(MainWindow.lpszClassName, L"Harry Potter and the Chamber of Secrets", WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX | WS_VISIBLE, 0, 0, 480, 590, 0, 0, hInstance, config);
	
	// AboutWindow
	AboutWindow.lpszClassName = L"About";
    AboutWindow.hInstance = hInstance;
    AboutWindow.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    AboutWindow.lpfnWndProc = aboutWindowProc;
    AboutWindow.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&AboutWindow);

    // Window event handeling
    while (GetMessage(&msg, NULL, 0, 0)){ // Get all messages from both the thread and the window (Play the role of the main loop)
        TranslateMessage(&msg); // Virtual key to characters
        DispatchMessage(&msg); // Send the messages to their windows
    }
	
	// Running the game if needed / cleanup
	PatchAndRun(config);
}