#pragma once

#include "../../api/Register.h"

class MediaAPI
{
public:
    MediaAPI(APIRegister &apiRegister);

    APIResponse message(APIRequest request);
    APIResponse input(APIRequest request);
};
