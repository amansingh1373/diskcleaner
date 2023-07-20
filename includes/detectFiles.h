#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>

namespace fs = std::filesystem;

std::size_t compute_hash(const fs::path& file_path) {
    std::ifstream file(file_path.string(), std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file: " + file_path.string());
    }

    // Compute the hash based on the sum of byte values in the file
    std::size_t hash = 0;
    char byte;
    while (file.get(byte)) {
        hash += static_cast<unsigned char>(byte);
    }

    return hash;
}

std::map<std::size_t, std::vector<fs::path>> find_duplicate_files(const fs::path& directory_path) {
    std::map<std::size_t, std::vector<fs::path>> hash_to_files_map;

    for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
        if (fs::is_regular_file(entry)) {
            const fs::path& file_path = entry.path();
            try {
                std::size_t hash = compute_hash(file_path);
                hash_to_files_map[hash].push_back(file_path);
            } catch (const std::exception& e) {
                std::cerr << "Error processing file: " << file_path << " - " << e.what() << std::endl;
            }
        }
    }

    return hash_to_files_map;
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

long long getFileSize(const char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) == 0) {
        return path_stat.st_size;
    }
    return -1;
}

std::string getFileExtension(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos) {
        return filename.substr(pos);
    }
    return "";
}

std::vector<std::string> detectLargeFiles(const char* basePath, long long maxSize = 1024 * 1024 * 10) {
    std::vector<std::string> filepaths;
    DIR* dir = opendir(basePath);

    if (!dir) {
        return {};
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string path = std::string(basePath) + "/" + std::string(entry->d_name);

        if (isRegularFile(path.c_str())) {
            long long fileSize = getFileSize(path.c_str());
            if (fileSize > maxSize) {
                filepaths.push_back(path);
            }
        } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            detectLargeFiles(path.c_str(), maxSize);
        }
    }

    closedir(dir);
    return filepaths;
}

std::vector<std::string> detectFileByExtension(const char* basePath, const std::string& targetExtension) {
    std::vector<std::string> filecollection;
    DIR* dir = opendir(basePath);

    if (!dir) {
        return {};
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string path = std::string(basePath) + "/" + std::string(entry->d_name);

        if (isRegularFile(path.c_str())) {
            std::string fileExtension = getFileExtension(std::string(entry->d_name));
            if (strcasecmp(fileExtension.c_str(), targetExtension.c_str()) == 0) {
                filecollection.push_back(path.c_str());
            }
        } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            detectFileByExtension(path.c_str(), targetExtension);
        }
    }
    closedir(dir);
    return filecollection;
}