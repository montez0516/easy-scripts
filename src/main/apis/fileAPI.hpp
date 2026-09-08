#ifndef FILE_API_H
#define FILE_API_H

#include "../../api/register.hpp"

class FileAPI
{
public:
    FileAPI(APIRegister &apiRegister);

    APIResponse openFilePicker(APIRequest request);
};

#endif