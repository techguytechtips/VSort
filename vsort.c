#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
unsigned char checkType(char* ext, char** Type, size_t sizeofType){
	for(unsigned short i = 0; i < sizeofType; i++){
			if (strcmp(ext, Type[i]) == 0)	
				return 1;
	}
	return 0;
}
int getType(char* ext, char* type){

	// array for advanced sorting
	char* Video[] = {"mp4", "mkv", "avi", "ogg", "flv", "mov"};
	char* Audio[] = {"mp3", "wma", "m4a", "pcm", "wav", "aiff", "aac", "ogg", "flac", "alac"};
	char* Photo[] = {"png", "jpg", "jpeg", "mpo"};
	char* Document[] = {"docx", "doc", "pdf", "odt", "odp", "xlsx", "txt", "json", "cfg", "config", "notebook", "c", "cpp", "cs", "py", "js", "rb", "out"};
	char* Shortcut[] = {"lnk", "desktop", "url"};
	char* Image[] = {"img", "iso", "dvd"};
	if (checkType(ext, Video, sizeof(Video) / sizeof(Video[0])) == 1)
		strcpy(type, "Videos");
	else if (checkType(ext, Audio, sizeof(Audio) / sizeof(Audio[0])) == 1)
		strcpy(type, "Audios");
	else if (checkType(ext, Photo, sizeof(Photo) / sizeof(Photo[0])) == 1)
		strcpy(type, "Photos");
	else if (checkType(ext, Document, sizeof(Document) / sizeof(Document[0])) == 1)
		strcpy(type, "Documents");
	else if (checkType(ext, Shortcut, sizeof(Shortcut) / sizeof(Shortcut[0])) == 1)
		strcpy(type, "Shortcuts");
	else if (checkType(ext, Image, sizeof(Image) / sizeof(Image[0])) == 1)
		strcpy(type, "Disc Images");
	else{
		strcpy(type, "Undefined");
		return -1;
	}
	return 0;

}

int main(int argc, char** argv){
	char exefile[strlen(argv[0])];		
	char  path[296];
	char dirpath[296];
	char* extp;
	char  ext[20];
	char* sortdir = "Sorted";
	char type[20];

	// flags
	unsigned char sortedflag = 0;
	unsigned char hiddenflag = 0;
	unsigned char advancedflag = 0;

	int option_index = 0;

	// make the "Sorted" directory
	if (argc > 1){
		if (strcmp(argv[1], "--help") == 0)
		{
			printf("Usage: %s [OPTIONS]\n-h\tDon't sort hidden files.\n-n \tDon't create a Sorted Directory.\n-a\tAdvanced Sorting, see Github for more info.\n", argv[0]);
			return 0;
		}

		while (( option_index = getopt(argc, argv, "hna")) != -1){
			switch (option_index){
				case 'n':
					sortedflag = 1;
					break;
				case 'h':
					hiddenflag = 1;
					break;
				case 'a':
					advancedflag = 1;
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
				else if(dir->d_name[0] == '.' && hiddenflag == 1);
				else{	
					extp = strrchr(dir->d_name + 1,'.');
					// seperate the "." from the filename
					// if it has no file extension make a "Undefined" folder
					if(extp == NULL){
						if(!sortedflag){
							mkdir("Sorted/Undefined", 0755);
							sprintf(path, "Sorted/Undefined/%s", dir->d_name);
						}
						else{
							mkdir("Undefined", 0755);
							sprintf(path, "Undefined/%s", dir->d_name);	
						}
						if((rename(dir->d_name, path)) != 0)
							printf("Failed to Rename %s\n", dir->d_name);

					}
					// otherwise make a folder for all of the extensions
					else{
						// strrchr returns a pointer to the last period, not a string,
						// so to do pointer arithmetic with it, it must be a seperate string
						memcpy(ext, extp, sizeof(extp) + 1);	
						memmove(ext, ext+1, strlen(ext));
						// make the folders depending on the sortedflag
						if(!sortedflag){
							if (advancedflag){
								if(getType(ext, type) < 0)
									printf("file extension \"%s\" not defined\n", ext);

								sprintf(dirpath, "Sorted/%s", type);
								sprintf(path, "Sorted/%s/%s", type, dir->d_name);
							}
							else{
								sprintf(dirpath, "Sorted/%s", ext);
								sprintf(path, "Sorted/%s/%s",ext, dir->d_name);
							}

						
						}	
						else
							if (advancedflag){
								getType(ext, type);
								strcpy(dirpath, type);
								sprintf(path, "%s/%s", type, dir->d_name);
							}
							else{
								strcpy(dirpath, ext);
								sprintf(path, "%s/%s",ext, dir->d_name);
							}
						mkdir(dirpath, 0755);
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
