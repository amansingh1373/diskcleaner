#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>


std::vector<std::string> videoExtensions = { 
    ".mp4", 
    ".avi", 
    ".mkv", 
    ".mov",
    ".wmv",
    ".flv",
    ".webm",
    ".mpeg",
    ".mpg",
    ".3gp",
    ".swf",
    ".vob",
    ".rm",
    ".rmvb",
    ".ts",
    ".m4v",
    ".m2ts",
    ".divx",
    ".ogv",
    ".ogm",
    ".asf",
    ".qt",
    ".mxf",
    ".dav"
};

std::vector<std::string> imageExtensions = { 
    ".jpg", 
    ".jpeg", 
    ".png", 
    ".gif",
    ".bmp",
    ".tiff",
    ".webp",
    ".svg",
    ".ico",
    ".tif",
    ".jfif",
    ".jpe",
    ".jif",
    ".jfi",
    ".jp2",
    ".j2k",
    ".jpf",
    ".jpx",
    ".jpm",
    ".djvu",
    ".djv",
    ".pnm",
    ".ppm",
    ".pgm",
    ".pbm",
    ".hdr",
    ".exr",
    ".raw",
    ".cr2",
    ".nef",
    ".orf",
    ".arw",
    ".dng"
};

std::vector<std::string> documentExtensions = {
    ".doc", 
    ".docx", 
    ".ppt", 
    ".pptx", 
    ".xls", 
    ".xlsx", 
    ".pdf", 
    ".txt", 
    ".rtf", 
    ".odt"
};

std::vector<std::string> archiveExtensions = {
    ".zip", 
    ".rar", 
    ".7z", 
    ".tar", 
    ".gz", 
    ".bz2", 
    ".xz", 
    ".iso"
};

std::vector<std::string> audioExtensions = {
    ".mp3", 
    ".wav", 
    ".ogg", 
    ".flac", 
    ".aac", 
    ".wma", 
    ".m4a"
};

std::vector<std::string> executableExtensions = {
    ".exe", 
    ".msi", 
    ".bat", 
    ".sh"
};

std::vector<std::string> GetLogicalDriveNames() {
    std::vector<std::string> drives;
    char buffer[MAX_PATH] = "";
    GetLogicalDriveStrings(MAX_PATH, buffer); //windows.h

    char* drive = buffer;
    std::cout<<"Drive Available:"<<std::endl;
    while (*drive) {
        std::cout<<drive<<std::endl;
        drives.push_back(drive);
        drive += strlen(drive) + 1;
    }
    std::cout<<std::endl;
    return drives;
}

bool GetDriveSpaceInfo(const std::string& drive, ULARGE_INTEGER& freeBytesAvailable, ULARGE_INTEGER& totalNumberOfBytes) {
    if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, nullptr)) { //windows.h
        return true;
    }
    return false;
}

void GetDirectorySizeAndCategorize(const std::string& path, std::map<std::string, __int64>& fileCategories, std::map<std::string, long long> &fileCount) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData); //windows.h

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {  //windows.h (macro)
                    GetDirectorySizeAndCategorize(path + "\\" + findFileData.cFileName, fileCategories,fileCount);
                } else {
                    // Categorize the file based on its extension
                    std::string extension = findFileData.cFileName;
                    size_t dotIndex = extension.find_last_of(".");
                    if (dotIndex != std::string::npos) {
                        extension = extension.substr(dotIndex);
                        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
                        if (std::find(videoExtensions.begin(), videoExtensions.end(), extension) != videoExtensions.end()) {
                            fileCategories["Videos"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Videos"]++;
                        } else if (std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end()) {
                            fileCategories["Images"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Images"]++;
                        } else if (std::find(documentExtensions.begin(), documentExtensions.end(), extension) != documentExtensions.end()) {
                            fileCategories["Documents"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Documents"]++;
                        } else if (std::find(archiveExtensions.begin(), archiveExtensions.end(), extension) != archiveExtensions.end()) {
                            fileCategories["Archives"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Archives"]++;
                        } else if (std::find(audioExtensions.begin(), audioExtensions.end(), extension) != audioExtensions.end()) {
                            fileCategories["Audios"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Audios"]++;
                         } else if (std::find(executableExtensions.begin(), executableExtensions.end(), extension) != executableExtensions.end()) {
                            fileCategories["Executables"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Executables"]++;
                        } else {
                            fileCategories["Others"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Others"]++;
                        }
                    }
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0); //windows.h

        FindClose(hFind);
    }
}

__int64 GetDirectorySize(const std::string& path) {
    __int64 size = 0;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData); //windows.h

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    size += GetDirectorySize(path + "\\" + findFileData.cFileName); //recursive
                } else {
                    size += static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32);
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind); //windows.h
    }

    return size;
}