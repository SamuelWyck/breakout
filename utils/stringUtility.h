#ifndef MY_STRING_UTILS_H
#define MY_STRING_UTILS_H


#include <string_view>
#include <string>
#include <vector>


namespace StringUtils {
    std::vector<std::string> split(std::string_view str, char delim=' ');
};



#endif