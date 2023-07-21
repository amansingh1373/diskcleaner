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



std::vector<std::string> GetLogicalDriveNames() {
    std::vector<std::string> drives;
    char buffer[MAX_PATH] = "";
    GetLogicalDriveStrings(MAX_PATH, buffer);

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
    if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, nullptr)) {
        return true;
    }
    return false;
}

void GetDirectorySizeAndCategorize(const std::string& path, std::map<std::string, __int64>& fileCategories, std::map<std::string, long long> &fileCount) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
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
                        } else {
                            fileCategories["Other"] += (static_cast<__int64>(findFileData.nFileSizeLow) | (static_cast<__int64>(findFileData.nFileSizeHigh) << 32));
                            fileCount["Others"]++;
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

// std::string getFileType(const std::string& filePath) {
//     std::map<std::string, std::string> fileTypes = {
//         // Images
//         {".jpg", "Image"},
//         {".jpeg", "Image"},
//         {".png", "Image"},
//         {".gif", "Image"},
//         {".bmp", "Image"},
//         {".tiff", "Image"},
//         {".webp", "Image"},
//         {".svg", "Image"},

//         // Videos
//         {".mp4", "Video"},
//         {".avi", "Video"},
//         {".mkv", "Video"},
//         {".mov", "Video"},
//         {".wmv", "Video"},
//         {".flv", "Video"},
//         {".webm", "Video"},
//         {".m4v", "Video"},

//         // Documents
//         {".doc", "Document"},
//         {".docx", "Document"},
//         {".pdf", "Document"},
//         {".txt", "Document"},
//         {".rtf", "Document"},
//         {".odt", "Document"},
//         {".ppt", "Document"},
//         {".pptx", "Document"},
//         {".xls", "Document"},
//         {".xlsx", "Document"},

//         // Audio
//         {".mp3", "Audio"},
//         {".wav", "Audio"},
//         {".ogg", "Audio"},
//         {".flac", "Audio"},
//         {".aac", "Audio"},
//         {".wma", "Audio"},

//         // Compressed Archives
//         {".zip", "Archive"},
//         {".rar", "Archive"},
//         {".7z", "Archive"},
//         {".tar", "Archive"},
//         {".gz", "Archive"},
//         {".bz2", "Archive"},

//         // Executable
//         {".exe", "Executable"},

//         // Add more extensions and their corresponding types as needed
//     };

//     size_t dotIndex = filePath.find_last_of(".");
//     if (dotIndex != std::string::npos) {
//         std::string ext = filePath.substr(dotIndex);
//         auto it = fileTypes.find(ext);
//         if (it != fileTypes.end()) {
//             return it->second;
//         }
//     }

//     return "Other";
// }

// bool isRegularFiles(const char* path) {
//     struct stat path_stat;
//     if (stat(path, &path_stat) == 0) {
//         return S_ISREG(path_stat.st_mode);
//     }
//     return false;
// }

// std::string getFileExtensions(const std::string& filename) {
//     size_t pos = filename.find_last_of('.');
//     if (pos != std::string::npos) {
//         return filename.substr(pos);
//     }
//     return "";
// }

// long long getFileSizes(const char* path) {
//     struct stat path_stat;
//     if (stat(path, &path_stat) == 0) {
//         return path_stat.st_size;
//     }
//     return -1;
// }

// void folderBifurcation(const char* basePath) {
//     DIR* dir = opendir(basePath);
//     std::unordered_map<std::string,int> nof;
//     std::unordered_map<std::string,int> sof;

//     if (!dir) {
//         return;
//     }

//     struct dirent* entry;
//     while ((entry = readdir(dir)) != NULL) {
//         std::string path = std::string(basePath) + "/" + std::string(entry->d_name);

//         if (isRegularFiles(path.c_str())) {
//             std::string fileExtension = getFileExtensions(std::string(entry->d_name));
//             if (!fileExtension.empty()) {
//                 std::string fileType = getFileType(path);
//                 long long fileSize = getFileSize(path.c_str());
//                 nof[fileType]++;
//                 sof[fileType] += fileSize;
//             }
//         } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             folderBifurcation(path.c_str());
//         }
//     }

//     for(auto &x : nof){
//         std::cout<<x.first<<" "<<x.second<<std::endl;
//     }
//     std::cout<<"========================================="<<std::endl;
//     std::cout<<"size:"<<std::endl;
//     for(auto &x : sof){
//         std::cout<<x.first<<" "<<x.second<<std::endl;
//     }
//     closedir(dir);
// }

