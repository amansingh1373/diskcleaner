#include <iostream>
#include <string>
#include <unordered_map>
#include "includes/diskutilization.h"
#include "includes/detectFiles.h"
#include "includes/deleteFiles.h"

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
            std::cout << drive<< std::endl;
            std::cout << "Percentage of free space on drive: " << percentageFree << "%" << std::endl;
        } else {
            std::cerr << "Failed to retrieve disk space information." << std::endl;
        }
        std::cout << "======================================================" << std::endl;
    }
}

void categorize(){
    std::vector<std::string> drives = GetLogicalDriveNames();

    for (const auto& drive : drives) {
        std::map<std::string, __int64> fileCategories;
        std::map<std::string, long long> fileCount{{"Videos",0},{"Images",0},{"Others",0}};
        fileCategories["Videos"] = 0;
        fileCategories["Images"] = 0;
        fileCategories["Other"] = 0;

        GetDirectorySizeAndCategorize(drive, fileCategories,fileCount);

        std::cout << "Drive: " << drive << std::endl;
        std::cout << "Space Utilization Breakdown:" << std::endl;
        for (const auto& category : fileCategories) {
            std::cout << category.first << " size: " << category.second << " bytes" << std::endl;
        }
        for (const auto& category : fileCount) {
            std::cout << category.first << " count: " << category.second << " bytes" << std::endl;
        }
        std::cout << "=================================================================" << std::endl;
    }
}

