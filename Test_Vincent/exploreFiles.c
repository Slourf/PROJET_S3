# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <dirent.h>

void fileName(int charNumber, char **Characters){
	struct dirent *lecture;
	int i = 0;
	DIR *dir;
	dir = opendir("CharFile");
	while (lecture = readdir(dir)){
		if (lecture -> d_name[0] != '.'){
			Characters[i] = lecture->d_name;
			++i;
		}
	}
	closedir(dir);
}
