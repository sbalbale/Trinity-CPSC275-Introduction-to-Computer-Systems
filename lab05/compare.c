/**
 * compare.c
 * Sean Balbale
 * CPSC 275
 **/

#include <stdio.h>

#define MAXLEN 100

int mystrcmp(char *s1, char *s2);



int main(){
	char *s1;
	char *s2;
	int compare;

	scanf("%s %s",s1,s2);
	
	compare = mystrcmp(s1,s2);

	printf("%s %s %d \n",s1,s2,compare);
}

int mystrcmp(char *s1, char *s2){
	if(*s1 == *s2){
		if(*s1 == '\0' || *s2 == '\0'){
			return 0;
		}
		else{
			return mystrcmp(s1+1,s2+1);
		}
	}
	return -1;
}



