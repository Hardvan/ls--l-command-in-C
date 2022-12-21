#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main()
{
    DIR *d;                                 // Directory
    struct dirent *de;                      // Directory Entry
    struct stat buf;                        // Buffer
    int i, j;                               // Loop Variables
    char P[10] = "rwxrwxrwx", AP[10] = " "; // P: Full Permissions, AP: Actual Permissions
    struct passwd *p;                       // Password
    struct group *g;                        // Group
    struct tm *t;                           // Time
    char time[26];                          // Time for display

    d = opendir("."); // Open current directory
    readdir(d);       // Read the first entry
    readdir(d);       // Read the second entry

    while ((de = readdir(d)) != NULL) // Read the remaining entries
    {
        stat(de->d_name, &buf); // Get the file information

        // 1) File Type
        if (S_ISDIR(buf.st_mode))
            printf("d");
        else if (S_ISREG(buf.st_mode))
            printf("-");
        else if (S_ISCHR(buf.st_mode))
            printf("c");
        else if (S_ISBLK(buf.st_mode))
            printf("b");
        else if (S_ISLNK(buf.st_mode))
            printf("l");
        else if (S_ISFIFO(buf.st_mode))
            printf("p");
        else if (S_ISSOCK(buf.st_mode))
            printf("s");

        // 2) File Permissions
        // P: Full Permissions, AP: Actual Permissions
        for (i = 0, j = (1 << 8); i < 9; i++, j >>= 1)
        {
            AP[i] = (buf.st_mode & j) ? P[i] : '-';
        }
        printf("%s", AP);

        // 3) No. of Hard Links
        printf(" %5d", buf.st_nlink);

        // 4) User Name
        p = getpwuid(buf.st_uid);
        printf(" %.8s", p->pw_name);

        // 5) Group Name
        g = getgrgid(buf.st_gid);
        printf(" %-8.8s", g->gr_name);

        // 6) File Size
        printf(" %8d", buf.st_size);

        // 7) Date and Time of Modification
        t = localtime(&buf.st_mtime);
        strftime(time, sizeof(time), "%b %d %H:%M", t);
        printf(" %s", time);

        // 8) File Name
        printf("%s\n", de->d_name);
    }

    return 0;
}
