#include "aboutWindow.h"

LPWSTR AboutText = L"Harry Potter 2 Neo Launcher\n\n2020 - Asayu";

LRESULT CALLBACK aboutWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch (msg)
    {
    case WM_CREATE:
		CenterWindow(hwnd);
		CreateWindowW(L"Static", AboutText, WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 5, 320, 200, hwnd, (HMENU) 1, NULL, NULL);
		CreateWindowW(L"Static", VERSION, WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 75, 320, 20, hwnd, (HMENU) 2, NULL, NULL);
		CreateWindowW(L"Static", GITLINK, WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 95, 320, 20, hwnd, (HMENU) 3, NULL, NULL);
		//SendMessage(hwnd, WM_CHANGEUISTATE, (WPARAM)(0x10001),(LPARAM)(0)); // Dissable selection rect
        break;
    
    default:
        break;
    }

    // Call the default window procedure to prossess all the remaining window messages (event)
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}