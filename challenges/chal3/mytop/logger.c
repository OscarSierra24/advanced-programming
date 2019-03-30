#include <stdio.h> 
#include <stdarg.h>
#include <string.h>
#include <syslog.h>

#define RESET		0
#define BRIGHT 		1

#define BLACK 		0
#define RED		    1  //PANIC
#define GREEN		2  //INFO 
#define YELLOW		3  //WARNING
#define CYAN		6  //ERROR
#define	WHITE		7  

int currentLog = 0; //0: stdout, 1: syslog

int initLogger(char *log){
    if(!strcmp(log, "stdout")){
        currentLog = 0;
    }
    else if(!strcmp(log, "syslog")){
        currentLog = 1;
    }
    else{
        currentLog = 0;        
    }

    return currentLog;
}

void textcolor(int attr, int fg, int bg){
	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}
int infof(const char *format, ...){
    int charCount = 0;
    textcolor(BRIGHT, GREEN, BLACK);
    va_list args;
    va_start(args, format);
            
    switch(currentLog){
        case 0:
            charCount = vprintf(format, args);
            break;
        case 1:
    		openlog ("Logger-INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
            vsyslog(LOG_INFO, format, args);
    		closelog();
            break;
    }
    va_end(args);
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}
int warnf(const char *format, ...){
    int charCount = 0;
    textcolor(BRIGHT, YELLOW, BLACK);
    va_list args;
    va_start(args, format);

    switch(currentLog){
        case 0:
            charCount = vprintf(format, args);
            break;
        case 1:
            openlog ("Logger-INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
            vsyslog(LOG_INFO, format, args);
    		closelog();
            break;
    }
    va_end(args);        
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}
int errorf(const char *format, ...){
    int charCount = 0;
    textcolor(BRIGHT, CYAN, BLACK);
    va_list args;
    va_start(args, format);

    switch(currentLog){
        case 0:
            charCount = vprintf(format, args);
            break;
        case 1:
            openlog ("Logger-INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
            vsyslog(LOG_INFO, format, args);
    		closelog();
            break;
    }
    va_end(args);        
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}
int panicf(const char *format, ...){
    int charCount;
    textcolor(BRIGHT, RED, BLACK);
    va_list args;
    va_start(args, format);
        switch(currentLog){
            case 0:
                charCount = vprintf(format, args);
            break;
        case 1:
            openlog ("Logger-INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
            vsyslog(LOG_INFO, format, args);
    		closelog();
            break;
    }
    va_end(args);
    textcolor(RESET, WHITE, BLACK);
    return charCount;
}