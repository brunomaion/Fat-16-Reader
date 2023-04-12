#include <stdio.h>
#include <stdlib.h>

typedef struct fat_BS
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	    bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char		extended_section[54];
 
}__attribute__((packed)) fat_BS_t;

typedef struct rootDicionarioExemplo {
    char nomeEntrada[8]; //0 0x00
    char extensaoEntrada[3]; //8 0x08
    char atributo; // 11 0x0B
    char reservado1; // 12 0x0C
    char criacaoMilissegundos; // 13 0X0D
    short horaCriacao; // 14 0X0E
    short dataCriacao; // 16 0X10
    short dataUltimoAcesso; // 18 0X12
    short reservado2; // 20 0X14
    short horaUltimaGravacao; // 22 0X16
    short dataUltimaGravacao; // 24 0X18
    short enderecoClusterInicial; // 26 0X1A
    int dimensaoBytes; // 28 0X1C
}__attribute__((packed)) rootDicionarioExemplo;;

int arqExiste (int num1) {
	
	if ((num1 == 0x00) || (num1 == 0xe5)) {
		return 0;
	}	else {
		return 1; 
	}
}


int statusCluster (int num1) {

	if (num1 = 0x0000) { //0x0000 	Cluster livre
		return 1;
	}
	if (num1 = (0x001 || 0x002 )) { //0x001 e 0x002 	Valores não utilizados e não permitidos
		return 2;
	}
	if (num1 = 0xFFF7) { //0xFFF7 	Cluster danificado
		return 3;
	}
	if ((num1 >= 0x0003) && (num1 <= 0xFFEF)) { //0x0003 a 0xFFEF 	O cluster faz parte de um ficheiro; o valor é o endereço do cluster seguinte
		return 4;
	}
	if ((num1 >= 0xFFF0) && (num1 <= 0xFFF6)) { //0xFFF0 a 0xFFF6 	Cluster reservado
		return 5;
	}
	if ((num1 >= 0xFFF8) && (num1 <= 0xFFFF)) { //0xFFF8 a 0xFFFF 	Último cluster de um ficheiro
		return 6;
	}

}

void retornaAtributo(int num1) {
	if (num1 == 0x20){
		printf("Arquivo");
	}
	if (num1 == 0x10){
		printf("Diretorio");
	}
	if (num1 == 0x0F){
		printf("Lfn");
	}
}

