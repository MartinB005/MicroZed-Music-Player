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
        void openDir(std::string name);
        std::vector<File>& listFiles();

    private:
        std::string currentPath = "";
        std::vector<File> files;
        DIR* dir = NULL;
        bool listed;

        FileType getFileType(struct dirent *entry);
};