#include <iostream>
#include <string>
#include <fstream>
#include <openssl/sha.h>

std::string calculateFileHash(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open the file: " << filePath << std::endl;
        return "";
    }

    const int bufferSize = 8192;
    char buffer[bufferSize];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    while (file.good()) {
        file.read(buffer, bufferSize);
        SHA256_Update(&sha256, buffer, file.gcount());
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    std::string result;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        result += hash[i];
    }

    return result;
}

int main() {
    std::string filePath;
    std::cout << "Enter the path of the file: ";
    std::getline(std::cin, filePath);

    std::string hashValue = calculateFileHash(filePath);
    if (!hashValue.empty()) {
        std::cout << "SHA-256 hash value of the file: " << hashValue << std::endl;
    }

    return 0;
}
