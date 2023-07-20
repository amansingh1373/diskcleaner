#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

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

void delete_duplicates(const std::map<std::size_t, std::vector<fs::path>>& duplicate_files) {
    for (const auto& entry : duplicate_files) {
        if (entry.second.size() > 1) {
            // Keep the first file, delete others
            for (size_t i = 1; i < entry.second.size(); ++i) {
                try {
                    fs::remove(entry.second[i]);
                    std::cout << "Deleted: " << entry.second[i].string() << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error deleting file: " << entry.second[i].string() << " - " << e.what() << std::endl;
                }
            }
        }
    }
}

int main() {
    static const fs::path directory_path = "./testDuplicate"; // Set your desired directory path here

    try {
        std::map<std::size_t, std::vector<fs::path>> duplicate_files = find_duplicate_files(directory_path);

        std::cout << "Duplicate files:" << std::endl;
        for (const auto& entry : duplicate_files) {
            if (entry.second.size() > 1) {
                std::cout << "Hash: " << entry.first << std::endl;
                for (const auto& file_path : entry.second) {
                    std::cout << "  - " << file_path.string() << std::endl;
                }
            }
        }
        int x;
        std::cout<<"Enter 1 to delete these files";
        std::cin>>x;
        if(x){
        delete_duplicates(duplicate_files);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}