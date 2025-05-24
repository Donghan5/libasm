#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int ft_atoi_base(const char *str, const char *str_base);

int main(void)
{
	printf("Testing the bonus libasm functions....\n");
	printf("---------------------------\n");
	printf("ft_atoi_base\n");
	const char *str = "10";
	const char *str_base = "10";
	printf("The result of ft_atoi_base is: %d\n", ft_atoi_base(str, str_base));
	const char *str_base1 = "2";
	printf("The result of ft_atoi_base is: %d\n", ft_atoi_base(str, str_base1));
	const char *str_base2 = "8";
	printf("The result of ft_atoi_base is: %d\n", ft_atoi_base(str, str_base2));
	const char *str_base3 = "16";
	printf("The result of ft_atoi_base is: %d\n", ft_atoi_base(str, str_base3));
	printf("---------------------------\n");
	return 0;
}
