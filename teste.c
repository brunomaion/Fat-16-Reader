#include <stdio.h>
#include <stdlib.h>

void retornaAtributo(int num1) {
	if (num1 == 0x20){
		printf("Arquivo\n");
	}
	if (num1 == 0x10){
		printf("Diretorio\n");
	}
	if (num1 == 0x0F){
		printf("LFN\n");
	}
}

int main(){
	int teste = 0x10;
	retornaAtributo(teste);

}