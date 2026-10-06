#pragma once

#include "../../api/register.hpp"

class MediaAPI
{
public:
    MediaAPI(APIRegister &apiRegister);

    APIResponse message(APIRequest request);
    APIResponse input(APIRequest request);
};