std::vector<std::vector<std::string>> detectDuplicateFolder(const fs::path directory_path){
    std::vector<std::vector<std::string>> finalResult;
    try {
        std::map<std::size_t, std::vector<fs::path>> duplicate_files = find_duplicate_files(directory_path);

        std::cout << "Duplicate files:" << std::endl;
        for (const auto& entry : duplicate_files) {
            if (entry.second.size() > 1) {
                std::vector<std::string> temp;
                for (const auto& file_path : entry.second) {
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

void seperatevalues(std::string str,std::vector<std::string> &vec){
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, ',')) {
        vec.push_back(token);
    }
}

void askAndDelete(char opt,std::vector<std::string> result,std::unordered_map<int,std::string> paths){
    opt = tolower(opt);
    bool flag = false;
    switch(opt){
        case 'a':   flag = deleteFiles(result);
                    if(!flag){
                        std::cout<<"There is some error.\n";
                    }else{
                        std::cout<<"File succesfully deleted\n";
                    }
                    break;
        case 'c':   {
                        std::cout<<"Provide me with numbers associated to the file path(eg:{1}.C:/Users/) (input: 1,10,15)\n";
                        std::string dfn = ""; //delete filepath number
                        std::cin>>dfn;
                        std::vector<std::string> ftbd; //files to be deleted
                        seperatevalues(dfn,ftbd);
                        /*verify that the input is correct*/
                        std::vector<std::string> newresult;
                        for(auto &x : ftbd){
                            int val = stoi(x);
                            newresult.push_back(paths[val]);
                        }
                        flag = deleteFiles(newresult);
                        if(!flag){
                            std::cout<<"There is some error.\n";
                        }else{
                            std::cout<<"File succesfully deleted\n";
                        }
                    }
                    break;
        default:    std::cerr<<"the input given is not appropriate.\n";
    }
}

int main(int argc, char* argv[]){
    
    std::string cmd = argv[1];

    std::unordered_map<std::string,int> mp = {
        {"space-avail",0},
        {"categorize",1},
        {"detect-duplicate",2},
        {"delete-duplicate",7},
        {"detect-large-files",3},
        {"delete-large-files",6},
        {"detect-same-ext-file",4},
        {"delete-same-ext-file",5},
        {"folder-breakdown",8}
    };

    int val = 0;
    switch(mp[cmd]){
        case 0: spaceAvail();
                break;
        case 1: categorize();
                break;
        case 2: {
                    try{
                        const char* filepath = argv[2];
                        std::vector<std::vector<std::string>> result = detectDuplicateFolder(filepath);
                        std::unordered_map<int,std::unordered_map<int,std::string>> whichduplicate;
                        int i = 1,j = 1;
                        for(auto &x : result){
                            std::cout<<"===============================     "<<i<<"     =======================================\n";
                            for(auto &y : x){
                                std::cout<<j<<"."<<y<<std::endl;
                                whichduplicate[i][j] = y;
                                j++;
                            }
                            i++;
                            std::cout<<std::endl<<"=============================================================================="<<std::endl;
                        }
                        std::cout<<"Do you want to delete duplicate files(Y/N):";
                        char ch = 'N';
                        std::cin>>ch;
                        ch = toupper(ch);
                        if(ch == 'Y'){
                            std::cout<<"Do you want to delete all the duplicates or selected duplicates(a/c):\n";
                            char opt1 = 'a';
                            std::cin>>opt1;
                            opt1 = tolower(opt1);
                            switch(opt1){
                                case 'a':   {
                                                bool flag = true;
                                                std::cout<<"\n*Caution:this will delete all the duplicate files shown and leave a random file of each duplicate group*\n";
                                                std::cout<<"Do you want to continue(Y/N):\n";
                                                char ch = 'Y';
                                                std::cin>>ch;
                                                if(ch == 'Y'){
                                                    for(auto &x : result){
                                                        flag = flag && deleteDuplicateFiles(x);
                                                    }
                                                    if(!flag){
                                                        std::cout<<"There is some error.\n";
                                                    }else{
                                                        std::cout<<"File succesfully deleted\n";
                                                    }
                                                }
                                            }
                                            break;
                                case 'c':   {
                                                std::cout<<"enter the number associated with the duplicate file bunch:";
                                                std::string res = "";
                                                std::vector<std::string> duplis_no;
                                                std::cin>>res;
                                                seperatevalues(res,duplis_no);
                                                std::vector<std::string> newresult;
                                                for(auto &x : duplis_no){
                                                    std::cout<<"===============================     "<<x<<"     =======================================\n";
                                                    std::cout<<"Provide me with numbers associated to the file path(eg:{1}.C:/Users/) (input: 1,10,15)\n";
                                                    std::string dfn = ""; //delete filepath number
                                                    std::cin>>dfn;
                                                    std::vector<std::string> temp;
                                                    seperatevalues(dfn,temp);
                                                    for(auto &it : temp){
                                                        newresult.push_back(whichduplicate[stoi(x)][stoi(it)]);
                                                    }
                                                }
                                                bool flag = deleteFiles(newresult);
                                                if(!flag){
                                                    std::cout<<"There is some error.";
                                                }else{
                                                    std::cout<<"File succesfully deleted";
                                                }
                                            }
                                            break;
                                default: std::cerr<<"you have entered a wrong option";
                            }
                            
                        }
                    }catch(...){
                        std::cerr<<"please provide a path of the folder.";
                    }
                    /*check if the given path is of a folder or not*/
                }
                break;
        case 3: {
                        try{
                        const char* dirpath= argv[2];
                        std::vector<std::string> result;
                        try{
                            long long maxsize = atoi(argv[3]) * 1024 * 1024;
                            result = detectLargeFiles(dirpath,maxsize);
                            std::unordered_map<int,std::string> paths;
                            int i = 1;
                            for(auto &x : result){
                                std::cout<<i<<"."<<x<<std::endl;
                                paths[i++] = x;
                            }

                            std::cout<<"Do You Want to Delete these files(Y/N)";
                            char ch = 'N';
                            std::cin>>ch;
                            ch = toupper(ch);
                            if(ch == 'Y'){
                                char opt = 'a';
                                std::cout<<"Do you want to delete all the files or select some files to delete:(a/c)\n";
                                std::cin>>opt;
                                askAndDelete(opt,result,paths);
                            }
                            
                        }catch(...){
                            result = detectLargeFiles(dirpath);
                            std::unordered_map<int,std::string> paths;
                            int i = 1;
                            for(auto &x : result){
                                std::cout<<i<<"."<<x<<std::endl;
                                paths[i++] = x;
                            }
                            for(auto &x : result){
                                std::cout<<x<<std::endl;
                            }

                            std::cout<<"Do You Want to Delete these files(Y/N)";
                            char ch = 'N';
                            std::cin>>ch;
                            ch = toupper(ch);
                            if(ch == 'Y'){
                                char opt = 'a';
                                std::cout<<"Do you want to delete all the files or select some files to delete:(a/c)\n";
                                std::cin>>opt;
                                askAndDelete(opt,result,paths);
                            }
                        }
                    }catch(...){
                        std::cerr<<"please provide a path of the folder.";
                    }
                }
                break;
        case 4: {
                    try{
                        const char* dirpath= argv[2];
                        std::vector<std::string> result;
                        try{
                            std::string ext = argv[3];
                            result = detectFileByExtension(dirpath,ext);
                            std::unordered_map<int,std::string> paths;
                            int i = 1;
                            for(auto &x : result){
                                std::cout<<i<<"."<<x<<std::endl;
                                paths[i++] = x;
                            }
                            std::cout<<"Do You Want to Delete these files(Y/N)";
                            char ch = 'N';
                            std::cin>>ch;
                            ch = toupper(ch);
                            if(ch == 'Y'){
                                char opt = 'a';
                                std::cout<<"Do you want to delete all the files or select some files to delete:(a/c)\n";
                                std::cin>>opt;
                                askAndDelete(opt,result,paths);
                            }
                            
                        }catch(...){
                            std::cerr<<"please provide a extension(eg: .cpp, .json)";
                        }
                    }catch(...){
                        std::cerr<<"please provide a path of the folder.";
                    }
                }
                break;
        case 5: {
                    try{
                        const char* dirpath= argv[2];
                        std::vector<std::string> result;
                        try{
                            std::string ext = argv[3];
                            result = detectFileByExtension(dirpath,ext);
                            std::unordered_map<int,std::string> paths;
                            int i = 1;
                            for(auto &x : result){
                                std::cout<<i<<"."<<x<<std::endl;
                                paths[i++] = x;
                            }
                            std::cout<<"Start deleting:(Y/N)";
                            char ch = 'N';
                            std::cin>>ch;
                            ch = toupper(ch);
                            if(ch == 'Y'){
                                char opt = 'a';
                                std::cout<<"Do you want to delete all the files or select some files to delete:(a/c)\n";
                                std::cin>>opt;
                                askAndDelete(opt,result,paths);
                            }
                            
                        }catch(...){
                            std::cerr<<"please provide a extension(eg: .cpp, .json)";
                        }
                    }catch(...){
                        std::cerr<<"please provide a path of the folder.";
                    }
                }
                break;
        case 6: {
                    try{
                        const char* dirpath= argv[2];
                        std::vector<std::string> result;
                        try{
                            long long maxsize = atoi(argv[3]) * 1024 * 1024;
                            result = detectLargeFiles(dirpath,maxsize);
                            std::unordered_map<int,std::string> paths;
                            int i = 1;
                            for(auto &x : result){
                                std::cout<<i<<"."<<x<<std::endl;
                                paths[i++] = x;
                            }

                            std::cout<<"Do You Want to Delete these files(Y/N)";
                            char ch = 'N';
                            std::cin>>ch;
                            ch = toupper(ch);
                            if(ch == 'Y'){
                                char opt = 'a';
                                std::cout<<"Do you want to delete all the files or select some files to delete:(a/c)\n";
                                std::cin>>opt;
                                askAndDelete(opt,result,paths);
                            }
                            
                        }catch(...){
                            result = detectLargeFiles(dirpath);
                            std::unordered_map<int,std::string> paths;
                            int i = 1;
                            for(auto &x : result){
                                std::cout<<i<<"."<<x<<std::endl;
                                paths[i++] = x;

                            }
                            for(auto &x : result){
                                std::cout<<x<<std::endl;
                            }

                            std::cout<<"Do You Want to Delete these files(Y/N)";
                            char ch = 'N';
                            std::cin>>ch;
                            ch = toupper(ch);
                            if(ch == 'Y'){
                                char opt = 'a';
                                std::cout<<"Do you want to delete all the files or select some files to delete:(a/c)\n";
                                std::cin>>opt;
                                askAndDelete(opt,result,paths);
                            }
                        }
                    }catch(...){
                        std::cerr<<"please provide a path of the folder.";
                    }
                    break;
                }
        case 7: {
                    try{
                        const char* filepath = argv[2];
                        std::vector<std::vector<std::string>> result = detectDuplicateFolder(filepath);
                        std::unordered_map<int,std::unordered_map<int,std::string>> whichduplicate;
                        int i = 1,j = 1;
                        for(auto &x : result){
                            std::cout<<"===============================     "<<i<<"     =======================================\n";
                            for(auto &y : x){
                                std::cout<<j<<"."<<y<<std::endl;
                                whichduplicate[i][j] = y;
                                j++;
                            }
                            i++;
                            std::cout<<std::endl<<"=============================================================================="<<std::endl;
                        }
                        std::cout<<"Do you want to delete duplicate files(Y/N):";
                        char ch = 'N';
                        std::cin>>ch;
                        ch = toupper(ch);
                        if(ch == 'Y'){
                            std::cout<<"Do you want to delete all the duplicates or selected duplicates(a/c):\n";
                            char opt1 = 'a';
                            std::cin>>opt1;
                            opt1 = tolower(opt1);
                            switch(opt1){
                                case 'a':   {
                                                bool flag = true;
                                                std::cout<<"\n*Caution:this will delete all the duplicate files shown and leave a random file of each duplicate group*\n";
                                                std::cout<<"Do you want to continue(Y/N):\n";
                                                char ch = 'Y';
                                                std::cin>>ch;
                                                if(ch == 'Y'){
                                                    for(auto &x : result){
                                                        flag = flag && deleteDuplicateFiles(x);
                                                    }
                                                    if(!flag){
                                                        std::cout<<"There is some error.\n";
                                                    }else{
                                                        std::cout<<"File succesfully deleted\n";
                                                    }
                                                }
                                            }
                                            break;
                                case 'c':   {
                                                std::cout<<"enter the number associated with the duplicate file bunch:";
                                                std::string res = "";
                                                std::vector<std::string> duplis_no;
                                                std::cin>>res;
                                                seperatevalues(res,duplis_no);
                                                std::vector<std::string> newresult;
                                                for(auto &x : duplis_no){
                                                    std::cout<<"===============================     "<<x<<"     =======================================\n";
                                                    std::cout<<"Provide me with numbers associated to the file path(eg:{1}.C:/Users/) (input: 1,10,15)\n";
                                                    std::string dfn = ""; //delete filepath number
                                                    std::cin>>dfn;
                                                    std::vector<std::string> temp;
                                                    seperatevalues(dfn,temp);
                                                    for(auto &it : temp){
                                                        newresult.push_back(whichduplicate[stoi(x)][stoi(it)]);
                                                    }
                                                }
                                                bool flag = deleteFiles(newresult);
                                                if(!flag){
                                                    std::cout<<"There is some error.";
                                                }else{
                                                    std::cout<<"File succesfully deleted";
                                                }
                                            }
                                            break;
                                default: std::cerr<<"you have entered a wrong option";
                            }
                            
                        }
                    }catch(...){
                        std::cerr<<"please provide a path of the folder.";
                    }
                    /*check if the given path is of a folder or not*/
                }
                break;
        case 8: {
                    const char* dirpaths = argv[2];
                    folderBifurcation(dirpaths,true);
                }
                break;
        default: std::cerr<<"Command does-not exit";
    }

    return 0;
}