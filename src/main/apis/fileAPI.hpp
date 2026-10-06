#pragma once

#include "../../api/register.hpp"

class FileAPI
{
public:
    FileAPI(APIRegister &apiRegister);

    APIResponse openFilePicker(APIRequest request);
};
