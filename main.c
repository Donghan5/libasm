#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern size_t ft_strlen(const char *str);
extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);


int main(void)
{
	const char *str = "Hello, World!";
	char buffer[100];
	long read;
	long read_bytes = 0;

	size_t length = ft_strlen(str);
	printf("Testing the mandatory libasm functions....\n");
	printf("---------------------------\n");
	printf("1. ft_strlen\n");
	printf("The length of the string is: %zu\n", length);
	printf("---------------------------\n");
	printf("2. ft_write\n");
	ft_write(1, "Hello, World!\n", 14);
	printf("---------------------------\n");
	printf("3. ft_read (reading Makefile)\n");
	read = ft_read(STDIN_FILENO, buffer, 100);
	while (read > 0)
	{
		buffer[read] = '\0';
		printf("Content read: %s", buffer);
		read = ft_read(STDIN_FILENO, buffer, 100);
		read_bytes += read;
	}
	printf("Read %ld bytes total\n", read_bytes);
	printf("---------------------------\n");

	return 0;
}
