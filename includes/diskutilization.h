#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


std::vector<std::string> videoExtensions = { ".mp4", ".avi", ".mkv", ".mov" };
std::vector<std::string> imageExtensions = { ".jpg", ".jpeg", ".png", ".gif" };


std::vector<std::string> GetLogicalDriveNames() {
    std::vector<std::string> drives;
    char buffer[MAX_PATH] = "";
    GetLogicalDriveStrings(MAX_PATH, buffer);

    char* drive = buffer;
    while (*drive) {
        drives.push_back(drive);
        drive += strlen(drive) + 1;
    }

    return drives;
}

bool GetDriveSpaceInfo(const std::string& drive, ULARGE_INTEGER& freeBytesAvailable, ULARGE_INTEGER& totalNumberOfBytes) {
    if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, nullptr)) {
        return true;
    }
    return false;
}

void GetDirectorySizeAndCategorize(const std::string& path, std::map<std::string, __int64>& fileCategories) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    GetDirectorySizeAndCategorize(path + "\\" + findFileData.cFileName, fileCategories);
                } else {
                    // Categorize the file based on its extension
                    std::string extension = findFileData.cFileName;
                    size_t dotIndex = extension.find_last_of(".");
                    if (dotIndex != std::string::npos) {
                        extension = extension.substr(dotIndex);
                        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
                        if (std::find(videoExtensions.begin(), videoExtensions.end(), extension) != videoExtensions.end()) {
                            fileCategories["Videos"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                        } else if (std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end()) {
                            fileCategories["Images"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                        } else {
                            fileCategories["Other"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                        }
                    }
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
}

__int64 GetDirectorySize(const std::string& path) {
    __int64 size = 0;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    size += GetDirectorySize(path + "\\" + findFileData.cFileName);
                } else {
                    size += static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32);
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    return size;
}

