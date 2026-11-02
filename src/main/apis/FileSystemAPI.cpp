#include "FileSystemAPI.h"
#include "../../api/Register.h"
#include "../../core/utils/Conversion.h"
#include "../../core/utils/windows/WindowsUtils.h"

#include <QFileDialog>
#include <QString>
#include <QThread>
#include <QApplication>

FileAPI::FileAPI(APIRegister &apiRegister)
{
    apiRegister.registerMethod(
        "files.filepicker",
        [this](const APIRequest &request)
        { return openFilePicker(request); },
        APIThread::GUI);
    apiRegister.registerMethod(
        "files.directorypicker",
        [this](const APIRequest &request)
        { return openDirectoryPicker(request); },
        APIThread::GUI);
}

APIResponse FileAPI::openFilePicker(const APIRequest &request)
{
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        "Select a File",
        QString(),
        "All Files (*.*);;Text Files (*.txt);;Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
        return APIResponse::success({{"cancelled", true},
                                     {"path", nullptr}});

    return APIResponse::success({{"cancelled", false},
                                 {"path", filePath.toUtf8().toStdString()}});
}

APIResponse FileAPI::openDirectoryPicker(const APIRequest &request)
{
    std::string existingDir = request.params.value<std::string>("existingDir", "");

    QString directoryPath = QFileDialog::getExistingDirectory(nullptr, "Select a Directory", QString::fromStdString(existingDir));

    if (directoryPath.isEmpty())
        return APIResponse::success({{"path", nullptr}});

    return APIResponse::success({{"path", directoryPath.toUtf8().toStdString()}});
}