#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

/**
 * Function: getUser
 * Purpose: To retrieve the username associated with a given user ID.
 * Parameters: uid_t uid - User ID.
 * Returns: char* - Pointer to the username string.
 */
char *getUser(uid_t uid)
{
    struct passwd *pws;
    pws = getpwuid(uid);
    return pws->pw_name;
}

/**
 * Function: getGroup
 * Purpose: To retrieve the group name associated with a given group ID.
 * Parameters: gid_t gid - Group ID.
 * Returns: char* - Pointer to the group name string.
 */
char *getGroup(gid_t gid)
{
    struct group *grp;
    grp = getgrgid(gid);
    return grp->gr_name;
}

int main(int argc, char *argv[])
{
    struct dirent *entry;    // Directory entry pointer
    struct stat fileStat;    // File status structure
    DIR *dp = opendir(".");  // Open current directory

    if (dp == NULL)
    {
        perror("opendir");   // print error message
        return 1;
    }

    while ((entry = readdir(dp)))  // Read directory entries
    {
        if (entry->d_name[0] == '.') // Check for hidden files
        {
            continue; // Skip hidden files
        }
        if (stat(entry->d_name, &fileStat) < 0)  // Get file status
        {
            perror("stat");   // print error message
            continue;
        }

        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");  // File type
        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-"); // Owner permissions (read)
        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-"); // Owner permissions (write)
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-"); // Owner permissions (execute)
        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-"); // Group permissions (read)
        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-"); // Group permissions (write)
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-"); // Group permissions (execute)
        printf((fileStat.st_mode & S_IROTH) ? "r" : "-"); // Others permissions (read)
        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-"); // Others permissions (write)
        printf((fileStat.st_mode & S_IXOTH) ? "x" : "-"); // Others permissions (execute)
        printf(" %ld", fileStat.st_nlink);                // Number of hard links
        printf(" %s", getUser(fileStat.st_uid));          // User name of owner
        printf(" %s", getGroup(fileStat.st_gid));         // Group name of owner
        printf(" %ld", fileStat.st_size);                 // Size of file in bytes
        printf(" %s", ctime(&fileStat.st_mtime) + 4);     // Last modification time
        printf(" %s\n", entry->d_name);                   // File name
    }

    closedir(dp); // Close directory
    return 0;
}