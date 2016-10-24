#include<stdio.h>
#include<stdlib.h>
#define BFSZ 1048576

int main(int argc, char **argv)
{
	if(argc != 3){
		printf("./binary_name <src> <dst>\n");
		exit(1);
	}	

	FILE *dst = fopen(argv[2], "wb");
	if(!dst){
		perror("fail when add new file.\n");
		exit(1);
	}

	FILE *src = fopen(argv[1], "rb");
	if(!src){
		perror("failed when open source file.\n");
		exit(1);
	}

	char buffer[BFSZ];
	int len = 0;

	while((len = fread(buffer, 1, BFSZ, src)) > 0)
		fwrite(buffer, 1, len, dst);
	
	fclose(dst);
	fclose(src);

	return 0;
}
