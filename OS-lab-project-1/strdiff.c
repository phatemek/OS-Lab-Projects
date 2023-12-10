#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


void 
str_diff(char *a, char *b){
	
}

int
main(int argc, char *argv[]){
	unlink("strdiff_result.txt");
	int fd = open("strdiff_result.txt", O_CREATE | O_RDWR);
	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);
	int mx_len = len1;
	if (len2 > len1) mx_len = len2;
	char res[mx_len];
	memset(res, '\0',sizeof(res));
	for (int i = 0; i < mx_len; i++){
		if (i <= len1 - 1 && i <= len2 - 1){
			if (argv[1][i] >= argv[2][i]){
				res[i] = '0';
			}else res[i] = '1';
		}
		else if (i <= len1 - 1){
			res[i] = '0';
		}else{
			res[i] = '1';
		}
	}
	write(fd, res, mx_len);
	write(fd, (char*)"\n", 1);
	close(fd);
	exit();
}