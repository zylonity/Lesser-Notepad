#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h> //imports the windows library, i think.
#include <iostream> //deals with opening, reading and writing file
#include <fstream> //specifies that I want to both read and write a file
#include <string> //i have no idea why this is in here
#include <atlstr.h> //i wanna use cstring, i need to do more research on it

using namespace std;
wstring line;
wfstream inputFile;
wstring textString;

RECT MyRect; // makes a rectangle inside the window that text and things can go into

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Notepad-- The worse one of all!",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Messages are what the OS receives in input from the mouse or OS, so this deals with what the app is doing, if you change the size, or if you type something in.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: //in charge of painting the window, everything that goes in the window goes here
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps); //the screen i think?

            //Puts a white background that updates with the window
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1)); //COLOR_WINDOW is the default windows colours, adding sorts through them
            
            
            inputFile.open("Text.txt"); // opens text file 
            if (inputFile.is_open()) {
                while (getline(inputFile, line)) //gets every line of the txt file, and loads it onto a wide string
                {
                    textString = textString + L'\n' + line;
                }

            }
            else {
                inputFile.close();
            }
            
            LPCWSTR temp = textString.c_str(); //converts the wide string into a LPCWSTR
            SetTextColor(hdc, 0x00000000); // sets colour to black

            GetClientRect(hwnd, &MyRect); //gets the rectangle and sets it to be the size of 
            DrawText( // draws the text, an alternative to this is TextOut, but this one has a lot more formatting options
                hdc,
                temp,
                -1,
                &MyRect,
                DT_TOP | DT_WORDBREAK | DT_LEFT
            );

           

            EndPaint(hwnd, &ps); // finished painting the window
            return 0;
        }
    case WM_KEYDOWN:
        {
        CString charInput((wchar_t)wParam);
        wstring wCharInput(charInput);
        textString = textString + wCharInput;
        InvalidateRect(hwnd, &MyRect, true);
            //switch (wParam) 
            //{
            //default:
            //    
            //}

        }
        break;
    case WM_SYSDEADCHAR: 
        {
        NULL;
        }
        break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

