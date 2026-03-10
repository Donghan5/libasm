#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_list {
    void *data;
    struct s_list *next;
}          t_list;

extern int ft_atoi_base(const char *str, const char *base_str);
extern int ft_list_push_front(void **begin_list, void *data);
extern int ft_list_size(void *begin_list);
extern void ft_list_sort(void **begin_list, int (*cmp)());
extern void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

/**
 * Print the contents of a linked list.
 */
void    ft_printf_list(t_list *list)
{
    while (list)
    {
        printf("%p -> ", list->data);
        list = list->next;
    }
}

void ft_list_clear(t_list **list) {
    t_list *tmp;

    while (*list) {
        tmp = (*list)->next;
        free(*list);
        *list = tmp;
    }
}

int int_cmp(void *a, void *b) {
    return (*(int *)a - *(int *)b);
}

void    dummy_free(void *data)
{
    (void)data; // Do nothing
}

int main(void)
{
    printf("Testing the bonus libasm functions....\n");
    printf("---------------------------\n");
    printf("<ft_atoi_base>\n");

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
    printf("<ft_list_push_front>\n");
    t_list *list = NULL;

    int value1 = 42;
    ft_list_push_front((void**)&list, &value1);
    printf("After pushing %d:\n", value1);
    ft_printf_list(list);
    printf(" (Expected: %p ->)\n", &value1);
    int value2 = 84;
    ft_list_push_front((void**)&list, &value2);
    printf("After pushing %d:\n", value2);
    ft_printf_list(list);
    printf(" (Expected: %p -> %p ->)\n", &value2, &value1);
    int value3 = 168;
    ft_list_push_front((void**)&list, &value3);
    printf("After pushing %d:\n", value3);
    ft_printf_list(list);
    printf(" (Expected: %p -> %p -> %p ->)\n", &value3, &value2, &value1);
    ft_list_clear(&list);

    printf("All ft_list_push_front function tests completed.\n");
    printf("---------------------------\n");

    printf("<ft_list_size>\n");
    t_list *size_list = NULL;
    ft_list_push_front((void**)&size_list, (void*)1);
    ft_list_push_front((void**)&size_list, (void*)2);
    ft_list_push_front((void**)&size_list, (void*)3);
    printf("List contents: ");
    ft_printf_list(size_list);
    printf("\n");
    int size = ft_list_size((void*)size_list);
    printf("List size: %d (Expected: 3)\n", size);
    ft_list_clear(&size_list);
    printf("All ft_list_size function tests completed (with ft_list_push_front).\n");
    printf("---------------------------\n");

 printf("---------------------------\n");
    printf("<ft_list_sort>\n");
    t_list *sort_list = NULL;
    int sort_data[] = {5, 1, 3, 4, 2};
    ft_list_push_front((void**)&sort_list, &sort_data[4]); 
    ft_list_push_front((void**)&sort_list, &sort_data[3]); 
    ft_list_push_front((void**)&sort_list, &sort_data[2]); 
    ft_list_push_front((void**)&sort_list, &sort_data[1]); 
    ft_list_push_front((void**)&sort_list, &sort_data[0]); 
    printf("Unsorted list: ");
    ft_printf_list(sort_list);
    printf("\n");
    ft_list_sort((void**)&sort_list, &int_cmp);
    printf("Sorted list: ");
    ft_printf_list(sort_list);
    printf("\n");
    ft_list_clear(&sort_list);
    printf("All ft_list_sort function tests completed (with ft_list_push_front).\n");

    printf("---------------------------\n");
    printf("<ft_list_remove_if>\n");
    t_list *remove_list = NULL;
    int remove_data[] = {1, 2, 3};
    int data_to_remove = 3;
    ft_list_push_front((void**)&remove_list, &remove_data[2]); 
    ft_list_push_front((void**)&remove_list, &remove_data[1]); 
    ft_list_push_front((void**)&remove_list, &remove_data[0]); 
    printf("Original list: ");
    ft_printf_list(remove_list);
    printf("\nRemoving all occurrences of 3:\n");
    ft_list_remove_if(&remove_list, &data_to_remove, &int_cmp, &dummy_free);
    printf("List after removal: ");
    ft_printf_list(remove_list);
    printf("\n (Expected: %p -> %p ->)\n", (void*)&remove_data[0], (void*)&remove_data[1]);
    ft_list_clear(&remove_list);
    printf("All ft_list_remove_if function tests completed (with ft_list_push_front).\n");
    printf("---------------------------\n");
    
    return 0;
}
