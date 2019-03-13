#include <stdio.h> 
#include <stdarg.h>

#define RESET		0
#define BRIGHT 		1

#define BLACK 		0
#define RED		    1  //PANIC
#define GREEN		2  //INFO 
#define YELLOW		3  //WARNING
#define CYAN		6  //ERROR
#define	WHITE		7  

int initLogger(char *logType){
    
}

void textcolor(int attr, int fg, int bg){
	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}
int infof(const char *format, ...){
    int charCount;
    textcolor(BRIGHT, GREEN, BLACK);
    va_list args;
    va_start(args, format);
    charCount = vprintf(format, args);
    va_end(args);
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}
int warnf(const char *format, ...){
    int charCount;
    textcolor(BRIGHT, YELLOW, BLACK);
    va_list args;
    va_start(args, format);
    charCount = vprintf(format, args);
    va_end(args);
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}
int errorf(const char *format, ...){
    int charCount;
    textcolor(BRIGHT, CYAN, BLACK);
    va_list args;
    va_start(args, format);
    charCount = vprintf(format, args);
    va_end(args);
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}
int panicf(const char *format, ...){
    int charCount;
    textcolor(BRIGHT, RED, BLACK);
    va_list args;
    va_start(args, format);
    charCount = vprintf(format, args);
    va_end(args);
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}