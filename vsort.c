#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
int main(){
	char path[282];
	char* extp;
	char ext[6];
	char* sortdir = "Sorted";
	// make the "Sorted" directory
	if ((mkdir(sortdir, 0755)) != 0){
		printf("failed to make directory!\n");
		return 0;
	}

	// open the current working directory
	DIR *d;
	struct dirent *dir;
	d = opendir(".");

	// main loops
	if (d) {
		// loop for making folders
		while ((dir = readdir(d)) != NULL){
			// make sure its a regular file, no folders, symlinks, inodes, etc.
			if(dir->d_type == DT_REG){

				// seperate the "." from the filename
				// if it has no file extension make a "Undefined" folder
				if((extp = strrchr(dir->d_name,'.')) == NULL){
					printf("no EXT!\n");
					mkdir("Sorted/Undefined", 0755);
				}
				// otherwise make a folder for all of the extensions
				else{
					// strrchr returns a pointer to the last period, not a string,
					// so to do pointer arithmetic with it, it must be a seperate string
					memcpy(ext, extp, sizeof(extp) + 1);	
					memmove(ext, ext+1, strlen(ext));
					// make the folders inside the Sorted folder
					sprintf(path, "Sorted/%s", ext);
				}	mkdir(path, 0755);
			}
		}
		// go back to to the beginning of the directory pointer
		rewinddir(d);
		// loop for putting files in the folders
		while ((dir = readdir(d)) != NULL)
	       	{
			if (dir->d_type == DT_REG){
				// if there is no extension, put it in the "Undefined" folder
				if((extp = strrchr(dir->d_name,'.')) == NULL){	
					sprintf(path, "Sorted/Undefined/%s", dir->d_name);	
					rename(dir->d_name, path);
				}
				// otherwise put it in a folder matching it's extension
				else{
					memcpy(ext, extp, sizeof(extp) + 1);		
					memmove(ext, ext+1, strlen(ext));
					sprintf(path, "Sorted/%s/%s",ext, dir->d_name);
					if((rename(dir->d_name, path)) != 0)
						printf("failed rename!");
				}	
			}
	
					
		}
		closedir(d);
		
	}
	
	
	return 0;
}
