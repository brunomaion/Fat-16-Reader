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

int main()
{

    FILE *fp;
    fat_BS_t  boot_record; //declaração

    fp= fopen("fat161s.img", "rb");
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
	int fat1_start_sector = boot_record.reserved_sector_count;
	int fat1_start_byte = fat1_start_sector * boot_record.bytes_per_sector;
	int fat2_start_sector = boot_record.reserved_sector_count + boot_record.table_size_16;
	int fat2_start_byte = fat2_start_sector * boot_record.bytes_per_sector;

	//ROOT DIR
	int root_dir_start_sector = boot_record.reserved_sector_count + boot_record.table_size_16 * boot_record.table_count;
	int root_dir_start_byte = root_dir_start_sector * boot_record.bytes_per_sector;

	int root_dir_size_bytes = boot_record.root_entry_count * 32; // cada entrada no diretório raiz tem tamanho fixo de 32 bytes
	int root_dir_size_sectors = (root_dir_size_bytes + boot_record.bytes_per_sector - 1) / boot_record.bytes_per_sector; // arredondamento para cima

	//DATA
	int data_sector_offset = boot_record.reserved_sector_count + (boot_record.table_count * boot_record.table_size_16);
	int data_start_sector = root_dir_start_sector + root_dir_size_sectors;
	int data_start_byte = data_start_sector * boot_record.bytes_per_sector;


	printf("Infos BOOR RECORD:\n");
	printf(" Bytes por setor: %hd \n", boot_record.bytes_per_sector);
    printf(" Setores por cluster: %x \n", boot_record.sectors_per_cluster);
	printf(" Numero setores reservados: %d \n", boot_record.reserved_sector_count); //BR 11 0x0B
	printf(" Numero tabelas fat: %d \n", boot_record.table_count); //BR 16 0x10
	printf(" Numero entradas diretorio: %d \n", boot_record.root_entry_count); //BR 17 0x11
	printf(" Numero setores por fat: %d \n", boot_record.table_size_16); //BR 22 0x16
	printf(" Numero setores per track: %d \n", boot_record.sectors_per_track); //BR 24 0x18

	printf("\nInfos FAT:\n");
	printf(" Fat1, Setor: %d \n", fat1_start_sector); //SETOR INICIA FAT 2
	printf(" Fat1, Byte: %d \n", fat1_start_byte); //BYTE INICIA FAT 2
	printf(" Fat2, Setor: %d \n", fat2_start_sector); //SETOR INICIA FAT 2
	printf(" Fat2, Byte: %d \n", fat2_start_byte); //BYTE INICIA FAT 2


	printf("\nInfos ROOT DIR:\n");
	printf(" Root Dir, Setor: %d \n", root_dir_start_sector); //SETOR INICIA Root Dir 
	printf(" Root Dir, Byte: %d \n", root_dir_start_byte); //BYTE INICIA Root Dir 
	printf(" Root Dir, Tamanho Bytes: %d \n", root_dir_size_bytes); //Tamanho Root Dir 
	printf(" Root Dir, Tamanho Setor: %d \n", root_dir_size_sectors); //Tamanho Root Dir 

	printf("\nInfos DATA:\n");
	printf(" Data, Offset: %d \n", data_sector_offset); //Tamanho Root Dir 
	printf(" Data, Setor: %d \n", data_start_sector); //SETOR INICIA Root Dir 
	printf(" Data, Byte: %d \n", data_start_byte); //BYTE INICIA Root Dir 


	//printf("\n");
    return 0;
}
