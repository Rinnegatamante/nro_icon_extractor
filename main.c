#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static unsigned long long int fnv1a(const void *data, size_t size) {

    const unsigned char *const begin = data;
    const unsigned char *const end = begin + size;
    unsigned long long int result = 0xcbf29ce484222325;
    
    const unsigned char *p;
    for (p = begin; p != end; ++p) {
        result ^= *p;
        result *= 0x100000001b3;
    }

    return result;
}

int main(int argc,char** argv){
	
	if (argc != 2){
		printf("Usage: nro_icon_extractor FILE.nro\n");
		return 0;
	}
	
	FILE *input = fopen(argv[1],"rb");		
	if (input == NULL){
		printf("ERROR: File not found.\n");
		return;
	}
	
	unsigned long read_start = 0;
	fseek(input, 0, SEEK_END);
	unsigned long size = (unsigned long)ftell(input);
    char* data = (char*)malloc(size);
    fseek(input, 0, SEEK_SET);
    fread(data, 1, size, input);
    fclose(input);
	
	uint32_t *ptr = (uint32_t*)&data[0x10];
	if (*ptr != 0x304F524E){
		printf("ERROR: Invalid NRO file.\n");
		return;
	}
	
	ptr = (uint32_t*)&data[0x18];
	uint32_t nro_size = *ptr;
	ptr = (uint32_t*)&data[nro_size];
	if (*ptr != 0x54455341){
		printf("ERROR: Asset segment not found.\n");
		return;
	}
	
	ptr = (uint32_t*)&data[nro_size + 0x08];
	uint32_t icon_offs = *ptr;
	ptr = (uint32_t*)&data[nro_size + 0x10];
	uint32_t icon_size = *ptr;
	
	ptr = (uint32_t*)&data[nro_size + icon_offs];
	FILE *output = fopen("icon.jpg","wb+");
	fwrite(ptr, 1, icon_size, output);
	fclose(output);
	
	return 0;
	
}