#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_list {
	void          *data;
	struct s_list *next;
} t_list;

extern int   ft_atoi_base(const char *str, const char *base_str);
extern int   ft_list_push_front(void **begin_list, void *data);
extern int   ft_list_size(void *begin_list);
extern void  ft_list_sort(void **begin_list, int (*cmp)(void *, void *));
extern void  ft_list_remove_if(t_list **begin_list, void *data_ref,
                               int (*cmp)(void *, void *),
                               void (*free_fct)(void *));

static int g_passed = 0;
static int g_failed = 0;

static void check(const char *name, int passed)
{
	if (passed)
	{
		printf("  [PASS] %s\n", name);
		g_passed++;
	}
	else
	{
		printf("  [FAIL] %s\n", name);
		g_failed++;
	}
}

/* ── helpers ─────────────────────────────────────────────────── */

static void ft_list_clear(t_list **list)
{
	t_list *tmp;
	while (*list)
	{
		tmp = (*list)->next;
		free(*list);
		*list = tmp;
	}
}

static int int_cmp(void *a, void *b)
{
	return (*(int *)a - *(int *)b);
}

static void dummy_free(void *data)
{
	(void)data; /* data lives on the stack, do not free it */
}

static int list_is_sorted(t_list *list)
{
	while (list && list->next)
	{
		if (int_cmp(list->data, list->next->data) > 0)
			return 0;
		list = list->next;
	}
	return 1;
}

static int list_contains_val(t_list *list, int val)
{
	while (list)
	{
		if (*(int *)list->data == val)
			return 1;
		list = list->next;
	}
	return 0;
}

/* ================================================================
 * ft_atoi_base
 * ================================================================ */
static void test_atoi_base(void)
{
	printf("\n=== ft_atoi_base ===\n");

	/* --- base 10 basics --- */
	check("\"10\" base10 == 10",         ft_atoi_base("10",  "0123456789") == 10);
	check("\"0\" base10 == 0",           ft_atoi_base("0",   "0123456789") == 0);
	check("\"1\" base10 == 1",           ft_atoi_base("1",   "0123456789") == 1);
	check("\"255\" base10 == 255",       ft_atoi_base("255", "0123456789") == 255);
	check("\"1000\" base10 == 1000",     ft_atoi_base("1000","0123456789") == 1000);

	/* --- sign handling --- */
	check("\"-42\" base10 == -42",       ft_atoi_base("-42", "0123456789") == -42);
	check("\"+42\" base10 == 42",        ft_atoi_base("+42", "0123456789") == 42);
	check("\"--42\" base10 == 42",       ft_atoi_base("--42","0123456789") == 42);
	check("\"+-42\" base10 == -42",      ft_atoi_base("+-42","0123456789") == -42);
	check("\"---5\" base10 == -5",       ft_atoi_base("---5","0123456789") == -5);
	check("\"-0\" base10 == 0",          ft_atoi_base("-0",  "0123456789") == 0);

	/* --- leading whitespace --- */
	check("\"  42\" base10 == 42",       ft_atoi_base("  42",  "0123456789") == 42);
	check("\"\\t\\n5\" base10 == 5",     ft_atoi_base("\t\n5", "0123456789") == 5);
	check("\"  -7\" base10 == -7",       ft_atoi_base("  -7",  "0123456789") == -7);
	check("\"  +7\" base10 == 7",        ft_atoi_base("  +7",  "0123456789") == 7);

	/* --- base 2 (binary) --- */
	check("\"10\" base2 == 2",           ft_atoi_base("10",       "01") == 2);
	check("\"0\" base2 == 0",            ft_atoi_base("0",        "01") == 0);
	check("\"1\" base2 == 1",            ft_atoi_base("1",        "01") == 1);
	check("\"1111\" base2 == 15",        ft_atoi_base("1111",     "01") == 15);
	check("\"11111111\" base2 == 255",   ft_atoi_base("11111111", "01") == 255);
	check("\"-10\" base2 == -2",         ft_atoi_base("-10",      "01") == -2);

	/* --- base 8 (octal) --- */
	check("\"10\" base8 == 8",           ft_atoi_base("10", "01234567") == 8);
	check("\"77\" base8 == 63",          ft_atoi_base("77", "01234567") == 63);
	check("\"0\" base8 == 0",            ft_atoi_base("0",  "01234567") == 0);

	/* --- base 16 lowercase --- */
	check("\"ff\" base16 == 255",        ft_atoi_base("ff", "0123456789abcdef") == 255);
	check("\"10\" base16 == 16",         ft_atoi_base("10", "0123456789abcdef") == 16);
	check("\"1a\" base16 == 26",         ft_atoi_base("1a", "0123456789abcdef") == 26);
	check("\"0\" base16 == 0",           ft_atoi_base("0",  "0123456789abcdef") == 0);

	/* --- base 16 uppercase --- */
	check("\"FF\" base16UP == 255",      ft_atoi_base("FF",         "0123456789ABCDEF") == 255);
	check("\"  --+--+FF\" base16UP == 255",
		ft_atoi_base("  --+--+FF", "0123456789ABCDEF") == 255);

	/* --- char not in base stops conversion --- */
	check("\"19\" base2: stops at '9' == 1",
		ft_atoi_base("19", "01") == 1);
	check("\"8\" base8: '8' not in base == 0",
		ft_atoi_base("8",  "01234567") == 0);

	/* --- empty string --- */
	check("empty str base10 == 0",       ft_atoi_base("",    "0123456789") == 0);

	/* --- invalid base: length <= 1 => return 0 --- */
	check("single-char base (\"0\") => 0",  ft_atoi_base("5", "0") == 0);
	check("empty base => 0",                ft_atoi_base("5", "")  == 0);

	/* --- invalid base: contains '+' --- */
	check("base with '+' => 0",          ft_atoi_base("1", "0+1") == 0);

	/* --- invalid base: contains '-' --- */
	check("base with '-' => 0",          ft_atoi_base("1", "0-1") == 0);

	/* --- invalid base: contains whitespace --- */
	check("base with space => 0",        ft_atoi_base("1", "0 1")  == 0);
	check("base with tab => 0",          ft_atoi_base("1", "0\t1") == 0);
	check("base with newline => 0",      ft_atoi_base("1", "0\n1") == 0);

	/* --- invalid base: duplicate characters --- */
	check("base with dup '0' (\"001\") => 0",
		ft_atoi_base("1", "001") == 0);
	check("base with dup 'a' (\"0123456789aabcdef\") => 0",
		ft_atoi_base("1", "0123456789aabcdef") == 0);
}

