#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// main.c에서 사용하는 모든 ft_ 함수들을 extern으로 선언해줘야 해
extern int ft_atoi_base(const char *str, const char *base_str);

int main(void)
{
    printf("Testing the bonus libasm functions....\n");
    printf("---------------------------\n");
    printf("ft_atoi_base\n");

    const char *str = "10";

    const char *base_10 = "0123456789";
    printf("Input: \"%s\", Base: \"%s\" -> Result: %d (Expected: 10)\n", str, base_10, ft_atoi_base(str, base_10));

    const char *base_2 = "01";
    printf("Input: \"%s\", Base: \"%s\" -> Result: %d (Expected: 2)\n", str, base_2, ft_atoi_base(str, base_2));

    const char *base_8 = "01234567";
    printf("Input: \"%s\", Base: \"%s\" -> Result: %d (Expected: 8)\n", str, base_8, ft_atoi_base(str, base_8));

    const char *base_16 = "0123456789abcdef";
    printf("Input: \"%s\", Base: \"%s\" -> Result: %d (Expected: 16)\n", str, base_16, ft_atoi_base(str, base_16));

    const char *str2 = "  --+--+FF";
    const char *base_16_upper = "0123456789ABCDEF";
    printf("Input: \"%s\", Base: \"%s\" -> Result: %d (Expected: 255)\n", str2, base_16_upper, ft_atoi_base(str2, base_16_upper));

    printf("---------------------------\n");
    return 0;
}
