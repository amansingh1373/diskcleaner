#include <iostream>
#include <string>
#include <unordered_map>
#include "includes/diskutilization.h"
#include "includes/detectFiles.h"

void spaceAvail(){
    std::vector<std::string> drives = GetLogicalDriveNames();

    for (const auto& drive : drives) {
        std::string d = drive;
        LPTSTR long_string = new TCHAR[d.size() + 1];
        strcpy(long_string, d.c_str());
        std::replace(d.begin(), d.end(), '\\', '/');
        
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes,totalNumberOfFreeBytes;
        if (GetDiskFreeSpaceEx(long_string, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        //Calculate the percentage of free space on the drive
        double percentageFree = (static_cast<double>(freeBytesAvailable.QuadPart) / totalNumberOfBytes.QuadPart) * 100.0;

        std::cout << "Percentage of free space on drive: " << percentageFree << "%" << std::endl;
        } else {
        std::cerr << "Failed to retrieve disk space information." << std::endl;
        }
        std::cout << "=================================" << std::endl;
    }
}

void categorize(){
    std::vector<std::string> drives = GetLogicalDriveNames();

    for (const auto& drive : drives) {
        std::map<std::string, __int64> fileCategories;
        fileCategories["Videos"] = 0;
        fileCategories["Images"] = 0;
        fileCategories["Other"] = 0;

        GetDirectorySizeAndCategorize(drive, fileCategories);

        std::cout << "Drive: " << drive << std::endl;
        std::cout << "Space Utilization Breakdown:" << std::endl;
        for (const auto& category : fileCategories) {
            std::cout << category.first << ": " << category.second << " bytes" << std::endl;
        }

        std::cout << "=================================" << std::endl;
    }
}

std::vector<std::vector<std::string>> detectDuplicateFolder(const fs::path directory_path){
    std::vector<std::vector<std::string>> finalResult;
    try {
        std::map<std::size_t, std::vector<fs::path>> duplicate_files = find_duplicate_files(directory_path);

        std::cout << "Duplicate files:" << std::endl;
        for (const auto& entry : duplicate_files) {
            if (entry.second.size() > 1) {
                std::cout << "Hash: " << entry.first << std::endl;
                std::vector<std::string> temp;
                for (const auto& file_path : entry.second) {
                    std::cout << "  - " << file_path.string() << std::endl;
                    temp.push_back(file_path.string());
                }
                finalResult.push_back(temp);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return {{}};
    }
    return finalResult;
}

int main(int argc, char* argv[]){
    
    std::string cmd = argv[1];

    std::unordered_map<std::string,int> mp = {
        {"space-avail",0},
        {"categorize",1},
        {"detect-duplicate-folder",2},
        {"detect-large-files",3},
        {"detect-same-ext-file",4}
    };

    int val = 0;
    switch(mp[cmd]){
        case 0: spaceAvail();
                break;
        case 1: categorize();
                break;
        case 2: try{
                    const char* filepath = argv[2];
                    std::vector<std::vector<std::string>> result = detectDuplicateFolder(filepath);
                    for(auto &x : result){
                        for(auto &y : x){
                            std::cout<<y<<"   |   ";
                        }
                        std::cout<<std::endl;
                    }
                }catch(...){
                    std::cerr<<"please provide a path of the folder.";
                }
                /*check if the given path is of a folder or not*/

                break;
        case 3: try{
                    const char* dirpath= argv[2];
                    std::vector<std::string> result;
                    try{
                        long long maxsize = atoi(argv[3]) * 1024 * 1024;
                        result = detectLargeFiles(dirpath,maxsize);
                        
                    }catch(...){
                        result = detectLargeFiles(dirpath);
                    }
                    for(auto &x : result){
                        std::cout<<x<<std::endl;
                    }
                }catch(...){
                    std::cerr<<"please provide a path of the folder.";
                }
                break;
        case 4: try{
                    const char* dirpath= argv[2];
                    std::vector<std::string> result;
                    try{
                        std::string ext = argv[3];
                        result = detectFileByExtension(dirpath,ext);
                        
                    }catch(...){
                        std::cerr<<"please provide a extension(eg: .cpp, .json)";
                    }
                    for(auto &x : result){
                        std::cout<<x<<std::endl;
                    }
                }catch(...){
                    std::cerr<<"please provide a path of the folder.";
                }
                break;
        default:std::cout<<"default";
    }

    return 0;
}