/* ================================================================
 * ft_list_push_front
 * ================================================================ */
static void test_list_push_front(void)
{
	t_list *list = NULL;

	printf("\n=== ft_list_push_front ===\n");

	/* push to empty list */
	int v1 = 1;
	ft_list_push_front((void **)&list, &v1);
	check("push to empty: list not NULL",        list != NULL);
	check("push to empty: data == &v1",          list && list->data == &v1);
	check("push to empty: next == NULL",         list && list->next == NULL);
	check("push to empty: size == 1",            ft_list_size(list) == 1);

	/* push second element — becomes new head */
	int v2 = 2;
	ft_list_push_front((void **)&list, &v2);
	check("push 2nd: head->data == &v2",         list && list->data == &v2);
	check("push 2nd: head->next->data == &v1",   list && list->next && list->next->data == &v1);
	check("push 2nd: next->next == NULL",        list && list->next && list->next->next == NULL);
	check("push 2nd: size == 2",                 ft_list_size(list) == 2);

	/* push third element */
	int v3 = 3;
	ft_list_push_front((void **)&list, &v3);
	check("push 3rd: head->data == &v3",         list && list->data == &v3);
	check("push 3rd: size == 3",                 ft_list_size(list) == 3);

	ft_list_clear(&list);
	check("after clear: list == NULL",           list == NULL);

	/* push NULL data — should still allocate the node */
	ft_list_push_front((void **)&list, NULL);
	check("push NULL data: node created",        list != NULL);
	check("push NULL data: data field == NULL",  list && list->data == NULL);
	ft_list_clear(&list);

	/* push many elements and verify ordering */
	int vals[5] = {10, 20, 30, 40, 50};
	for (int i = 0; i < 5; i++)
		ft_list_push_front((void **)&list, &vals[i]);
	/* list should be: 50 -> 40 -> 30 -> 20 -> 10 */
	check("push 5: size == 5",                   ft_list_size(list) == 5);
	check("push 5: head is last pushed (&vals[4])",
		list && list->data == &vals[4]);
	ft_list_clear(&list);
}