int main()
{

    FILE *fp;
    fat_BS_t  boot_record; //declaração
	
	rootDicionarioExemplo rd; //

    fp= fopen("fat164s.img", "rb");
    fseek(fp, 0, SEEK_SET);
    fread(&boot_record, sizeof(fat_BS_t),1, fp);
	
	
	//BOOT RECORD
	// move o ponteiro de arquivo para o offset 16
	//BR 11 0x0B
    fseek(fp, 14, SEEK_SET);
    fread(&boot_record.reserved_sector_count, sizeof(unsigned short), 1, fp);

	//BR 16 0x10
    fseek(fp, 16, SEEK_SET);
    fread(&boot_record.table_count, sizeof(unsigned short), 1, fp);

	//BR 17 0x11
    fseek(fp, 17, SEEK_SET);
    fread(&boot_record.root_entry_count, sizeof(unsigned short), 1, fp);

	//BR 22 0x16
    fseek(fp, 22, SEEK_SET);
    fread(&boot_record.table_size_16, sizeof(unsigned short), 1, fp);

	//BR 24 0x18
    fseek(fp, 24, SEEK_SET);
    fread(&boot_record.sectors_per_track, sizeof(unsigned short), 1, fp);

	//FATs
	int fat_sizeByte = boot_record.table_size_16 * boot_record.root_entry_count;
	int fat1_start_sector = boot_record.reserved_sector_count;
	int fat1_start_byte = fat1_start_sector * boot_record.bytes_per_sector;
	int fat2_start_sector = boot_record.reserved_sector_count + boot_record.table_size_16;
	int fat2_start_byte = fat2_start_sector * boot_record.bytes_per_sector;

	//ROOT DIR
	int root_dir_start_sector = boot_record.reserved_sector_count + boot_record.table_size_16 * boot_record.table_count;
	int root_dir_start_byte = root_dir_start_sector * boot_record.bytes_per_sector;
	int root_dir_size_bytes = boot_record.root_entry_count * 32; // cada entrada no diretório raiz tem tamanho fixo de 32 bytes
	int root_dir_size_sectors = (root_dir_size_bytes + boot_record.bytes_per_sector - 1) / boot_record.bytes_per_sector; // arredondamento para cima

	int entradasTotais = root_dir_size_bytes/32; // !! DUVIDA !! Qual tamanho do ROOT DIR MESMO, esta certo? Pq nao acho a DATA correta


	//DATA
	int data_start_sector = root_dir_start_sector + root_dir_size_sectors;
	int data_start_byte = data_start_sector * boot_record.bytes_per_sector; //SETOR INICIAL  DATA * 

	int inicialRd11 =11;
	int inicialRd0 =0;

	for (int i = 0; i < entradasTotais; i++)
	{
		//le primeiro elemento 
		short rd1;
		fseek(fp, (root_dir_start_byte+inicialRd0), SEEK_SET);
    	fread(&rd1, sizeof(char), 1, fp);

		if(arqExiste(rd1)==1) { //CHECK SE O ARQUIVO EXISTE E NAO FOI EXCLUIDO
			
			fseek(fp, (root_dir_start_byte+inicialRd11), SEEK_SET);
    		fread(&rd.atributo, sizeof(unsigned short), 1, fp);
			retornaAtributo(rd.atributo);
			printf(": ");

			fseek(fp, (root_dir_start_byte+inicialRd0), SEEK_SET);
			fread(&rd.nomeEntrada, sizeof(char), 8, fp);

			printf("%s", rd.nomeEntrada);
			printf("\n");
		}

		inicialRd0 += 32;
		inicialRd11 += 32;

	}


	for (int i = 0; i < entradasTotais; i++)
	{
		

		
		
		
		
		//
		/*
		fseek(fp, (root_dir_start_byte), SEEK_SET);
    	fread(&rd.nomeEntrada, sizeof(char), 8, fp);
		printf("%s\n", rd.nomeEntrada);
		// 

		//VERIFICAR ATRIBUTOS
		fseek(fp, (root_dir_start_byte+inicialRd11), SEEK_SET);
    	fread(&rd.atributo, sizeof(unsigned short), 1, fp);
		retornaAtributo(rd.atributo);
		inicialRd11 += 32;
		//
		*/
	}
	

	// 
	/*
	printf("Infos BOOR RECORD:\n");
	printf(" Bytes por setor: %hd \n", boot_record.bytes_per_sector);
    printf(" Setores por cluster: %x \n", boot_record.sectors_per_cluster);
	printf(" Numero setores reservados: %d \n", boot_record.reserved_sector_count); //BR 11 0x0B
	printf(" Numero tabelas fat: %d \n", boot_record.table_count); //BR 16 0x10
	printf(" Numero entradas diretorio: %d \n", boot_record.root_entry_count); //BR 17 0x11
	printf(" Numero setores por fat: %d \n", boot_record.table_size_16); //BR 22 0x16
	printf(" Numero setores per track: %d \n", boot_record.sectors_per_track); //BR 24 0x18

	printf("\nInfos FAT:\n");
	printf(" Tamanho: %d \n", fat_sizeByte); //tamanho da fat
	printf(" Fat1, Setor: %d \n", fat1_start_sector); //SETOR INICIA FAT 2
	printf(" Fat1, Byte: %d \n", fat1_start_byte); //BYTE INICIA FAT 2
	printf(" Fat2, Setor: %d \n", fat2_start_sector); //SETOR INICIA FAT 2
	printf(" Fat2, Byte: %d \n", fat2_start_byte); //BYTE INICIA FAT 2


	printf("\nInfos ROOT DIR:\n"); 
	printf(" Root Dir, Setor inicial: %d \n", root_dir_start_sector); //BYTE INICIA Root Dir 
	printf(" Root Dir, Byte: %d \n", root_dir_start_byte); //BYTE INICIA Root Dir 
	printf(" Root Dir, Tamanho Bytes: %d \n", root_dir_size_bytes); //Tamanho Root Dir bytes
	printf(" Root Dir, Tamanho Setor: %d \n", root_dir_size_sectors); //Tamanho Root Dir setores
	printf(" Root Dir, Entradas totais: %d \n", entradasTotais); //Tamanho Root Dir setores        // !! DUVIDA !!




	printf("\nInfos DATA:\n");
	printf(" Data, Setor: %d \n", data_start_sector); //SETOR INICIA Root Dir 
	printf(" Data, Byte: %d \n", data_start_byte); //BYTE INICIA Root Dir 
	//
	 */

	//printf("\n");
    return 0;
}
