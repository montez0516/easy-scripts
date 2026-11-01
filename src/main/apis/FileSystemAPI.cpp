#include "FileSystemAPI.h"
#include "../../api/Register.h"
#include "../../core/utils/Conversion.h"
#include "../../core/utils/windows/WindowsUtils.h"

#include <QFileDialog>
#include <QString>
#include <QWidget>

FileAPI::FileAPI(APIRegister &apiRegister)
{
    apiRegister.registerMethod("files.filepicker", [this](const APIRequest &request)
                               { return openFilePicker(request); });
}

APIResponse FileAPI::openFilePicker(const APIRequest &request)
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

    if (GetOpenFileNameW(&dialog))
    {
        return APIResponse::success({{"path", Utils::Conversion::toString(std::filesystem::path(fileBuffer).wstring())}});
    }

    DWORD error = CommDlgExtendedError();

    if (error == 0)
        return APIResponse::success({{"path", nullptr},
                                     {"cancelled", true}});

    return APIResponse::failure("File picker failed with code " + Utils::Windows::GetErrorFromCode(error));
}
