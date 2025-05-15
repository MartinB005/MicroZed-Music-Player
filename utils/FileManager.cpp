#include <stdio.h>
#include <string.h>
#include "FileManager.h"

File::File(std::string path, FileType type) {
    this->path = path;
    this->type = type;
}

std::string File::getName() {
    size_t pos = this->path.find_last_of('/');
    printf("pos %s %d\n", this->path.c_str(), pos);
   
    if (pos != std::string::npos) {
        return this->path.substr(pos + 1);
    }

    return this->path;
}

FileType File::getType() {
    return this->type;
}

std::string File::getAbsoulutePath() {
    return this->path;
}

FileManager::FileManager() {
    dir = NULL;
}

void FileManager::openDir(std::string name) {
    printf("dir %p\n", dir);
   // if (dir != NULL) closedir(dir);
    listed = false;

    
    if (name == "..") {
        currentPath = currentPath.substr(0, currentPath.find("/") + 1);
    }

    else if (name == "/") {
        currentPath = "/";

    } else {
        currentPath = currentPath + name + "/";
    }

    dir = opendir(currentPath.c_str());

    if (dir == NULL) {
        perror("opendir");
    }
}

std::vector<File>& FileManager::listFiles() {
    if (listed) return files;

    struct dirent *entry;
    files.clear();
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR || entry->d_type == DT_REG) {
            if (strcmp(entry->d_name, ".") == 0) continue;

            FileType fileType = getFileType(entry);
            File file(currentPath + entry->d_name, fileType);
            files.push_back(file);
        }
    }

    listed = true;

    return files;
}

FileType FileManager::getFileType(struct dirent *entry) {
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




