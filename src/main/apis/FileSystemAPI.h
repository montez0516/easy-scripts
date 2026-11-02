#pragma once

#include "../../api/Register.h"

class FileAPI
{
public:
    FileAPI(APIRegister &apiRegister);

    APIResponse openFilePicker(const APIRequest &request);
    APIResponse openDirectoryPicker(const APIRequest &request);
};
