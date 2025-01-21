#include "file_dialog.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>

std::string getFileNameDialog(){
    // Reference: https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes#opening-a-file
    // Above provides the link to common box dialog window api that we are using yo open a file diaglog to open a file.
    OPENFILENAME mediaFile;
    char mediaFileName[200]; // we accept an upper limit of 200 chars for a filename
    HWND hwnD = GetActiveWindow(); // get owner window
    HANDLE fileHandle;
    
    ZeroMemory(&mediaFile, sizeof(mediaFile)); // zero initializing memory
    // now we will set some attributes for mediaFile before calling
    // GetOpenFileName() for opening the file dialog
    mediaFile.lStructSize = sizeof(mediaFile);
    mediaFile.hwndOwner = hwnD;
    mediaFile.lpstrFile = (LPSTR)mediaFileName; //LPSTR is a window32 api string datatype 
    mediaFile.lpstrFile[0] = '\0'; // initializing with null byte to prevent GetOpenFileName() from initializing using mediaFileName content
    mediaFile.nMaxFile = sizeof(mediaFileName);
    mediaFile.lpstrFilter = (LPSTR)"Audio Files\0*.MP3\0"; // filter files to only include media files
    mediaFile.nFilterIndex = 1; // default filter index
    mediaFile.lpstrFileTitle = NULL;
    mediaFile.nMaxFileTitle = 0;
    mediaFile.lpstrInitialDir = NULL; // initial dir to look for media files
    mediaFile.Flags =  OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // set flags to ensure existance of files

    GetOpenFileName(&mediaFile);
    
    return std::string(mediaFile.lpstrFile);
}


#else

std::string getFileNameDialog(){
    return "";
}

#endif // PLATFORM_WINDOWS
