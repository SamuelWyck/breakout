#include <string_view>
#include <string>
#include <vector>
#include "./stringUtility.h"



namespace StringUtils {
    std::vector<std::string> split(std::string_view str, char delim) {
        std::vector<std::string> splitString{};

        auto tailPtr {str.begin()};
        auto leadPtr {str.begin()};

        while (leadPtr != str.end()) {
            if (*leadPtr != delim) {
                leadPtr += 1;

            } else if (*leadPtr == delim) {
                splitString.push_back(std::string{tailPtr, leadPtr});
                leadPtr += 1;
                tailPtr = leadPtr;
            }
        }

        if (tailPtr != leadPtr) {
            splitString.push_back(std::string{tailPtr, leadPtr});
        }

        return splitString;
    };
};