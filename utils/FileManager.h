#pragma once

#include <dirent.h>
#include <functional>
#include <vector>
#include <string>

enum FileType {
    CHILD_DIR,
    PARENT_DIR,
    SOUND_FILE,
    UNKNOWN_FILE,
};

class File {

    public:
        
        /**
         * @brief Construct a new File object
         * 
         * @param path absolute path
         * @param type file type
         */
        File(std::string path, FileType type);
        FileType getType();
        std::string getName();
        std::string getAbsoulutePath();

    private:
        FileType type;
        std::string path;

};

class FileManager {
    public:
        FileManager();

        /**
         * @brief Open the directory
         * 
         * @param name of the directory
         */
        void openDir(std::string name);

        /**
         * @brief list directory files
         * 
         * @return std::vector<File>& file list reference
         */
        std::vector<File>& listFiles();

    private:
        std::string currentPath = "";
        std::vector<File> files;
        DIR* dir = NULL;
        bool listed;

        /**
         * @brief Get the File Type from dirent entry
         */
        FileType getFileType(struct dirent *entry);
};