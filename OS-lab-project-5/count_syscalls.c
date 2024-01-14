#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"

void itoa(int n, char *res){
	int ted = 0;
	int x = n;
	while (n > 0){
		ted ++;
		n /= 10;
	}
	int ted2 = 0;
	while (x > 0){
		ted2 ++;
		res[ted - ted2] = (x % 10) + '0';
		x /= 10;
	}
}

int main(int argc, char *argv[]){
	for (int i = 0; i < 10; i++){
		char num[4];
		itoa(i, num);
		int pid = fork();
		if (pid == 0){
			int fd = open("salam.txt", O_CREATE | O_WRONLY);
			if (fd < 0){
				printf(1, "Error while create file\n");
			}
			write(fd, "fahgopaeht9[0thgeah[aefhgs9r\n", 29);
			close(fd);
			exit();
		}
	}
	while (wait() != -1);
	count_syscalls();
	exit();
}