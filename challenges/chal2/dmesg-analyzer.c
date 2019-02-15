#include <stdio.h>
#include <string.h>

#define REPORT_FILE "report.txt"
#define MAX_SEGMENT_SIZE 5000

void insertNewLog(const char *category, const char* text);
void extractData();
void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {
    if (argc < 2) {
	    printf("Usage:./dmesg-analizer.o logfile.txt\n");
    	return 1;
    }
    analizeLog(argv[1], REPORT_FILE);
    extractData();
    return 0;
}

void analizeLog(char *logFile, char *report) {
    FILE *file;
    int state;  //1:   saving time variable  2: saving category
                //3:   saving text           100: when multiple lines of the same log are going to be read  
                //101: for text when reading multiple lines of one category
    file = fopen(logFile, "r");
    int c; //char
    char varTime[MAX_SEGMENT_SIZE] = ""; 
    char category[MAX_SEGMENT_SIZE] = "";
    char text[MAX_SEGMENT_SIZE] = "";
    printf("Generating Report from: [%s] log file\n", logFile);
    printf("\n\n");

    state = 1;
    int i = 0;
    int seekSpace = 0; 
    while((c = getc(file)) != EOF){
        if(state == 1){ //For vartime
            if(c == ']'){
                varTime[i] = ']';
                varTime[++i] = '\0';
                //printf("vartime: %s\n", varTime);

                i = 0;     
                state = 2;
                continue;
            }
            varTime[i] = c;
            i++;
        }
        if(state == 2){ // for category
            if(c == ':'){ //if I read ':' the next value that im looking for is an space
                seekSpace = 1;
                category[i] = ':';
                i++;
                continue;
            }
            if(seekSpace && c == ' '){
                category[i] = '\0';
                //printf("category: %s\n", category);
                i = 0;
                state = 3;
                seekSpace = 0;
                continue;
            }      
            if(seekSpace && c == '\n'){ //when multiple lines of the same log are going to be read
                //printf("multiple lines handler\n");
                category[++i] = '\0';
                //printf("category: %s\n", category);
                seekSpace = 0;
                i = 0;
                state = 100;
                continue;
            }
            seekSpace = 0;

            if(c == '\n'){ 
                category[i] = '\0';
                strcpy(text, category);
                strcpy(category, "General:\0");
                //printf("GNRL vartime: %s, category: %s, text: %s\n", varTime, category, text);
                insertNewLog(category, strcat(varTime, text));

                i = 0;
                state = 1;
                continue;
            }

            category[i] = c;
            i++;
        }
        if(state == 3){// for text 
            if(c == '\n'){
                text[i] = '\0';
                //printf("vartime: %s, category: %s, text: %s\n", varTime, category, text);
                insertNewLog(category, strcat(varTime, text));

                i=0;
                state = 1;
                continue;
            }
            text[i] = c;
            i++;
        }
        if(state == 100){// for vartime when reading multiple lines of one category
            if(c == ']'){
                varTime[i] = ']';
                varTime[++i] = '\0';
                //printf("vartime ml: %s\n", varTime);

                i = 0;     
                state = 101;
                continue;
            }
            varTime[i] = c;
            i++;
        }
        if(state == 101){// for text when reading multiple lines of one category    
            if(c == ':'){
                seekSpace = 1;
                text[i] = ':';
                i++;
                continue;
            }
            if(seekSpace && c == ' '){
                //printf("seekSpace && c == ' ' case\n");
                strcpy(category, text);
                category[i] = '\0';
                //printf("category: %s\n", category);
                
                i = 0;
                state = 3;
                seekSpace = 0;
                continue;
            }
            if(seekSpace && c == '\n'){
                //printf("seekSpace && c == new line case\n");
                strcpy(category, text);
                category[i] = '\0';
                
                i = 0;
                state = 100;
                seekSpace = 0;
                continue;
            }
            if(c == '\n'){
                text[i] = '\0';
                //printf("ML vartime %s, category: %s, text: %s\n", varTime, category, text);
                insertNewLog(category, strcat(varTime, text));

                i = 0;
                state = 100;
                continue;
            }
            text[i] = c;
            i++;
        }          
    }
    fclose(file);
    
    //FILE *file = fopen(logFile, "r");

    //int refReport = open("report.txt", O_RDONLY);

    //int c;
    //while ((c = getc(file)) != EOF){
    //    printf("%c", c);
    //    write(refReport, c, 1);
    //}

    //close(report);
    //fclose(file); 

    //printf("Report is generated at: [%s]\n", report);
}