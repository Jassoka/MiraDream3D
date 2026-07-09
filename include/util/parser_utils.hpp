//
// Created by jassoka on 7/9/26.
//

#ifndef MIRADREAM3D_PARSER_UTILS_HPP
#define MIRADREAM3D_PARSER_UTILS_HPP
#include <string>

enum MessageType
{
    WARNING,
    ERROR
};


inline std::string parserMessageFormat(const MessageType type, const std::string&objectName, const std::string &filePath, const std::string& msg, const int line, const int col)
{
    std::string messageTypeName;
    switch (type)
    {
        case WARNING:
            messageTypeName = "Warning";
            break;
        case ERROR:
            messageTypeName = "Error";
            break;
    }
    return filePath + '\n' +  messageTypeName + ": [" + objectName + " l." + std::to_string(line) +
                       " c." + std::to_string(col) + "] " + msg;
}

#endif //MIRADREAM3D_PARSER_UTILS_HPP
