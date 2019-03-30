#include <stdio.h>
#include <stdlib.h>           
#include <dirent.h>           
#include <sys/types.h>        
#include "logger.h"           
#include <unistd.h>           
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

#define PROC "/proc"

FILE *saveFile;

void clear();
void retrieveEachProcessesInfo(DIR *dp, struct dirent *ep, int output);

FILE *createDashboardSnapshot(){
  FILE *fptr;
  char filename[1024];
  
  time_t t = time(NULL);
  struct tm ts = *localtime(&t);
 
  sprintf(filename, "mytop-status-%d%d%d.txt",ts.tm_year+1900,ts.tm_mon+1,ts.tm_mday);
  
  fptr = fopen(filename, "w");
  return fptr;
} 

void signalManager(int sig){
  saveFile = createDashboardSnapshot();

  DIR *dp;
  struct dirent *ep;     
  dp = opendir (PROC);
  ep = readdir(dp);
  fprintf(saveFile, "|-------|-------|--------------------------|------------------|------------|-----------|------------|\n");
  fprintf(saveFile, "|PID    |Parent |Name                      |State             |Memory      |# Threads  |Open Files  |\n");
  fprintf(saveFile, "|-------|-------|--------------------------|------------------|------------|-----------|------------|\n");
  retrieveEachProcessesInfo(dp, ep, 1);
  fprintf(saveFile, "|-------|-------|--------------------------|------------------|------------|-----------|------------|\n");
  exit(-1);
}

char *trim(char *str){
  char *eos;

  while(isspace((unsigned char)*str)) str++;
  eos = str + strlen(str) - 1;
  while(eos > str && isspace((unsigned char)*eos)) eos--;

  eos[1] = '\0';

  return str;
}

void printWithFormat(char *pid,char *ppid,char *propState,char *threads,char *name,char *memory, int openFiles, int output){
  if(output == 0){
    printf("|%-7s|%-7s|%-26s|%-18s|%-12s|%-11s|%-12d|\n", pid, ppid, name, propState, memory, threads, openFiles);
  } else if(output == 1){
    fprintf(saveFile, "|%-7s|%-7s|%-26s|%-18s|%-12s|%-11s|%-12d|\n", pid, ppid, name, propState, memory, threads, openFiles);
  }
}

void retrieveEachProcessesInfo(DIR *dp, struct dirent *ep, int output){
  dp = opendir (PROC);
  
  if (dp == NULL){
    panicf ("Couldn't open /proc");
    exit(-1);
  }

  char path[1024];
  FILE *fp;
  char ch;

  char pid[1024];
  char ppid[1024];
  char name[1024];
  char propState[1024];
  char threads[1024];
  char memory[1024];

  char tempProp[1024]; //temporal, makes code easier
  int openFiles = 0;

  while ((ep = readdir (dp))){
    int pidN = -1; 
    sscanf(ep->d_name, "%d", &pidN); 
    
    if(pidN == -1){ //filters integers only, which means that only processes are going to be analized 
      continue;
    }

    //--------------------- OPEN PID FILE TO RETRIEVE INFO --------------------//

    sprintf(path, "%s/%s/status", PROC, ep->d_name);
    
    fp = fopen(path, "r");
    if (fp == NULL){
      panicf("could not read the file %s/%s/status\n", PROC, ep->d_name);
      continue;
    }
    char prop[100];
    int i = -1;
    int state = 0; // 0: initial state; 1: when ':' was found; 2: while searching for end of line ('\n')
    
    while((ch = fgetc(fp)) != EOF){
      
      if(state == 0){
        i++;

        if(ch == ':'){
          state = 1;
          prop[i] = '\0'; 
          i = -1;
          continue;
        }

        prop[i] = ch;
      } else if (state == 1){
        state = 2;
      } else if (state == 2){
        i++;

        if(ch == '\n'){
          tempProp[i] = '\0';

          if(strcmp(prop, "Pid") == 0){
            strncpy(pid, tempProp, (i+1));
          }
          else if(strcmp(prop, "PPid") == 0){
            strncpy(ppid, tempProp, (i+1) );
          }
          else if(strcmp(prop, "State") == 0){
            strncpy(propState, tempProp, (i+1));
          }
          else if(strcmp(prop, "Threads") == 0){
            strncpy(threads, tempProp, (i+1));
          }
          else if(strcmp(prop, "Name") == 0){
            strncpy(name, tempProp, (i+1));
          }
          else if(strcmp(prop, "VmHWM") == 0){
            strncpy(memory, trim(tempProp), (i+1));
          }
          i = -1;
          state = 0;
          continue;
        }
        tempProp[i] = ch;
      } 
    }
    fclose(fp);
    
    //--------------GET OPEN FILES----------------------// 
    DIR *dfd;
    struct dirent *dirnt;
    openFiles = 0;
    char pathfd[1024];
    sprintf(pathfd, "%s/%s/fd", PROC, ep->d_name);
    dfd = opendir (pathfd);
    if (dfd == NULL){
      errorf ("Couldn't open %s\n", pathfd);
      continue;
    }
    while ((dirnt = readdir (dfd))){
      openFiles++;
    }
    (void) closedir (dfd);
    //-------------------------------------------------//

    printWithFormat(pid, ppid, propState, threads, name, memory, openFiles, output);
    
    //-------------------------------------------------------------------------// 
  }
  (void) closedir (dp);
}

int main(){
  initLogger("stdout");
  signal(SIGINT, signalManager); 
  
  DIR *dp;
  struct dirent *ep;     
  
  dp = opendir (PROC);   //avoids warnings 
  ep = readdir(dp);         //avoids warnings

  for(;;){
    clear();
    printf("|-------|-------|--------------------------|------------------|------------|-----------|------------|\n");
    printf("|PID    |Parent |Name                      |State             |Memory      |# Threads  |Open Files  |\n");
    printf("|-------|-------|--------------------------|------------------|------------|-----------|------------|\n");
    retrieveEachProcessesInfo(dp, ep, 0);
    printf("|-------|-------|--------------------------|------------------|------------|-----------|------------|\n");
    sleep(2);
  }

  return 0;
}

void clear() {
  system("clear");
}