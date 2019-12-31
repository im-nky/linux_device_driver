#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
	int fd;
	char option;
	printf("Welcome to charcter device driver...\n");

	fd = open("/dev/my_device",O_RDWR);
	if(fd < 0) {
		printf("Can't open device file..\n");
		return 0;
	}
	while(1) {
		printf("**** Please Enter Your Option****\n");
		printf("1. read  \n");
		printf("2. write \n");
		printf("3. Exit  \n");
		scanf("%c",&option);
		switch(option) {
			case '1':
				printf("Enter the string into the driver\n");	
				scanf("%[^\t\n]s",write_buf);
				printf("Data Written....\n");
				write(fd,write_buf, strlen(write_buf)+1);
				printf("Done..\n");
			break;

			case '2':
                                printf("Data is Reading ....\n"); 
				read(fd,read_buf,1024);  
                                printf("Done..\n");
				printf("Data = %s \n", read_buf);
                        break;
			case '3':
                                close(fd);
				exit(1);
                        break;
			default:
                                printf("Enter Valid Option .\n");            
                        break;
		}
	}
	close(fd);
}
