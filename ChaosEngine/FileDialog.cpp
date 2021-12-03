#include "FileDialog.h"
#include <commdlg.h>
#include <Windows.h> 
#include "mmgr.h"

std::string FileDialog::LoadScene(const char* filter)
{
    OPENFILENAME filetoLoad;
    char size[260] = {};
    ZeroMemory(&filetoLoad, sizeof(OPENFILENAME));
    filetoLoad.lStructSize = sizeof(OPENFILENAME);
    filetoLoad.hwndOwner = GetActiveWindow();
    filetoLoad.lpstrFile = size;
    filetoLoad.nMaxFile = sizeof(filetoLoad);
    filetoLoad.lpstrFilter = filter;
    filetoLoad.nFilterIndex = 1;
    filetoLoad.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&filetoLoad))
        return filetoLoad.lpstrFile;

    return std::string("Error at Loading Scene");
}

std::string FileDialog::SaveScene(const char* filter)
{
    OPENFILENAME fileToSave;
    char size[260] = {};
    ZeroMemory(&fileToSave, sizeof(OPENFILENAME));
    fileToSave.lStructSize = sizeof(OPENFILENAME);
    fileToSave.hwndOwner = GetActiveWindow();
    fileToSave.lpstrFile = size;
    fileToSave.nMaxFile = sizeof(fileToSave);
    fileToSave.lpstrFilter = filter;
    fileToSave.nFilterIndex = 1;
    fileToSave.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileName(&fileToSave))
        return fileToSave.lpstrFile;

    return std::string("Error at Saving Scene");
}
