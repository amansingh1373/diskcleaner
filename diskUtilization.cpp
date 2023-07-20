#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "includes/diskutilization.h"

int main() {
    std::vector<std::string> drives = GetLogicalDriveNames();

    for (const auto& drive : drives) {
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes;
        if (GetDriveSpaceInfo(drive, freeBytesAvailable, totalNumberOfBytes)) {
            __int64 totalSize = GetDirectorySize(drive);

            std::map<std::string, __int64> fileCategories;
            fileCategories["Videos"] = 0;
            fileCategories["Images"] = 0;
            fileCategories["Other"] = 0;

            GetDirectorySizeAndCategorize(drive, fileCategories);

            std::cout << "Drive: " << drive << std::endl;
            std::cout << "Free Space: " << freeBytesAvailable.QuadPart << " bytes" << std::endl;
            std::cout << "Used Space: " << totalSize << " bytes" << std::endl;
            std::cout << "Total Size: " << totalNumberOfBytes.QuadPart << " bytes" << std::endl;

            std::cout << "Space Utilization Breakdown:" << std::endl;
            for (const auto& category : fileCategories) {
                std::cout << category.first << ": " << category.second << " bytes" << std::endl;
            }

            std::cout << "=================================" << std::endl;
        } else {
            std::cerr << "Failed to retrieve disk space information for drive: " << drive << std::endl;
        }
    }

    return 0;
}