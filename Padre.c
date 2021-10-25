#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




int main()
{
	int bytesCounter;
	char mensaje[100], *nfifo="tuberia";
	int fd;
	printf("Padre...\n");
	fd=open(nfifo, O_RDONLY);
	bytesCounter=read(fd,mensaje,100);
	mensaje[bytesCounter]='\0';
	printf("Mensaje compartido por el hijo: %s\n", mensaje );
	close(fd);
	unlink(nfifo);

}