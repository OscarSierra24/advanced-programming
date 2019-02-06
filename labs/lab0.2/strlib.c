#include <stdio.h>
#include <stdlib.h>

int strlen(char *str){
	int i = 0;
	while(str[i]!='\0'){
		i++;
	}
	return i;	
}

char *stradd(char *origin, char *addition){
	int lenOrigin = strlen(origin);
	int lenAddition = strlen(addition);
	
	char *result = (char *)malloc(lenOrigin+lenAddition+1);
	
	int i = 0; 
	for(;i<lenOrigin;i++){
		result[i] = origin[i];
	}
	for(; i<lenOrigin+lenAddition;i++){
		result[i] = addition[i-lenOrigin];
	}

	result[i] = '\0';

	return result;
}

int strfind(char *origin, char *substr){
	int j = 0;
	int substrSize = strlen(substr);

	for(int i = 0; i<strlen(origin);i++){
		if(j < substrSize){
			if(origin[i] == substr[j]){
				j++;
			}else{
				j=0;
			}		
		}else if(j == substrSize){
			return 1;
		}
	}
	if(j==substrSize){
		return 1;
	}	
	return 0;
}

