#include "mediaAPI.hpp"
#include "../../api/register.hpp"

#include <iostream>

MediaAPI::MediaAPI(APIRegister &apiRegister)
{
    apiRegister.registerMethod("media.message", [this](APIRequest request)
                               { return message(request); });
    apiRegister.registerMethod("media.input", [this](APIRequest request)
                               { return input(request); });
}

APIResponse MediaAPI::message(APIRequest request)
{
    APIResponse response;
    response.status = true;
    std::cout << request.params << std::endl;
    return response;
}

APIResponse MediaAPI::input(APIRequest request)
{
    APIResponse response;
    response.status = true;

    std::cout << request.params << std::endl;

    std::string input;

    std::getline(std::cin, input);

    response.result = input;

    return response;
}