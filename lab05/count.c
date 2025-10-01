/**
 * count.c
 * Sean Balbale
 * CPSC 275
 **/

#include <stdio.h>
#define MAXLEN 200

int numwords(char line[]);


int main(){
        char line[MAXLEN];
	numwords(line);

	return 0;
}

int numwords(char line[]){
	char c;
	
	int count = 0;
	int nChar = 0;
	int nWords = 0;
	int nLines = 0;
	c = getchar();
  	while(c != EOF){
		if(c == '\n'){
			nLines++;
			nChar++;
			nWords++;
			count = 0;
		}
		else if(c == ' '){
			nWords++;
		}
		nChar++;
		count++;
		c = getchar();
	}
	
	printf("%d %d %d \n",nLines, nWords, nChar);
	return 0;
}

