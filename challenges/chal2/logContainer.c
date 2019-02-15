#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

struct logContainer{
    const char *category;
    const char *text[1000];
    int lastTextPos;
};

struct logContainer container[1000];
int lastEl = 0; //last logContainer position
char catMem[10000][10000]; //category memory 
char tMem[10000][10000]; //text memory
int lastTMem = 0; //last tMem position

void insertNewLog(char category[],char text[]){
    for(int i = 0; i<lastEl;i++){
        if(strcmp(catMem[i], category) == 0){
            /*repeated category*/
            container[i].text[container[i].lastTextPos] = strcpy(tMem[lastTMem], text);
            container[i].lastTextPos++;
            lastTMem++;

            return;
       }
    }
    /*Surpassing this line of code means that we have a new category*/
    container[lastEl].category = strcpy(catMem[lastEl], category);
    container[lastEl].text[0] = strcpy(tMem[lastTMem], text);
    container[lastEl].lastTextPos = 1;
    lastEl++;
    lastTMem++;
}
void extractData(){
    size_t output = open("dmesg_report.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    for(int i = 0; i < lastEl; i++){
        write(output, container[i].category, strlen(container[i].category));
        write(output, "\n", 1);
        for(int j = 0; j < container[i].lastTextPos; j++){
            write(output, container[i].text[j], strlen(container[i].text[j]));
            write(output, "\n", 1);
        }
    }
    close(output);
}
