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

class FileBrowser {
    public:
        void openDir(std::string name);
        std::vector<File> listFiles();

    private:
        std::string currentPath = "";
        DIR* dir;

        FileType getFileType(struct dirent *entry);
};