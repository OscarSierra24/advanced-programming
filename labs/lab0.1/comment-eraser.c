#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
	//printf(argv[1]);
	//printf("\n");

	FILE *output = fopen("output.c", "w+");
	FILE *fp = fopen(argv[1], "r");

	if(fp == NULL){
		perror("The file could not be opened.\n");
		return 0;
	}

	char prevCh = '\0';
	char currentCh = '\0';
	int canWrite = 1;
	int state = 0; //when state = 0 we are not in a state, when state = 1 we are at type 1 comment, when state = 2 type 2 comment and when state = 3 we are inside a string 
	while((currentCh=fgetc(fp)) != EOF){
		//printf("current char: %c \n", currentCh);
		if(currentCh == '/' && prevCh == '/' && state == 0){
			//printf("detecté comentario");
			canWrite = 0;
			//printf("%d \n",canWrite);
			state  = 1;
		}
		if(currentCh == '*' && prevCh == '/' && state == 0){
			//printf("detecté comentario tipo 2");
			canWrite = 0;
			state = 2;
		}
		if(currentCh == '"' && state == 0){
			state = 3;
		}
		if (currentCh == '"' && state == 3){
			if(prevCh != '\\'){
				state = 0;
				prevCh = '\0';
				//printf("salí del string");
			}
		}

		if (currentCh == '\n' && state == 1){
			//printf("salí de comentario tipo 1");
			canWrite = 1;
			prevCh = '\0';
			state = 0;
		}
		if(currentCh == '/' && prevCh == '*' && state == 2){
			//printf("salí de comentario tipo2 ");
			canWrite = 1;
			prevCh = '\0';
			state = 0;
			continue;
		}
		if(canWrite == 1){
			//printf("%c \n", currentCh);
			fputc(prevCh, output);
		}
		prevCh = currentCh;
	}

	fclose(fp);
	fclose(output);

	return 0;
}
