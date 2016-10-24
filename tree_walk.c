#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#define HOMEWORK ON
#define BFSZ 1048576

char cur_name[10000];
const char* src = "server/";
const char* dst = "client/";

void cp(char *from, char *to)
{
	FILE *dst = fopen(to, "wb");
	if(!dst){
		perror("fail when add new file.\n");
		exit(1);
	}

	FILE *src = fopen(from, "rb");
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
}


void tras_folder(DIR *dir, char *cur_path)
{
	struct dirent *cur_file;
	int cur_path_len = strlen(cur_path);
	while((cur_file = readdir(dir)) != NULL){
		char *file_name = cur_file->d_name;

		#ifdef HOMEWORK
		#define HOMEWORK_EXT
		if(file_name[0] == '.')
			continue;
		#endif
	
		printf("%s", cur_path);
		printf("%s\n", file_name);
		strcat(cur_path, cur_file->d_name);
		//strcat(cur_path, "/");

		char copy_path[10000] = {0};
		strcat(copy_path, dst);
		strcat(copy_path, &cur_path[strlen(src)]);

		//printf("to be copied : %s\n", copy_path);

		if(cur_file->d_type == DT_DIR){
			strcat(cur_path, "/");
			if(mkdir(copy_path, ACCESSPERMS) == -1){
				perror("failed to copy foldir\n");
				exit(1);
			}

			DIR *next_folder = opendir(cur_path);
			if(!next_folder){
				perror("failed when opendir() next folder in tras_folder\n");
				exit(1);
			}
			tras_folder(next_folder, cur_path);

		}else if(cur_file -> d_type == DT_LNK){
			/* fill how to handle symbolic link here */	
		}else
			cp(cur_path, copy_path);
		
		cur_path[cur_path_len] = '\0';
	}
}

int main(int argc, char **argv)
{

	/*
	if(argc != 2){
		printf("./trasverse_folder <folder_path>\n");
		exit(0);
	}
	*/


	char path[100000];
	strcpy(path, src);
	int path_len = strlen(src);
	if(path[path_len - 1] != '/')
		strcat(path, "/");
	path_len++;


	DIR *dir;
	struct dirent *folder;

	if((dir = opendir(path)) == NULL){
		perror("faile when opendir()\n");
		exit(1);
	}

	tras_folder(dir, path);

	return 0;
}
