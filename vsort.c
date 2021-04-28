#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
int main(){
	int rcheck;
	char path[282];
	char* extp;
	char ext[6];
	char* sortdir = "Sorted";
	int dirreturn = mkdir(sortdir, 0755);
	if (dirreturn != 0){
		printf("failed to make directory!\n");
		return 0;
	}
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL){
			if(dir->d_type == DT_REG){
				if((extp = strrchr(dir->d_name,'.')) == NULL){
					printf("no EXT!\n");
					mkdir("Sorted/Undefined", 0755);
				}
				else{
					memset(ext,0,sizeof(ext));
					strcat(ext, extp);
					memmove(ext, ext+1, strlen(ext));
					printf("%s\n",ext);
					sprintf(path, "Sorted/%s", ext);
				}	mkdir(path, 0755);	
			}
		}
		rewinddir(d);
		while ((dir = readdir(d)) != NULL)
	       	{
			if (dir->d_type == DT_REG){
				if((extp = strrchr(dir->d_name,'.')) == NULL){
					sprintf(path, "Sorted/Undefined/%s", dir->d_name);	
					rename(dir->d_name, path);
				}
				else{
					memset(ext,0,sizeof(ext));
					strcat(ext, extp);
					printf("%s\n", dir->d_name);
					memmove(ext, ext+1, strlen(ext));
					sprintf(path, "Sorted/%s/%s",ext, dir->d_name);
					printf("%s\n", path);		
					rcheck = rename(dir->d_name, path);
					if(rcheck != 0)
						printf("failed rename!");
				}	
			}
	
					
		}
		closedir(d);
		
	}
	
	
	return 0;
}
