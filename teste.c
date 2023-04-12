#include <stdio.h>
#include <stdlib.h>

int arqExiste (int num1) {
	
	if ((num1 == 0x00) || (num1 == 0xe5)) {

		printf("passou 1\n");
		return 0;
	}	else {

		printf("passou 2\n");
		return 1; 
	}
}

int main(){

	int rd1 = 0xe5;
	printf("%x\n", rd1);
	int boolArq = arqExiste(rd1);
	printf("%d\n", boolArq);
	if(boolArq==1) {
		printf("Existe\n");
	}
}