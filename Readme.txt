How To Use:
If you have a C++ compiler already installed, you just have to execute the run.bat file.

*Note:* C++ compiler must be present to execute this project.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Disk Cleaner Application Documentation

Introduction
Disk Cleaner is a command-line application that helps you manage disk space efficiently by providing various functionalities to analyze and clean up files on your system. This documentation provides an overview of the available commands and their usage.

Commands

1. space-avail
Command:
diskcleaner space-avail
Description:
This command displays the available drives in the system along with the percentage of free space in each drive.

2. categorize
Command:
diskcleaner categorize
Description:
This command categorizes files in the specified directory into Image, Video, audios, archives, executables types. It also provides the count of files in each category and their respective sizes.

3. detect-duplicate and delete-duplicate
Command:
diskcleaner detect-duplicate "Directory-path"
diskcleaner delete-duplicate "Directory-path"
Description:
Both commands serve the same purpose of detecting duplicate files in the specified directory and its subdirectories. The application will list the duplicate files and prompt the user in a yes/no format for each file if they want to delete it. Additionally, users can select specific duplicate files to delete.

4. detect-large-files and delete-large-files
Command (Detect Large Files):
diskcleaner detect-large-files "Directory-path" "size in mb" (optional)
Command (Delete Large Files):
diskcleaner delete-large-files "Directory-path" "size in mb" (optional)
Description:
Both commands serve the same purpose of detecting large files (based on the optional size in MB parameter) in the specified directory and its subdirectories. The application will list the large files and prompt the user in a yes/no format for each file if they want to delete it. Users can also select specific large files to delete.

5. detect-same-ext-file and delete-same-ext-file
Command (Detect Files with Same Extension):
diskcleaner detect-same-ext-file "Directory-path" "extension(eg:.json, .cpp etc.)"
Command (Delete Files with Same Extension):
diskcleaner delete-same-ext-file "Directory-path" "extension(eg:.json, .cpp etc.)"
Description:
Both commands serve the same purpose of detecting files with the specified extension in the specified directory and its subdirectories. The application will list the files with the given extension and prompt the user in a yes/no format for each file if they want to delete it. Users can also select specific files to delete.

6. folder-breakdown
Command:
diskcleaner folder-breakdown "Directory-path"
Description:
This command provides a breakdown of the number and size of specific file types (Images, Videos, Audio, Archives, Documents) in the specified directory and its subdirectories.

7. help
Command:
diskcleaner help
Description:
This command provides you details of all the command available

Conclusion
This concludes the overview of the Disk Cleaner application and its available functionalities. By using these commands, you can effectively manage disk space and remove unwanted files from your system.

Contributors:
Ambuj Shukla
Shubhanshu Gupta
Aman Singh
