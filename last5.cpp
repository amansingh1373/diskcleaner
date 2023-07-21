#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <cstdio>
using namespace std;

int imageCount = 0;
int videoCount = 0;
int fileCount = 0;
int folderCount = 0;
vector<string> largeFiles;

string getFileExtension(const string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos != string::npos) {
        return filename.substr(pos);
    }
    return "";
}

bool isRegularFile(const char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        return S_ISREG(path_stat.st_mode);
    }
    return false;
}

bool isDirectory(const char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        return S_ISDIR(path_stat.st_mode);
    }
    return false;
}

bool isImageFile(const string& fileExtension) {
    vector<string> imageExtensions = {".jpg", ".jpeg", ".png", ".gif", ".bmp"};
    for (const string& ext : imageExtensions) {
        if (strcasecmp(fileExtension.c_str(), ext.c_str()) == 0) {
            imageCount++;
            return true;
        }
    }
    return false;
}

bool isVideoFile(const string& fileExtension) {
    vector<string> videoExtensions = {".mp4", ".avi", ".mkv", ".mov", ".wmv"};
    for (const string& ext : videoExtensions) {
        if (strcasecmp(fileExtension.c_str(), ext.c_str()) == 0) {
            videoCount++;
            return true;
        }
    }
    return false;
}

long long getFileSize(const char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        return path_stat.st_size;
    }
    return -1;
}

string getFileType(const string& filePath) {
    map<string, string> fileTypes = {
        // Images
        {".jpg", "Image"},
        {".jpeg", "Image"},
        {".png", "Image"},
        {".gif", "Image"},
        {".bmp", "Image"},
        {".tiff", "Image"},
        {".webp", "Image"},
        {".svg", "Image"},

        // Videos
        {".mp4", "Video"},
        {".avi", "Video"},
        {".mkv", "Video"},
        {".mov", "Video"},
        {".wmv", "Video"},
        {".flv", "Video"},
        {".webm", "Video"},
        {".m4v", "Video"},

        // Documents
        {".doc", "Document"},
        {".docx", "Document"},
        {".pdf", "Document"},
        {".txt", "Document"},
        {".rtf", "Document"},
        {".odt", "Document"},
        {".ppt", "Document"},
        {".pptx", "Document"},
        {".xls", "Document"},
        {".xlsx", "Document"},

        // Audio
        {".mp3", "Audio"},
        {".wav", "Audio"},
        {".ogg", "Audio"},
        {".flac", "Audio"},
        {".aac", "Audio"},
        {".wma", "Audio"},

        // Compressed Archives
        {".zip", "Archive"},
        {".rar", "Archive"},
        {".7z", "Archive"},
        {".tar", "Archive"},
        {".gz", "Archive"},
        {".bz2", "Archive"},

        // Executable
        {".exe", "Executable"},

        // Add more extensions and their corresponding types as needed
    };

    size_t dotIndex = filePath.find_last_of(".");
    if (dotIndex != string::npos) {
        string ext = filePath.substr(dotIndex);
        auto it = fileTypes.find(ext);
        if (it != fileTypes.end()) {
            return it->second;
        }
    }

    return "Other";
}

void segregateFilesByExtension(const char* basePath) {
    DIR* dir = opendir(basePath);

    if (!dir) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string path = string(basePath) + "/" + string(entry->d_name);

        if (isRegularFile(path.c_str())) {
            fileCount++;
            string fileExtension = getFileExtension(string(entry->d_name));
            if (!fileExtension.empty()) {
                string fileType = getFileType(path);
                long long fileSize = getFileSize(path.c_str());
                cout << "File: " << path<< "\n\t" << " Type: " << fileType << " Size: " << fileSize << endl;
            }
        } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            folderCount++;
            cout << "Directory: " << path << endl;
            segregateFilesByExtension(path.c_str());
        }
    }

    closedir(dir);
}

void deleteFilesByExtension(const char* basePath, const string& targetExtension) {
    DIR* dir = opendir(basePath);

    if (!dir) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string path = string(basePath) + "/" + string(entry->d_name);

        if (isRegularFile(path.c_str())) {
            string fileExtension = getFileExtension(string(entry->d_name));
            if (strcasecmp(fileExtension.c_str(), targetExtension.c_str()) == 0) {
                if (remove(path.c_str()) == 0) {
                    cout << "Deleted: " << path << endl;
                } else {
                    cout << "Failed to delete: " << path << endl;
                }
            }
        } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            deleteFilesByExtension(path.c_str(), targetExtension);
        }
    }
    closedir(dir);
}

void deleteFilesLargerThanSize(const char* basePath, long long maxSize) {
    DIR* dir = opendir(basePath);

    if (!dir) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string path = string(basePath) + "/" + string(entry->d_name);

        if (isRegularFile(path.c_str())) {
            long long fileSize = getFileSize(path.c_str());
            if (fileSize > maxSize) {
                if (remove(path.c_str()) == 0) {
                    cout << "Deleted: " << path << " (Size: " << fileSize << " bytes)" << endl;
                } else {
                    cout << "Failed to delete: " << path << " (Size: " << fileSize << " bytes)" << endl;
                }
            }
        } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            deleteFilesLargerThanSize(path.c_str(), maxSize);
        }
    }

    closedir(dir);
}

int main(void) {
    const char* basePath = "C:\\Users\\singh\\Desktop\\tally code brewers\\testDuplicate"; // Base directory path
    segregateFilesByExtension(basePath);

    cout << "Image Count: " << imageCount << endl;
    cout << "Video Count: " << videoCount << endl;
    cout << "File Count: " << fileCount-imageCount-videoCount << endl;
    cout << "Folder Count: " << folderCount << endl;

    deleteFilesByExtension(basePath,".mp4");

    long long maxSize = 1024 * 1024 * 10; // Max size in bytes (e.g., 1 MB)
    deleteFilesLargerThanSize(basePath, maxSize);

    for(auto &x : largeFiles) cout<<x<<",";
    return 0;
}