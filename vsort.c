#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
int main(int argc, char** argv){
	char exefile[strlen(argv[0])];		
	char  path[296];
	char* extp;
	char  ext[20];
	char* sortdir = "Sorted";
	short sortedflag = 0;
	short hiddenflag = 0;
	int option_index = 0;
	// make the "Sorted" directory
	if (argc > 1){
		if (strcmp(argv[1], "--help") == 0)
		{
			printf("Usage: %s [OPTIONS]\n-h\tDon't sort hidden files.\n-n \tDon't create a Sorted Directory.\n", argv[0]);
			return 0;
		}

		while (( option_index = getopt(argc, argv, "hn")) != -1){
			switch (option_index){
				case 'n':
					sortedflag = 1;
					break;
				case 'h':
					hiddenflag = 1;
					break;
				default:
				
					printf("Usage: %s [OPTIONS]\n see --help for more information.\n", argv[0]);
					return 0;
			}

		}
	}
	if(!sortedflag){	
		mkdir(sortdir, 0755);
	}
	memcpy(exefile, argv[0]+2, strlen(argv[0])-1);

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
				if(dir->d_name[0] == '.' && hiddenflag == 1);
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
				if(dir->d_name[0] == '.' && hiddenflag == 1);
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
