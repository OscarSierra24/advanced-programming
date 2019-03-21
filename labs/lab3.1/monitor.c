#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include "logger.h"
#include <sys/inotify.h>
#include <limits.h>
#include <ftw.h>
#include <string.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + 1))

int inotifyFd, wd; //inotify file descriptor, watch descriptor 
ssize_t numRead;
char *p;
char buf[BUF_LEN] __attribute__ ((aligned(8)));
struct inotify_event *event;
int flags;
int nftwVal;
int isPrevActionMv = 0; //checks if the previous action was move, if it is, stores the cookie
char *prevName;

int refresh_watch(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    wd = inotify_add_watch(inotifyFd, fpath, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_ISDIR);
    if (wd == -1){
        panicf("Error: inotify_add_watch failed\n");
        return 0;
    }
    return 0;
}

int main(int argc, char *argv[]){
    initLogger("stdout");

    if(argc < 2){
        panicf("Usage: ./monitor your_directory\n");
        return 0;
    }

    if((inotifyFd = inotify_init()) == -1){
        panicf("Error: cannot init sys/inotify\n");
        return 0;
    }
    
    nftwVal = nftw(argv[1], refresh_watch, 20, flags);
    if(nftwVal == -1){
        errorf("Error: on nftw");
        return 0;
    }

    for(;;){ 
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            errorf("read() from inotify fd returned 0!\n");

        if (numRead == -1)
            errorf("read\n");

        for (p = buf; p < buf + numRead;) {
            event = (struct inotify_event *) p;
            
            char type[7];
            if(event->mask & IN_ISDIR){
                type[0] = 'f';
                type[1] = 'o';
                type[2] = 'l';
                type[3] = 'd';
                type[4] = 'e';
                type[5] = 'r';
                type[6] = '\0';
            } else{
                type[0] = 'f';
                type[1] = 'i';
                type[2] = 'l';
                type[3] = 'e';
                type[4] = '\0';
            }

            if (event->mask & IN_CREATE)        infof("%s %s was created\n", type, event->name);            
            if (event->mask & IN_DELETE)        infof("%s %s was deleted\n", type, event->name);
            
            if (event->cookie > 0){//if cookie exists, then move happened    
                if(isPrevActionMv){//
                    if(strcmp(prevName, event->name) != 0){
                        isPrevActionMv = 0;
                        infof("%s was renamed to %s\n", prevName, event->name);
                        break;
                    }
                }
                isPrevActionMv = event->cookie;
                prevName = event->name;
            }else{
                isPrevActionMv = 0;
            }

            if((event->mask & IN_DELETE || event->mask & IN_CREATE) || (event->mask & IN_ISDIR && (event->mask & IN_MOVED_FROM || event->mask & IN_MOVED_TO))){
                nftwVal = nftw(argv[1], refresh_watch, 20, flags);
                if(nftwVal == -1){
                    errorf("Error: on nftw");
                    return 0;
                }
            }

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}