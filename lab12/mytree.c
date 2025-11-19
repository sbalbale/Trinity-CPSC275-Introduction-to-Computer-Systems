#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>


#define INDENT_STEP 3

/**
 * Function: printTree
 * Purpose: To recursively print the directory structure.
 * Parameters: const char *dirPath - Path of the directory to print.
 *             int level - Current level of indentation.
 */
void printTree(const char *dirPath, int level)
{
    struct dirent *entry;          // Directory entry pointer
    DIR *dp = opendir(dirPath);    // Open the directory

    if (dp == NULL)                 // Error handling
    {
        perror("opendir");          // print error message
        return;
    }

    while ((entry = readdir(dp)))   // Read directory entries
    {
        if (entry->d_name[0] == '.') // Skip hidden files and directories
        {
            continue;
        }

        for (int i = 0; i < level * INDENT_STEP; i++) // Indentation
        {
            putchar(' '); // Indent according to the level
        }
        printf("%s\n", entry->d_name); // Print directory name

        // Construct new path for subdirectory
        char newPath[1024];
        strcpy(newPath, dirPath);
        strcat(newPath, "/");
        strcat(newPath, entry->d_name);
        

        struct stat fileStat;       // File status structure
        if (stat(newPath, &fileStat) == 0 && S_ISDIR(fileStat.st_mode)) // Check if it's a directory
        {
            printTree(newPath, level + 1); // Recursive call for subdirectory
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[])
{
    struct dirent *entry;    // Directory entry pointer
    struct stat fileStat;    // File status structure
    DIR *dp = opendir("../..");  // Open current directory
    if (dp == NULL)
    {
        perror("opendir");   // Error handling
        return 1;
    }
    printTree("..", 0); // Start printing tree from parent directory
    closedir(dp);
}