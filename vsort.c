#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
int main(int argc, char** argv){
	char exefile[strlen(argv[0])];		
	char  path[296];
	char* extp;
	char  ext[20];
	char* sortdir = "Sorted";
	int sortedflag = 0;
	// make the "Sorted" directory
	if (argc > 1){
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		{
			printf("Usage: %s [OPTIONS]\n-f | --force\tContinue if the directory exists.\n", argv[0]);
			return 0;
		}
		else if (strcmp(argv[1], "-f" ) == 0 || strcmp(argv[1], "--force") == 0)
		{		
			mkdir(sortdir, 0755);
			memcpy(exefile, argv[0]+2, strlen(argv[0])-1);
				
		}
		else if(strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--no-dir") == 0){
			sortedflag = 1;
		}
		else{
			printf("Usage: %s [OPTIONS]\n see -h or --help for more information.\n", argv[0]);
			return 0;
		}
	}
	else{
		if ((mkdir(sortdir, 0755)) != 0){
			printf("Directory exists or insufficient permissions.\nUse -f or --force to use existing directory.\n");
			return 0;
		}	
		memcpy(exefile, argv[0]+2, strlen(argv[0])-1);
		
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
				if(strcmp(dir->d_name, exefile) == 0);
				else{	
					// seperate the "." from the filename
					// if it has no file extension make a "Undefined" folder
					if((extp = strrchr(dir->d_name + 1,'.')) == NULL){
							if(!sortedflag)
							mkdir("Sorted/Undefined", 0755);
						else
							mkdir("Undefined", 0755);
					}
				
					// otherwise make a folder for all of the extensions
					else{
						// strrchr returns a pointer to the last period, not a string,
						// so to do pointer arithmetic with it, it must be a seperate string
						memcpy(ext, extp, sizeof(extp) + 1);	
						memmove(ext, ext+1, strlen(ext));
						// make the folders depending on the sortedflag
						if(!sortedflag){
							sprintf(path, "Sorted/%s", ext);
							mkdir(path, 0755);
						}
						else
							mkdir(ext, 0755);	
					}
				}
			}
		}
		// go back to to the beginning of the directory pointer
		rewinddir(d);
		// loop for putting files in the folders
		while ((dir = readdir(d)) != NULL)
	       	{
			if (dir->d_type == DT_REG){
					
				if(strcmp(dir->d_name, exefile) == 0);
				else{
				
					// if there is no extension, put it in the "Undefined" folder
					if((extp = strrchr(dir->d_name +1,'.')) == NULL){
						if(!sortedflag)
							sprintf(path, "Sorted/Undefined/%s", dir->d_name);	
					
						else
							sprintf(path, "Undefined/%s", dir->d_name);

						if((rename(dir->d_name, path)) != 0)
							printf("Failed to Rename %s\n", dir->d_name);

					}
					else if(strcmp(dir->d_name, exefile) == 0);
					// otherwise put it in a folder matching it's extension
					else{
						memcpy(ext, extp, sizeof(extp) + 1);		
						memmove(ext, ext+1, strlen(ext));
						if(!sortedflag)
							sprintf(path, "Sorted/%s/%s",ext, dir->d_name);
						else
							sprintf(path, "%s/%s",ext, dir->d_name);

						if((rename(dir->d_name, path)) != 0)
							printf("Failed to Rename %s\n", dir->d_name);
					}
				}	
			}
	
					
		}
		closedir(d);
		
	}
	
	
	return 0;
}
