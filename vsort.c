#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>

void makedir(const char *name){
  #if __WIN32
  mkdir(name);
  #else
  mkdir(name, 0755);
  #endif
}
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
	char* Document[] = {"docx", "doc", "pdf", "odt", "odp", "xlsx", "txt", "json", "cfg", "config", "notebook"};
	char* Code[] = {"c", "cpp", "cs", "py", "js", "rb", "rs", "sh"};
	char* Shortcut[] = {"lnk", "desktop", "url"};
	char* Image[] = {"img", "iso", "dvd", "img"};
	char* Archive[] = {"zip", "rar", "7z", "tar", "gz", "bz2", "xz", "lz4"};
	char* Program[] = {"exe", "out", "app"};
	if (checkType(ext, Video, sizeof(Video) / sizeof(Video[0])) == 1)
		strcpy(type, "Videos");
	else if (checkType(ext, Audio, sizeof(Audio) / sizeof(Audio[0])) == 1)
		strcpy(type, "Audios");
	else if (checkType(ext, Photo, sizeof(Photo) / sizeof(Photo[0])) == 1)
		strcpy(type, "Photos");
	else if (checkType(ext, Document, sizeof(Document) / sizeof(Document[0])) == 1)
		strcpy(type, "Documents");
	else if (checkType(ext, Code, sizeof(Code) / sizeof(Code[0])) == 1)
		strcpy(type, "Code");
	else if (checkType(ext, Shortcut, sizeof(Shortcut) / sizeof(Shortcut[0])) == 1)
		strcpy(type, "Shortcuts");
	else if (checkType(ext, Image, sizeof(Image) / sizeof(Image[0])) == 1)
		strcpy(type, "Disc Images");
	else if (checkType(ext, Archive, sizeof(Archive) / sizeof(Archive[0])) == 1)
		strcpy(type, "Archives");
	else if (checkType(ext, Program, sizeof(Program) / sizeof(Program[0])) == 1)
		strcpy(type, "Programs");
	else{
		strcpy(type, "Undefined");
		return -1;
	}
	return 0;

}
int extsort(char* cwd, char* sortdir, char* ext, char* filename){
	char path[296];
	char dirpath[296];
	char filepath[296];
	sprintf(dirpath, "%s/%s%s", cwd, sortdir, ext);
	sprintf(path, "%s/%s%s/%s", cwd, sortdir, ext, filename);
	sprintf(filepath, "%s/%s", cwd, filename);
	makedir(dirpath);
	if((rename(filepath, path)) != 0)
		printf("Failed to Rename %s\n", filename);


}
int main(int argc, char** argv){
	//char  exefile[strlen(argv[0])];
	char  path[296];
	char  dirpath[296];
	char  type[20];
	char  ext[20];
	char* extp;
	char* sortdir = "Sorted/";

	// flags
	unsigned char sortflag = 0;
	unsigned char hiddenflag = 0;
	unsigned char advancedflag = 0;

	int option_index = 0;

	if (argc > 1){
		if (strcmp(argv[1], "--help") == 0)
		{
			printf("Usage: %s [OPTIONS]\n-s\tSort based on file extension\n-h\tDon't sort hidden files.\n-n \tDon't create a Sorted Directory.\n-a\tSort based on type.\n", argv[0]);
			return 0;
		}

		while (( option_index = getopt(argc, argv, "sanh")) != -1){
			switch (option_index){;
				case 'a':
					advancedflag = 1;
					break;
				case 's':
					sortflag = 1;
					break;
				case 'n':
					if (sortflag || advancedflag)
						sortdir = "";
					break;
				case 'h':
					if (sortflag || advancedflag)
						hiddenflag = 1;
					break;
				default:

					printf("Usage: %s [OPTIONS]\n see --help for more information.\n", argv[0]);
					return 0;
			}

		}
	}
	else{
		printf("Usage: %s [OPTIONS]\n see --help for more information.\n", argv[0]);
		return 0;
	}

	// make sorted dir
	makedir(sortdir);

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
                		char *exefile = &argv[0][strlen(argv[0]) - strlen(dir->d_name)];
				if(strcmp(dir->d_name, exefile) == 0);
				else if(dir->d_name[0] == '.' && hiddenflag == 1);
				else{
					extp = strrchr(dir->d_name + 1,'.');
					// seperate the "." from the filename
					// if it has no file extension make a "Undefined" folder
					if(extp == NULL){
						sprintf(dirpath, "%sUndefined", sortdir);
						sprintf(path, "%sUndefined/%s", sortdir, dir->d_name);
						makedir(dirpath);
						if((rename(dir->d_name, path)) != 0)
							printf("Failed to Rename %s\n", dir->d_name);

					}
					// otherwise make a folder for extension and move extension to folder
					else{
						// strrchr returns a pointer to the last period, not a string,
						// so to do pointer arithmetic with it, it must be a seperate string
						memcpy(ext, extp, sizeof(extp) + 1);
						memmove(ext, ext+1, strlen(ext));
						if (advancedflag){
							if(getType(ext, type) < 0)
								printf("file extension \"%s\" not defined\n", ext);

							sprintf(dirpath, "%s%s", sortdir, type);
							sprintf(path, "%s%s/%s", sortdir, type, dir->d_name);
							makedir(dirpath);
							if((rename(dir->d_name, path)) != 0)
								printf("Failed to Rename %s\n", dir->d_name);
							if (sortflag)
								extsort(dirpath, "", ext, dir->d_name);


						}
						else if (sortflag)
							extsort(".", sortdir, ext, dir->d_name);

					}


				}
			}

		}
		closedir(d);
	}

	return 0;
}
