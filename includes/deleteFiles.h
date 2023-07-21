#include <filesystem>
#include <vector>

bool deleteFiles(const std::vector<std::string>& filePaths) {
    bool allFilesDeleted = true;
    
    for (const std::string& filePath : filePaths) {
        if (std::filesystem::exists(filePath)) {
            try {
                std::filesystem::remove(filePath);
                std::cout << "Deleted file: " << filePath << std::endl;
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error deleting file: " << filePath << " - " << e.what() << std::endl;
                allFilesDeleted = false;
            }
        } else {
            std::cerr << "File not found: " << filePath << std::endl;
            allFilesDeleted = false;
        }
    }
    
    return allFilesDeleted;
}

bool deleteDuplicateFiles(const std::vector<std::string>& filePaths) {
    bool allFilesDeleted = true;
    long long size = filePaths.size();
    for (int i = 1;i < size;i++) {
        if (std::filesystem::exists(filePaths[i])) {
            try {
                std::filesystem::remove(filePaths[i]);
                std::cout << "Deleted file: " << filePaths[i] << std::endl;
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error deleting file: " << filePaths[i] << " - " << e.what() << std::endl;
                allFilesDeleted = false;
            }
        } else {
            std::cerr << "File not found: " << filePaths[i] << std::endl;
            allFilesDeleted = false;
        }
    }
    
    return allFilesDeleted;
}
