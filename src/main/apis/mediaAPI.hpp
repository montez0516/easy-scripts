#ifndef MEDIA_API_H
#define MEDIA_API_H

#include "../../api/register.hpp"

class MediaAPI
{
public:
    MediaAPI(APIRegister &apiRegister);

    APIResponse message(APIRequest request);
    APIResponse input(APIRequest request);
};

#endif