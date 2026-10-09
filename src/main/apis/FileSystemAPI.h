#pragma once

#include "../../api/Register.h"

class FileAPI
{
public:
    FileAPI(APIRegister &apiRegister);

    APIResponse openFilePicker(APIRequest request);
};
