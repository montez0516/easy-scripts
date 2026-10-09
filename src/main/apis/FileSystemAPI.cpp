#include "FileSystemAPI.h"
#include "../../api/Register.h"

#include <windows.h>
#include <commdlg.h>
#include <filesystem>

FileAPI::FileAPI(APIRegister &apiRegister)
{
    apiRegister.registerMethod("files.filepicker", [this](APIRequest request)
                               { return openFilePicker(request); });
}

APIResponse FileAPI::openFilePicker(APIRequest request)
{
    wchar_t fileBuffer[MAX_PATH] = {};

    OPENFILENAMEW dialog{};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = nullptr;
    dialog.lpstrFile = fileBuffer;
    dialog.nMaxFile = MAX_PATH;

    dialog.lpstrFilter =
        L"All Files\0*.*\0"
        L"Text Files\0*.txt\0"
        L"Images\0*.png;*.jpg;*.jpeg\0";

    dialog.nFilterIndex = 1;

    dialog.Flags =
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST;

    APIResponse response;

    if (GetOpenFileNameW(&dialog))
    {
        response.status = true;
        response.result = std::filesystem::path(fileBuffer).string();
        return response;
    }

    response.status = false;
    response.error = GetLastError();
    return response;
}