/* ================================================================
 * ft_list_size
 * ================================================================ */
static void test_list_size(void)
{
	t_list *list = NULL;
	int vals[10];

	printf("\n=== ft_list_size ===\n");

	/* empty / NULL */
	check("NULL list: size == 0",                ft_list_size(NULL) == 0);
	check("empty list: size == 0",               ft_list_size(list) == 0);

	/* single element */
	ft_list_push_front((void **)&list, &vals[0]);
	check("1 element: size == 1",                ft_list_size(list) == 1);

	/* grow to 5 */
	for (int i = 1; i < 5; i++)
		ft_list_push_front((void **)&list, &vals[i]);
	check("5 elements: size == 5",               ft_list_size(list) == 5);

	/* grow to 10 */
	for (int i = 5; i < 10; i++)
		ft_list_push_front((void **)&list, &vals[i]);
	check("10 elements: size == 10",             ft_list_size(list) == 10);

	ft_list_clear(&list);
	check("after clear: size == 0",              ft_list_size(list) == 0);
}

/* ================================================================
 * ft_list_sort
 * ================================================================ */
static void test_list_sort(void)
{
	printf("\n=== ft_list_sort ===\n");

	/* empty list */
	t_list *empty = NULL;
	ft_list_sort((void **)&empty, int_cmp);
	check("sort empty list: still NULL",         empty == NULL);

	/* single element */
	t_list *single = NULL;
	int sv = 42;
	ft_list_push_front((void **)&single, &sv);
	ft_list_sort((void **)&single, int_cmp);
	check("sort single: not NULL",               single != NULL);
	check("sort single: data preserved",         single && single->data == &sv);
	check("sort single: is sorted",              list_is_sorted(single));
	ft_list_clear(&single);

	/* two elements — sorted */
	t_list *two_s = NULL;
	int a2s = 2, b2s = 1;
	ft_list_push_front((void **)&two_s, &a2s); /* list: 2 */
	ft_list_push_front((void **)&two_s, &b2s); /* list: 1 -> 2 */
	ft_list_sort((void **)&two_s, int_cmp);
	check("two elements (already sorted): sorted", list_is_sorted(two_s));
	check("two elements (already sorted): size 2", ft_list_size(two_s) == 2);
	ft_list_clear(&two_s);

	/* two elements — reverse order */
	t_list *two_r = NULL;
	int a2r = 1, b2r = 2;
	ft_list_push_front((void **)&two_r, &b2r); /* list: 2 */
	ft_list_push_front((void **)&two_r, &a2r); /* list: 1 ... wait */
	/* push_front(2) → {2}; push_front(1) → {1,2}: already sorted, reverse test: */
	/* push order to get {2,1}: push 1 first, then push 2 */
	ft_list_clear(&two_r);
	two_r = NULL;
	ft_list_push_front((void **)&two_r, &a2r); /* list: 1 */
	ft_list_push_front((void **)&two_r, &b2r); /* list: 2 -> 1 */
	ft_list_sort((void **)&two_r, int_cmp);
	check("two elements (reverse): sorted",      list_is_sorted(two_r));
	ft_list_clear(&two_r);

	/* already sorted (ascending) */
	t_list *asc = NULL;
	int asc_data[] = {5, 4, 3, 2, 1};
	/* push 5,4,3,2,1 → list is 1->2->3->4->5 */
	for (int i = 0; i < 5; i++)
		ft_list_push_front((void **)&asc, &asc_data[i]);
	ft_list_sort((void **)&asc, int_cmp);
	check("already sorted (1..5): is sorted",    list_is_sorted(asc));
	check("already sorted (1..5): size == 5",    ft_list_size(asc) == 5);
	ft_list_clear(&asc);

	/* reverse sorted (descending before sort) */
	t_list *desc = NULL;
	int desc_data[] = {1, 2, 3, 4, 5};
	/* push 1,2,3,4,5 → list is 5->4->3->2->1 */
	for (int i = 0; i < 5; i++)
		ft_list_push_front((void **)&desc, &desc_data[i]);
	ft_list_sort((void **)&desc, int_cmp);
	check("reverse (5..1): is sorted after",     list_is_sorted(desc));
	check("reverse (5..1): size == 5",           ft_list_size(desc) == 5);
	ft_list_clear(&desc);

	/* random order */
	t_list *rnd = NULL;
	int rnd_data[] = {3, 1, 4, 1, 5, 9, 2, 6};
	for (int i = 7; i >= 0; i--)
		ft_list_push_front((void **)&rnd, &rnd_data[i]);
	ft_list_sort((void **)&rnd, int_cmp);
	check("random 8 elements: is sorted",        list_is_sorted(rnd));
	check("random 8 elements: size == 8",        ft_list_size(rnd) == 8);
	ft_list_clear(&rnd);

	/* all duplicates */
	t_list *dup = NULL;
	int dup_data[] = {3, 3, 3};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&dup, &dup_data[i]);
	ft_list_sort((void **)&dup, int_cmp);
	check("all duplicates (3,3,3): is sorted",   list_is_sorted(dup));
	check("all duplicates: size == 3",           ft_list_size(dup) == 3);
	ft_list_clear(&dup);

	/* large list */
	t_list *big = NULL;
	int big_data[100];
	for (int i = 0; i < 100; i++)
		big_data[i] = 99 - i;          /* 99,98,…,0 */
	for (int i = 0; i < 100; i++)
		ft_list_push_front((void **)&big, &big_data[i]);
	ft_list_sort((void **)&big, int_cmp);
	check("100-element descending: is sorted",   list_is_sorted(big));
	check("100-element descending: size == 100", ft_list_size(big) == 100);
	ft_list_clear(&big);
}

