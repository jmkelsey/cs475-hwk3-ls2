#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include "ls2.h"

#define MAX_PATH_LENGTH 256
#define MAX_INDENT_LENGTH 16

bool ls2(stack_t* s, char *filename, char *searchTerm, int depth) {
    //printf("running at depth %i\n", depth);
    DIR *dir;
    struct dirent* entry;
    struct stat buf;

    bool foundTarget = false;

    char indents[MAX_INDENT_LENGTH];
    memset(indents, '\0', MAX_INDENT_LENGTH);
    for (int i = 0; i < depth; i++) {
        strcat(indents, INDENT);
    }

    // Open the directory
    dir = opendir(filename);
    if (dir == NULL) {
        perror("Error opening directory");
        return false;
    }

    // Read through the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a directory
        if (entry->d_type == DT_DIR) {
            // Skip "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            
            // Build the path to the directory
            char path[MAX_PATH_LENGTH];
            snprintf(path, MAX_PATH_LENGTH+2, "%s/%s", filename, entry->d_name);

            // Recurse into the directory
            if (ls2(s, path, searchTerm, depth + 1)) {
                // Allocate memory for the full path
                char *fullPath = malloc(strlen(indents) + strlen(entry->d_name) + strlen("/ (directory)") + 1);
                if (fullPath == NULL) {
                    perror("Error allocating memory");
                    return false;
                }
                
                // Build the full path
                sprintf(fullPath, "%s%s/ (directory)", indents, entry->d_name);
                
                // Push the full path onto the stack
                push(s, fullPath);

                foundTarget = true;
                
            }
        } else if (entry->d_type == DT_REG) {
            // Check if the entry matches the search term
            if (searchTerm == NULL || strstr(entry->d_name, searchTerm)) {
                //find the size
                lstat(entry->d_name, &buf);

                // Allocate memory for the full path
                char *fullPath = malloc(strlen(indents) + strlen(entry->d_name) + (strlen("I cant cast size to strlen")) + strlen(" ( bytes)") + 1);
                if (fullPath == NULL) {
                    perror("Error allocating memory");
                    return false;
                }
                
                long int fileSize = buf.st_size;
                // Build the full path
                sprintf(fullPath, "%s%s (%ld bytes)", indents, entry->d_name, fileSize);
                
                // Push the full path onto the stack
                push(s, fullPath);

                foundTarget = true;
            }
        }
    }
    closedir(dir);
    if (searchTerm == NULL) {
        foundTarget = true;
    }
    
    return foundTarget;
}