#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h> //imports the windows library, i think.
#include <iostream> //deals with opening, reading and writing file
#include <fstream> //specifies that I want to both read and write a file
#include <string> //i have no idea why this is in here
#include <atlstr.h> //i wanna use cstring, i need to do more research on it
#include <shobjidl_core.h> //browsing for files

#define ID_Save 1
#define ID_Open 2
#define ID_Exit 3
#define ID_Undo 4
#define ID_Redo 5
#define ID_VWS 6 


using namespace std;
wstring line;
wfstream inputFile;

wstring inputFileLocation;
wstring textString;



RECT myRect; // makes a rectangle inside the window that text and things can go into

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HMENU hMenubar = CreateMenu();
    HMENU hFile = CreateMenu();
    HMENU hEdit = CreateMenu();
    HMENU hHelp = CreateMenu();

    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, L"File");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEdit, L"Edit");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

    AppendMenu(hFile, MF_STRING, ID_Save, L"Save");
    AppendMenu(hFile, MF_STRING, ID_Open, L"Open");
    AppendMenu(hFile, MF_STRING, ID_Exit, L"Exit");

    AppendMenu(hEdit, MF_STRING, ID_Undo, L"Undo");
    AppendMenu(hEdit, MF_STRING, ID_Redo, L"Redo");

    AppendMenu(hHelp, MF_STRING, NULL, L"You really need help to use this? Damn");

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Notepad for losers",          // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        hMenubar,       // Menu
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
        //if (!TranslateAccelerator(win.Window(), hAccel, &msg))
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
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)); //COLOR_WINDOW is the default windows colours, adding sorts through them


        inputFile.open(inputFileLocation); // opens text file 
        if (inputFile.is_open()) {
            while (getline(inputFile, line)) //gets every line of the txt file, and loads it onto a wide string
            {
                textString = textString + L'\n' + line;
            }

        }
        else {
            
        }

        SetTextColor(hdc, 0x00000000); // sets colour to black
        HFONT aFont = CreateFontA(
            0, //cHeight, height of cell/character
            0, //cWidth, width of font
            0, //cEscapement something graphics related?
            0, //cOrientation, rotation??
            0, //cWeight (boldness)
            0, //bItalic italic font
            0, //bUnderline underlined font
            0, //bStrikeOut striked font
            ANSI_CHARSET, //charset
            0, //something about output
            0, //i cba to note more stuff
            0, //fuck off
            0, // https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createfonta
            "Arial"
        );

        HGDIOBJ fontSelection = SelectObject(hdc, aFont);
        GetClientRect(hwnd, &myRect); //gets the rectangle and sets it to be the size of 
        DrawTextExW( // draws the text, an alternative to this is TextOut, but this one has a lot more formatting options
            hdc,
            &textString[0], //WhY DOES THIS WORK TO CONVERT A WSTRING INTO A LPWSTR
            -1,
            &myRect,
            DT_TOP | DT_WORDBREAK | DT_LEFT,
            NULL
        );

        EndPaint(hwnd, &ps); // finished painting the window
        return 0;
    }

    case WM_CHAR:
    {

        switch (wParam)
        {
        case VK_BACK: //Detects if backspace was pressed
        {
            if (textString.length() != 0) { //failsafe so you don't delete more characters than it has
                textString.erase(textString.length() - 1); //deletes 1 character
                InvalidateRect(hwnd, &myRect, true); //refreshes rect
            }
        }
        default:
            if (wParam != VK_BACK) {
                CString charInput((wchar_t)wParam); //INPUT TO CSTRING, IDK WHAT IT DOES STILL
                wstring wCharInput(charInput);//CString to Wstring, i mean it works
                textString = textString + wCharInput; //adds character
                InvalidateRect(hwnd, &myRect, true); //refreshes rect
            }
        }}
        break;
    
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == ID_Exit) {
            exit(0);
        }
        else if (LOWORD(wParam) == ID_Open) { //I copied the example to open the window from the docs, im sorry
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                IFileOpenDialog* pFileOpen;

                // Create the FileOpenDialog object.
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                    IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

                if (SUCCEEDED(hr))
                {
                    // Show the Open dialog box.
                    hr = pFileOpen->Show(NULL);

                    // Get the file name from the dialog box.
                    if (SUCCEEDED(hr))
                    {
                        IShellItem* pItem;
                        hr = pFileOpen->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                            // Display the file name to the user.
                            if (SUCCEEDED(hr))
                            {
                                inputFileLocation = pszFilePath;
                                CoTaskMemFree(pszFilePath);
                                inputFile.close();
                                InvalidateRect(hwnd, &myRect, true); //refreshes rect
                            }
                            pItem->Release();
                        }
                    }
                    pFileOpen->Release();
                }
                CoUninitialize();
            }
            
        }
    }
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
