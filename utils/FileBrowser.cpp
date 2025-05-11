#include <stdio.h>
#include <string.h>
#include "FileBrowser.h"

File::File(std::string path, FileType type) {
    this->path = path;
    this->type = type;
}

std::string File::getName() {
    size_t pos = this->path.find('/');
   
    if (pos != std::string::npos) {
        return this->path.substr(0, pos);
    }

    return this->path;
}

FileType File::getType() {
    return this->type;
}

void FileBrowser::openDir(std::string name) {
    if (dir != NULL) closedir(dir);

    if (name == "..") {
        currentPath = currentPath.substr(0, currentPath.find("/"));
    }

    else {
        dir = opendir((currentPath + name).c_str());
    }

    if (dir == NULL) {
        perror("opendir");
    }
}

std::vector<File> FileBrowser::listFiles() {
    struct dirent *entry;
    std::vector<File> files;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG) {
            if (strcmp(entry->d_name, ".") == 0) continue;

            FileType fileType = getFileType(entry);
            File file(currentPath + "/" + entry->d_name, fileType);
            files.push_back(file);
        }
    }
}

FileType FileBrowser::getFileType(struct dirent *entry) {
    if (entry == NULL) return UNKNOWN_FILE;

    switch (entry->d_type) {
        case DT_DIR:
            return strcmp(entry->d_name, "..") == 0 ? PARENT_DIR : CHILD_DIR;

        case DT_REG:
            if (strstr(entry->d_name, ".wav") || strstr(entry->d_name, ".mp3")) {
                return SOUND_FILE;
            }
            return UNKNOWN_FILE;

        default:
            return UNKNOWN_FILE;
    }
}




