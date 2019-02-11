#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    const char *ptsPath = "/dev/pts/";
    struct dirent *pDirent;

    DIR * dir = opendir(ptsPath);
    if (dir == NULL){
        printf("Cannot open directory");
    }
    
    pDirent = readdir(dir); // [.] folder
    pDirent = readdir(dir); // [..] folder
    
    while((pDirent = readdir(dir)) != NULL){
        char *fullpath = (char *) malloc(1 + 9 + strlen(pDirent->d_name));
        strcpy(fullpath, ptsPath);
        strcat(fullpath, pDirent->d_name);
        
        int userFile = open(fullpath, O_WRONLY);
        int i = 0;
        for(i=1;i<argc;i++){
            write(userFile, argv[i], strlen(argv[i]));
            write(userFile, " ", 1);
        }
        write(userFile, "\n", 1);
        close(userFile);
    }
    closedir(dir);

    return 1;
}