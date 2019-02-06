#include <stdio.h>

int strlen(char *str);
char *stradd(char *origin, char *addition);
int strfind(char *origin, char *substr);

int main(int argc, char *argv[]){
	printf("\nInitial Length: %d \n", strlen(argv[1]));
	char *res = stradd(argv[1], argv[2]);
	printf("new string: %s \n", res);
	printf("substring was found: ");	
	if(strfind(res,argv[3]) == 0){
		printf("no \n");
	}else{
		printf("yes \n");	
	}
		
	return 0;	
}
