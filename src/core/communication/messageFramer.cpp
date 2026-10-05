#include "messageFramer.hpp"

#include <string>
#include <vector>

std::vector<std::string> MessageFramer::push(std::string_view message)
{
    buffer_.append(message);

    std::vector<std::string> messages;

    size_t pos;

    while ((pos = buffer_.find('\n')) != std::string::npos)
    {
        messages.push_back(buffer_.substr(0, pos));
        buffer_.erase(0, pos + 1);
    }

    return messages;
}