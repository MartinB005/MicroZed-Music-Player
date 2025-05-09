#include <stdio.h>
#include "FileBrowser.h"

void FileBrowser::openDir(char* path) {
    if (dir != NULL) closedir(dir);
    dir = opendir(path);
    

    if (dir == NULL) {
        perror("opendir");
    }
}

void FileBrowser::listFiles(std::function<void(char*, bool)> func) {
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG) {
            func(entry->d_name, entry->d_type == DT_DIR);
        }
    }
}




