#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

// cross platform mkdir function because
// Windows doesn't use the same function args
void makedir(const char *name){
 	#if __WIN32
 	mkdir(name);
 	#else
 	mkdir(name, 0755);
 	#endif
}
// function with a for loop to check if the ext resides in the array passed
unsigned char checkType(char* ext, char** Type, size_t sizeofType){
	for(unsigned short i = 0; i < sizeofType; i++){
			if (strcmp(ext, Type[i]) == 0)
				return 1;
	}
	return 0;
}

// inefficient function to figure out what type the file is
int getType(char* ext, char* type){

	// array for advanced sorting
	char* Video[] = {"mp4", "mkv", "avi", "ogg", "flv", "mov"};
	char* Audio[] = {"mp3", "wma", "m4a", "pcm", "wav", "aiff", "aac", "ogg", "flac", "alac", "acm"};
	char* Photo[] = {"png", "jpg", "jpeg", "mpo", "gif", "bmp, ico"};
	char* Document[] = {"docx", "doc", "pdf", "odt", "odp", "xlsx", "txt", "json", "cfg", "config", "notebook", "bak"};
	char* Code[] = {"c", "cpp", "cs", "py", "js", "rb", "rs", "sh", "htm", "html" "css"};
	char* Shortcut[] = {"lnk", "desktop", "url"};
	char* Image[] = {"img", "iso", "dvd", "img"};
	char* Archive[] = {"zip", "rar", "7z", "tar", "gz", "bz2", "xz", "lz4"};
	char* Program[] = {"exe", "out", "app"};
	char* System[] = {"ini", "dll", "dat", "inf", "drv", "sys", "bin", "dmp"};

	// bad if/else block for checking type, I am still thinking of a solution
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
	else if (checkType(ext, System, sizeof(System) / sizeof(System[0])) == 1)
		strcpy(type, "System Files");
	else{
		strcpy(type, "Undefined");
		return -1;
	}
	return 0;

}
// simple string tolower function
void toLower(char* string){
	for(int i = 0; string[i]; i++)
  		string[i] = tolower(string[i]);

}

// function to sort files based on extension
int extsort(char* cwd, char* sortdir, char* ext, char* filename){
	char path[296];
	char dirpath[296];
	char filepath[296];

	// create string which represents path to create dirs
	sprintf(dirpath, "%s/%s%s", cwd, sortdir, ext);

	// create string which represents path to put file
	sprintf(path, "%s/%s%s/%s", cwd, sortdir, ext, filename);

	// create a string which represents the original file path before moving
	sprintf(filepath, "%s/%s", cwd, filename);
	makedir(dirpath);
	if((rename(filepath, path)) != 0)
		printf("Failed to Rename %s\n", filename);


}

// function to print help menu since it will be used many times
void help(char* name){
	printf("Usage: %s [OPTIONS]\n see --help for more information.\n", name);
}

int main(int argc, char** argv){
	// set up arrays/strings
	char  path[296];
	char  dirpath[296];
	char  type[20];
	char  ext[296];	
	char* sortdir = "Sorted/";

	// flags
	unsigned char sortflag = 0;
	unsigned char hiddenflag = 0;
	unsigned char advancedflag = 0;

	// get and deal with command line arguments
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
					else{
						help(argv[0]);
						return 0;
					}
					break;
				case 'h':
					if (sortflag || advancedflag)
						hiddenflag = 1;
					else{
						help(argv[0]);
						return 0;
					}
					break;
				default:

					help(argv[0]);
					return 0;
			}

		}
	}
	else{
		help(argv[0]);
		return 0;
	}

	// make sorted dir (or not depending on the sortdirflag)
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

				// confusing line that basically takes the last x amount letters from argv
				// x = the amount of letters in the current file
				// doing this allows the ability to check if the current file is equal to
				// the programs name, this check is so it doesn't copy it's self and pass over it
				// the reason it needs to be done like this is because Windows stores the full path in argv[0]
                		char *exefile = &argv[0][strlen(argv[0]) - strlen(dir->d_name)];
				if(strcmp(dir->d_name, exefile) == 0);

				// check if the file starts with a dot (meaning its hidden on linux)
				// and if it's set not to copy hidden files
				// if both of those are true, skip the file
				// (still working for a way that also works on windows)
				else if(dir->d_name[0] == '.' && hiddenflag == 1);
				else{
					// seperate the "." from the filename
					// to single out the extenstion
					char *extp = strrchr(dir->d_name + 1,'.');

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
						memmove(ext, ext+1, sizeof(ext));

						// make the ext lowercase before we check it
						// this doesn't effect the file but makes it
						// where we don't think .EXE and .exe are two different files
						toLower(ext);

						// if advanced sort is requested
						if (advancedflag){

							// if extenstion doesn't match any known type
							if(getType(ext, type) < 0)
								printf("file extension \"%s\" not defined\n", ext);
					
							sprintf(dirpath, "%s%s", sortdir, type);
							sprintf(path, "%s%s/%s", sortdir, type, dir->d_name);
							makedir(dirpath);
							if((rename(dir->d_name, path)) != 0)
								printf("Failed to Rename %s\n", dir->d_name);

							// if ext sorting is requested ALONG with advanced sort
							// it will first sort it based on type then extension inside each folder
							if (sortflag)
								extsort(dirpath, "", ext, dir->d_name);


						}
						// if ext sorting is requested
						else if (sortflag)
							extsort(".", sortdir, ext, dir->d_name);

					}
				}
			}
		}

		// finished all files, close the dir pointer and exit the program
		closedir(d);
	}
	return 0;
}
