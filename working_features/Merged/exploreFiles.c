# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>

void fileName(char * Characters){
	struct dirent *lecture;
	int i = 0;
	DIR *dir;
	dir = opendir("CharFile");
	while ((lecture = readdir(dir))){
		if (lecture -> d_name[0] != '.'){
			Characters[i]= lecture->d_name[0];
			++i;
		}
	}
	closedir(dir);
}
/*
	int main(){
	char chr[52];
	fileName(chr);
	char *path = calloc(10 , sizeof(char));
	strcat(path, "CharFile/");
	for (int i = 0 ; i < 52 ; ++i){
	printf("%c ", chr[i]);
	}

	struct dirent *reading;
	int count = 0 ;
	for (int i = 0 ; i < 52 ; ++i){
	path[9] = chr[i];
	DIR* dir= opendir(path);
	if (dir){
	while((reading = readdir(dir))){
	if (reading->d_name[0] != '.'){
	}
	}
	++count;
	}
	closedir(dir);
	}
	free(path);
	}
 */
