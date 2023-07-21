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
#include <cstring>

namespace fs = std::filesystem;

std::unordered_map<std::string,int> nof;
std::unordered_map<std::string,int> sof;


// SHA-256 Constants
constexpr unsigned int kBlockSize = 512 / 8;
constexpr unsigned int kHashSize = 256 / 8;
constexpr unsigned int kNumRounds = 64;

const unsigned int kConstants[kNumRounds] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Right-rotate a 32-bit integer by a specified number of bits
inline unsigned int rotateRight(unsigned int value, unsigned int numBits) {
    return (value >> numBits) | (value << (32 - numBits));
}

// Perform SHA-256 transformation on a single 512-bit block
void sha256_transform(const char* block, unsigned int* state) {
    unsigned int words[kNumRounds];

    // Prepare the message schedule
    for (unsigned int i = 0; i < 16; ++i) {
        words[i] = block[i * 4] << 24;
        words[i] |= block[i * 4 + 1] << 16;
        words[i] |= block[i * 4 + 2] << 8;
        words[i] |= block[i * 4 + 3];
    }
    for (unsigned int i = 16; i < kNumRounds; ++i) {
        unsigned int s0 = rotateRight(words[i - 15], 7) ^ rotateRight(words[i - 15], 18) ^ (words[i - 15] >> 3);
        unsigned int s1 = rotateRight(words[i - 2], 17) ^ rotateRight(words[i - 2], 19) ^ (words[i - 2] >> 10);
        words[i] = words[i - 16] + s0 + words[i - 7] + s1;
    }

    // Initialize the working variables
    unsigned int a = state[0];
    unsigned int b = state[1];
    unsigned int c = state[2];
    unsigned int d = state[3];
    unsigned int e = state[4];
    unsigned int f = state[5];
    unsigned int g = state[6];
    unsigned int h = state[7];

    // Main loop
    for (unsigned int i = 0; i < kNumRounds; ++i) {
        unsigned int S1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
        unsigned int ch = (e & f) ^ (~e & g);
        unsigned int temp1 = h + S1 + ch + kConstants[i] + words[i];
        unsigned int S0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
        unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
        unsigned int temp2 = S0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    // Update the state with the transformed values
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

std::string compute_sha256(const fs::path& file_path) {
    std::ifstream file(file_path.string(), std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file: " + file_path.string());
    }

    // Initialize the state (hash values)
    unsigned int state[kHashSize / 4] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Prepare the block buffer
    char block[kBlockSize];
    memset(block, 0, kBlockSize);

    // Process the file block by block
    while (!file.eof()) {
        file.read(block, kBlockSize);
        sha256_transform(block, state);
    }

    // Convert the final hash state to a string
    std::ostringstream oss;
    for (unsigned int i = 0; i < kHashSize / 4; ++i) {
        oss << std::hex << std::setfill('0') << std::setw(8) << state[i];
    }

    return oss.str();
}

/*
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
*/

std::map<std::string, std::vector<fs::path>> find_duplicate_files(const fs::path& directory_path) {
    std::map<std::string, std::vector<fs::path>> hash_to_files_map;

    for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
        if (fs::is_regular_file(entry)) {
            const fs::path& file_path = entry.path();
            try {
                std::string hash = compute_sha256(file_path);
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

std::vector<std::string> filepaths;

std::vector<std::string> detectLargeFiles(const char* basePath, long long maxSize = 1024 * 1024 * 10) {
    
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

std::vector<std::string> filecollection;

std::vector<std::string> detectFileByExtension(const char* basePath, const std::string& targetExtension) {
    
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


std::string getFileType(const std::string& filePath) {
    std::map<std::string, std::string> fileTypes = {
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
    if (dotIndex != std::string::npos) {
        std::string ext = filePath.substr(dotIndex);
        auto it = fileTypes.find(ext);
        if (it != fileTypes.end()) {
            return it->second;
        }
    }

    return "Other";
}


void folderBifurcation(const char* basePath,bool flag) {
    DIR* dir = opendir(basePath);
    if (!dir) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string path = std::string(basePath) + "/" + std::string(entry->d_name);

        if (isRegularFile(path.c_str())) {
            std::string fileExtension = getFileExtension(std::string(entry->d_name));
            if (!fileExtension.empty()) {
                std::string fileType = getFileType(path);
                long long fileSize = getFileSize(path.c_str());
                nof[fileType]++;
                sof[fileType] += fileSize;
            }
        } else if (isDirectory(path.c_str()) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            folderBifurcation(path.c_str(),false);
        }
    }

    if(flag){
        std::cout<<"========================================="<<std::endl;
        std::cout<<"========================================="<<std::endl;
        std::cout<<"Number of files:"<<std::endl;
        for(auto &x : nof){
            std::cout<<">"<<x.first<<" "<<x.second<<std::endl;
        }
        std::cout<<"========================================="<<std::endl;
        std::cout<<"size of files:"<<std::endl;
        for(auto &x : sof){
            std::cout<<">"<<x.first<<" "<<x.second<<std::endl;
        }
        
        std::cout<<"========================================="<<std::endl;
        std::cout<<"========================================="<<std::endl<<std::endl;
    }
    closedir(dir);
}