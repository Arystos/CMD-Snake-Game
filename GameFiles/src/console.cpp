#include "console.h"
#include <windows.h>
#include <iostream>

using namespace std;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ResizeAndCenterConsole(int width, int height) {
    // Set console title
    SetConsoleTitle(TEXT("Snake Game")); // Use the TEXT macro to convert the string to a wide-character string

    // Resizes, repositions, and modifies console window properties.

    // Get handle to console and desktop windows
    HWND console = GetConsoleWindow();
    const HWND hDesktop = GetDesktopWindow();

    // Get desktop dimensions
    RECT desktop;
    GetWindowRect(hDesktop, &desktop);

    // Calculate new console position to center it
    int consolePosX = (desktop.right - width) / 2;
    int consolePosY = (desktop.bottom - height) / 2;

    // Move and resize console window
    MoveWindow(console, consolePosX, consolePosY, width, height, TRUE);

    // Hide the cursor
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    // If the cmd window is closed in the meantime off the game enable it again 
    // (otherwise the mouse is frozen)
    if (console == NULL) {
        AllocConsole();
        console = GetConsoleWindow();
    }

    // Disable maximize button and resizing of console window
    LONG lStyle = GetWindowLong(console, GWL_STYLE);
    lStyle &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
    SetWindowLong(console, GWL_STYLE, lStyle);
    HMENU hMenu = GetSystemMenu(console, FALSE);
    EnableMenuItem(hMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED);
}