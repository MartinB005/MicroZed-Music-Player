#pragma once

#include <dirent.h>
#include <functional>

class FileBrowser {
    public:
        void openDir(char* path);
        void listFiles(std::function<void(char*, bool)>);

    private:
        DIR* dir;
};