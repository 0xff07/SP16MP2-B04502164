#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#define BFSZ 1048576

char cur_name[10000];
const char* src = "server/";
const char* dst = "client/";

void cp(char *from, char *to)
{
	/* open destination. */
	FILE *dst = fopen(to, "wb");
	if(!dst){
		perror("fail when add new file.\n");
		exit(1);
	}

	/* open source. */
	FILE *src = fopen(from, "rb");
	if(!src){
		perror("failed when open source file.\n");
		exit(1);
	}

	/* Don't think, just copy it. */
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

	/* read every files in curent folder */
	while((cur_file = readdir(dir)) != NULL){
		char *file_name = cur_file->d_name;

		/* Skip if it's a hidden file */
		if(file_name[0] == '.')
			continue;
	
		/* Print current file then concat. It's easier to debug.*/
		printf("%s", cur_path);
		printf("%s\n", file_name);
		strcat(cur_path, cur_file->d_name);

		/* Produce the copy destnation */
		char copy_path[10000] = {0};
		strcat(copy_path, dst);
		strcat(copy_path, &cur_path[strlen(src)]);

		/* Process the files w.r.t. different file types. */
		if(cur_file->d_type == DT_DIR){

			/* Append "/" to make it look like a folder and easier to cd. */
			strcat(cur_path, "/");

			/* Make the same folder in destination path.*/
			if(mkdir(copy_path, ACCESSPERMS) == -1){
				perror("failed to copy foldir\n");
				exit(1);
			}

			/* After copying, cd to that directory. */
			DIR *next_folder = opendir(cur_path);
			if(!next_folder){
				perror("failed when opendir() next folder in tras_folder\n");
				exit(1);
			}
			
			/* and do it recursively. */
			tras_folder(next_folder, cur_path);


		}else if(cur_file -> d_type == DT_LNK){
			/* Copy path inside symbolic link to link_target. */
			char link_target[10000] = {0};
			if(readlink(cur_path, link_target, 10000) == -1){
				perror("failed to readlink().\n");
				exit(1);
			}

			/* then create a new one in destnation folder.*/
			if(symlink(link_target, copy_path) == -1){
				perror("failed to symlink().\n");
				exit(1);
			}
		}else{
			/* Otherwise it's just a normal file. Just cp it.*/
			cp(cur_path, copy_path);
		}
		/* Change path back to current folder.*/		
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
