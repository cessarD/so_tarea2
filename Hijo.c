#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




int main(int argc, char const *argv[])
{
	int flag, fd;  //filre descriptor
	char *nfifo= "tuberia";
	char *frase= "La gente positiva cambia el mundo, la negativa lo mantiene como está";
	flag=mkfifo(nfifo, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (!flag){
		printf("Hijo waiting\n");
		fd = open(nfifo, O_WRONLY);
			write(fd,frase, strlen(frase));
			printf("mensaje compartido\n");
			close(fd);
	}else{
		perror("Error con tuberia \n");
	}

	return 0;
}