/* ================================================================
 * ft_list_remove_if
 * ================================================================ */
static void test_list_remove_if(void)
{
	printf("\n=== ft_list_remove_if ===\n");

	/* empty list — must not crash */
	t_list *empty = NULL;
	int dummy_val = 42;
	ft_list_remove_if(&empty, &dummy_val, int_cmp, dummy_free);
	check("remove from empty list: still NULL",  empty == NULL);

	/* remove the only element */
	t_list *single = NULL;
	int sv = 99;
	ft_list_push_front((void **)&single, &sv);
	ft_list_remove_if(&single, &sv, int_cmp, dummy_free);
	check("remove only element: list == NULL",   single == NULL);

	/* remove head node (value == 1) from {1, 2, 3} */
	t_list *head_rm = NULL;
	int h_data[] = {1, 2, 3};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&head_rm, &h_data[i]);
	int rm_head = 1;
	ft_list_remove_if(&head_rm, &rm_head, int_cmp, dummy_free);
	check("remove head: size == 2",              ft_list_size(head_rm) == 2);
	check("remove head: new head value == 2",
		head_rm && *(int *)head_rm->data == 2);
	check("remove head: 1 no longer present",    !list_contains_val(head_rm, 1));
	check("remove head: 3 still present",        list_contains_val(head_rm, 3));
	ft_list_clear(&head_rm);

	/* remove tail node (value == 3) from {1, 2, 3} */
	t_list *tail_rm = NULL;
	int t_data[] = {1, 2, 3};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&tail_rm, &t_data[i]);
	int rm_tail = 3;
	ft_list_remove_if(&tail_rm, &rm_tail, int_cmp, dummy_free);
	check("remove tail: size == 2",              ft_list_size(tail_rm) == 2);
	check("remove tail: 3 no longer present",    !list_contains_val(tail_rm, 3));
	check("remove tail: 1 still present",        list_contains_val(tail_rm, 1));
	check("remove tail: 2 still present",        list_contains_val(tail_rm, 2));
	ft_list_clear(&tail_rm);

	/* remove middle node (value == 2) from {1, 2, 3} */
	t_list *mid_rm = NULL;
	int m_data[] = {1, 2, 3};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&mid_rm, &m_data[i]);
	int rm_mid = 2;
	ft_list_remove_if(&mid_rm, &rm_mid, int_cmp, dummy_free);
	check("remove middle: size == 2",            ft_list_size(mid_rm) == 2);
	check("remove middle: 2 no longer present",  !list_contains_val(mid_rm, 2));
	check("remove middle: 1 still present",      list_contains_val(mid_rm, 1));
	check("remove middle: 3 still present",      list_contains_val(mid_rm, 3));
	ft_list_clear(&mid_rm);

	/* remove all nodes — all equal value */
	t_list *all_rm = NULL;
	int a_val = 7;
	int a_data[] = {7, 7, 7, 7};
	for (int i = 3; i >= 0; i--)
		ft_list_push_front((void **)&all_rm, &a_data[i]);
	ft_list_remove_if(&all_rm, &a_val, int_cmp, dummy_free);
	check("remove all equal nodes: list == NULL", all_rm == NULL);

	/* remove none — value not present */
	t_list *none_rm = NULL;
	int n_data[] = {1, 2, 3};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&none_rm, &n_data[i]);
	int not_found = 99;
	ft_list_remove_if(&none_rm, &not_found, int_cmp, dummy_free);
	check("remove none: size unchanged (3)",     ft_list_size(none_rm) == 3);
	check("remove none: 1 still present",        list_contains_val(none_rm, 1));
	check("remove none: 2 still present",        list_contains_val(none_rm, 2));
	check("remove none: 3 still present",        list_contains_val(none_rm, 3));
	ft_list_clear(&none_rm);

	/* remove multiple non-consecutive occurrences: {1, 5, 2, 5, 3} remove 5 */
	t_list *multi_rm = NULL;
	int mu_data[] = {1, 5, 2, 5, 3};
	for (int i = 4; i >= 0; i--)
		ft_list_push_front((void **)&multi_rm, &mu_data[i]);
	int mu_val = 5;
	ft_list_remove_if(&multi_rm, &mu_val, int_cmp, dummy_free);
	check("remove multiple (non-adjacent): size == 3", ft_list_size(multi_rm) == 3);
	check("remove multiple: 5 no longer present",  !list_contains_val(multi_rm, 5));
	check("remove multiple: 1 still present",       list_contains_val(multi_rm, 1));
	check("remove multiple: 2 still present",       list_contains_val(multi_rm, 2));
	check("remove multiple: 3 still present",       list_contains_val(multi_rm, 3));
	ft_list_clear(&multi_rm);

	/* remove consecutive duplicates at head: {5, 5, 1} remove 5 */
	t_list *cons_head = NULL;
	int ch_data[] = {5, 5, 1};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&cons_head, &ch_data[i]);
	int ch_val = 5;
	ft_list_remove_if(&cons_head, &ch_val, int_cmp, dummy_free);
	check("remove consecutive head dup: size == 1",  ft_list_size(cons_head) == 1);
	check("remove consecutive head dup: 5 gone",     !list_contains_val(cons_head, 5));
	check("remove consecutive head dup: 1 present",  list_contains_val(cons_head, 1));
	ft_list_clear(&cons_head);

	/* remove consecutive duplicates at tail: {1, 5, 5} remove 5 */
	t_list *cons_tail = NULL;
	int ct_data[] = {1, 5, 5};
	for (int i = 2; i >= 0; i--)
		ft_list_push_front((void **)&cons_tail, &ct_data[i]);
	int ct_val = 5;
	ft_list_remove_if(&cons_tail, &ct_val, int_cmp, dummy_free);
	check("remove consecutive tail dup: size == 1",  ft_list_size(cons_tail) == 1);
	check("remove consecutive tail dup: 5 gone",     !list_contains_val(cons_tail, 5));
	check("remove consecutive tail dup: 1 present",  list_contains_val(cons_tail, 1));
	ft_list_clear(&cons_tail);
}

/* ================================================================
 * main
 * ================================================================ */
int main(void)
{
	printf("========================================\n");
	printf("   libasm bonus functions tester\n");
	printf("========================================\n");

	test_atoi_base();
	test_list_push_front();
	test_list_size();
	test_list_sort();
	test_list_remove_if();

	printf("\n========================================\n");
	printf("Results: %d passed, %d failed\n", g_passed, g_failed);
	printf("========================================\n");

	return (g_failed > 0) ? 1 : 0;
}
