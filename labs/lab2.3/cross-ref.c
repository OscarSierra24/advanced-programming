#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFSIZE  4096
#define MAXWORDSIZE 100
#define WORDSQUANTITY 10000

struct wordInfo{
    char *name;
    int noOfLines;
    int *lines;
};

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("please, add the file path that will be analyzed as an argument\n");
        return 0;
    }

    unsigned int file = open(argv[1], O_RDONLY);
    if(file == -1){
        printf("an error ocurred while opening the file");
        return 0;
    }

    char buffer[BUFFSIZE+1];
    int bytes;
    char word[MAXWORDSIZE+1];
    int j = 0; // it is used to iterate words array
    int lineNo = 1;
    int wordsStored = 0;
    struct wordInfo words[WORDSQUANTITY];
    while((bytes = read(file, buffer, BUFFSIZE)) > 0){
        buffer[bytes] = '\0';

        for(int i = 0; i < BUFFSIZE; i++){
            if(buffer[i] == '.' || buffer[i] == ',' || buffer[i] == ';' || buffer[i] == '-' || buffer[i] == '_' || buffer[i] == '!' || buffer[i] == ':' || buffer[i] == '\'' || buffer[i] == '\"'){
                continue;
            }
            
            if(buffer[i] == ' ' || buffer[i] == '\0' || buffer[i] == '\n'){
                word[j] = '\0';
                //printf("word: %s\n", word);
                if(j <= 3){ //if the word size is not big enough, then that word is irrelevant and won't be saved
                    j=0;
                    if(buffer[i] == '\n'){
                        lineNo++;
                    }
                    continue;
                }
                int alreadyStored = 0; //if the word is not stored then alreadyStored = 0 
                for(int i = 0; i<wordsStored; i++){
                    if(strcmp(word, words[i].name) == 0){
                        alreadyStored = 1;
                        //printf("%s\n",words[i].name);
                        words[i].lines[words[i].noOfLines] = lineNo;
                        words[i].noOfLines++;
                        break;
                    }
                }
                if(alreadyStored==0){
                    words[wordsStored].name = (char *) malloc(j);
                    words[wordsStored].lines = (int *) malloc(1000); 
                    words[wordsStored].noOfLines = 0;
                    words[wordsStored].lines[words[wordsStored].noOfLines] = lineNo;
                    words[wordsStored].noOfLines++;
                        
                    strcpy(words[wordsStored].name, word);
                    //printf("stored at : %p\n", &words[wordsStored]);
                    wordsStored++;
                    j=0;
                } else if (alreadyStored == 1){
                    j=0;
                    if(buffer[i] == '\n'){
                        lineNo++;
                    }
                    continue;
                }
            } else {
                word[j] = buffer[i];
                j++;
            }
            if(buffer[i] == '\n'){
                lineNo++;
            }
            if(buffer[i] == '\0'){
                break;
            }

        }
    }

    for(int i = 0; i < wordsStored; i++){
        printf("%s: \n", words[i].name);
        for(int j = 0; j < words[i].noOfLines; j++){
            printf("%d, ", words[i].lines[j]);
        }
        printf("\n");
    }

    return 0;
}