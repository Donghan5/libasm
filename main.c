#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

extern size_t   ft_strlen(const char *str);
extern ssize_t  ft_write(int fd, const void *buf, size_t count);
extern ssize_t  ft_read(int fd, void *buf, size_t count);
extern char    *ft_strcpy(char *dst, const char *src);
extern char    *ft_strdup(const char *str);
extern int      ft_strcmp(const char *s1, const char *s2);

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

/* normalise strcmp/ft_strcmp result to -1 / 0 / 1 */
static int sign(int v) { return (v > 0) - (v < 0); }

/* ================================================================
 * ft_strlen
 * ================================================================ */
static void test_strlen(void)
{
	char long_str[10001];
	char embedded[] = {'H', 'e', 'l', 'l', 'o', '\0', 'W', 'o', 'r', 'l', 'd', '\0'};

	printf("\n=== ft_strlen ===\n");

	check("empty string",
		ft_strlen("") == strlen(""));
	check("single char 'a'",
		ft_strlen("a") == strlen("a"));
	check("single null byte \"\\0\"",
		ft_strlen("\0") == strlen("\0"));
	check("\"Hello, World!\"",
		ft_strlen("Hello, World!") == strlen("Hello, World!"));
	check("spaces only \"   \"",
		ft_strlen("   ") == strlen("   "));
	check("newline + tab + CR \"\\n\\t\\r\"",
		ft_strlen("\n\t\r") == strlen("\n\t\r"));
	check("special chars \"!@#$%^&*()\"",
		ft_strlen("!@#$%^&*()") == strlen("!@#$%^&*()"));
	check("digits \"0123456789\"",
		ft_strlen("0123456789") == strlen("0123456789"));
	check("high-ASCII bytes \"\\x80\\xFF\\xAA\"",
		ft_strlen("\x80\xFF\xAA") == strlen("\x80\xFF\xAA"));

	/* very long string */
	memset(long_str, 'A', 10000);
	long_str[10000] = '\0';
	check("very long string (10000 chars)",
		ft_strlen(long_str) == strlen(long_str));

	/* embedded null — length stops at first '\0' */
	check("stops at embedded null (5)",
		ft_strlen(embedded) == strlen(embedded));
}

/* ================================================================
 * ft_strcpy
 * ================================================================ */
static void test_strcpy(void)
{
	char dst_ft[10001];
	char dst_std[10001];
	char long_src[10001];
	char *ret;

	printf("\n=== ft_strcpy ===\n");

	/* empty string */
	memset(dst_ft,  'X', sizeof(dst_ft));
	memset(dst_std, 'X', sizeof(dst_std));
	ft_strcpy(dst_ft, "");
	strcpy(dst_std,   "");
	check("empty: content matches",           strcmp(dst_ft, dst_std) == 0);
	check("empty: null terminator at [0]",    dst_ft[0] == '\0');

	/* single character */
	ft_strcpy(dst_ft, "a");
	strcpy(dst_std,   "a");
	check("single char 'a': content",         strcmp(dst_ft, dst_std) == 0);

	/* normal string */
	ft_strcpy(dst_ft, "Hello, World!");
	strcpy(dst_std,   "Hello, World!");
	check("normal string: content",           strcmp(dst_ft, dst_std) == 0);

	/* special characters */
	ft_strcpy(dst_ft, "!@#$%^&*()");
	strcpy(dst_std,   "!@#$%^&*()");
	check("special chars: content",           strcmp(dst_ft, dst_std) == 0);

	ft_strcpy(dst_ft, "\n\t\r");
	strcpy(dst_std,   "\n\t\r");
	check("newline/tab/CR: content",          strcmp(dst_ft, dst_std) == 0);

	/* high-ASCII */
	ft_strcpy(dst_ft, "\x80\xFF\xAA");
	strcpy(dst_std,   "\x80\xFF\xAA");
	check("high-ASCII bytes: content",        strcmp(dst_ft, dst_std) == 0);

	/* very long string */
	memset(long_src, 'B', 10000);
	long_src[10000] = '\0';
	ft_strcpy(dst_ft, long_src);
	strcpy(dst_std,   long_src);
	check("very long string (10000): content",strcmp(dst_ft, dst_std) == 0);

	/* return value must be dst */
	ret = ft_strcpy(dst_ft, "test");
	check("returns dst pointer",              ret == dst_ft);
}

/* ================================================================
 * ft_strcmp
 * ================================================================ */
static void test_strcmp(void)
{
	int r_ft, r_std;

	printf("\n=== ft_strcmp ===\n");

	/* equal strings */
	check("equal empty \"\" == \"\"",
		ft_strcmp("", "") == 0 && strcmp("", "") == 0);
	check("equal \"abc\" == \"abc\"",
		ft_strcmp("abc", "abc") == 0);
	check("equal single char \"a\" == \"a\"",
		ft_strcmp("a", "a") == 0);
	check("equal long identical strings",
		ft_strcmp("Hello, World!", "Hello, World!") == 0);

	/* s1 < s2 */
	r_ft  = sign(ft_strcmp("a", "b"));
	r_std = sign(strcmp("a",    "b"));
	check("s1 < s2: \"a\" vs \"b\"",          r_ft == r_std);

	r_ft  = sign(ft_strcmp("abc", "abd"));
	r_std = sign(strcmp("abc",    "abd"));
	check("s1 < s2: \"abc\" vs \"abd\"",       r_ft == r_std);

	r_ft  = sign(ft_strcmp("", "a"));
	r_std = sign(strcmp("",  "a"));
	check("s1 < s2: empty vs \"a\"",           r_ft == r_std);

	r_ft  = sign(ft_strcmp("Hello", "Hello, World!"));
	r_std = sign(strcmp("Hello",    "Hello, World!"));
	check("s1 < s2: prefix shorter",           r_ft == r_std);

	/* s1 > s2 */
	r_ft  = sign(ft_strcmp("b", "a"));
	r_std = sign(strcmp("b",    "a"));
	check("s1 > s2: \"b\" vs \"a\"",          r_ft == r_std);

	r_ft  = sign(ft_strcmp("abd", "abc"));
	r_std = sign(strcmp("abd",    "abc"));
	check("s1 > s2: \"abd\" vs \"abc\"",       r_ft == r_std);

	r_ft  = sign(ft_strcmp("a", ""));
	r_std = sign(strcmp("a",    ""));
	check("s1 > s2: \"a\" vs empty",           r_ft == r_std);

	/* same prefix, s2 longer */
	r_ft  = sign(ft_strcmp("Hello, World!", "Hello"));
	r_std = sign(strcmp("Hello, World!",    "Hello"));
	check("s1 > s2: prefix longer",            r_ft == r_std);

	/* special chars */
	r_ft  = sign(ft_strcmp("\n", "\t"));
	r_std = sign(strcmp("\n",    "\t"));
	check("special: '\\n' vs '\\t'",           r_ft == r_std);

	/* high-ASCII (note: ft_strcmp uses signed byte sub; may differ from libc) */
	r_ft  = sign(ft_strcmp("\x80", "\x01"));
	r_std = sign(strcmp("\x80",    "\x01"));
	check("high-ASCII: \"\\x80\" vs \"\\x01\" (unsigned cmp)", r_ft == r_std);

	r_ft  = sign(ft_strcmp("\x01", "\x80"));
	r_std = sign(strcmp("\x01",    "\x80"));
	check("high-ASCII: \"\\x01\" vs \"\\x80\" (unsigned cmp)", r_ft == r_std);

	/* NULL vs NULL — assembly checks rdi/rsi for 0, returns 0 */
	check("NULL vs NULL == 0", ft_strcmp(NULL, NULL) == 0);
}

/* ================================================================
 * ft_write
 * ================================================================ */
static void test_write(void)
{
	ssize_t r_ft, r_std;
	int errno_ft, errno_std;
	int pipefd[2];
	int devnull, rofd;
	char big[4096];

	printf("\n=== ft_write ===\n");

	/* --- valid writes to /dev/null --- */
	devnull = open("/dev/null", O_WRONLY);
	if (devnull >= 0)
	{
		r_ft = ft_write(devnull, "Hello", 5);
		check("write 5 bytes to /dev/null: returns 5", r_ft == 5);

		r_ft = ft_write(devnull, "", 0);
		check("write 0 bytes: returns 0",               r_ft == 0);

		r_ft = ft_write(devnull, "A", 1);
		check("write 1 byte: returns 1",                r_ft == 1);

		memset(big, 'Z', sizeof(big));
		r_ft = ft_write(devnull, big, sizeof(big));
		check("write 4096 bytes: returns 4096",         r_ft == (ssize_t)sizeof(big));

		close(devnull);
	}

	/* --- invalid fd: must return -1 and set errno = EBADF --- */
	errno = 0;
	r_std = write(-1, "test", 4);
	errno_std = errno;

	errno = 0;
	r_ft = ft_write(-1, "test", 4);
	errno_ft = errno;

	check("invalid fd (-1): returns -1",             r_ft == -1);
	check("invalid fd (-1): errno == EBADF",         errno_ft == EBADF);
	check("invalid fd (-1): errno matches libc",     errno_ft == errno_std);
	(void)r_std;

	/* --- closed fd --- */
	if (pipe(pipefd) == 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);

		errno = 0;
		r_ft = ft_write(pipefd[1], "test", 4);
		errno_ft = errno;

		check("closed fd: returns -1",               r_ft == -1);
		check("closed fd: errno == EBADF",           errno_ft == EBADF);
	}

	/* --- write to read-only fd --- */
	rofd = open("/dev/null", O_RDONLY);
	if (rofd >= 0)
	{
		errno = 0;
		r_std = write(rofd, "test", 4);
		errno_std = errno;

		errno = 0;
		r_ft = ft_write(rofd, "test", 4);
		errno_ft = errno;

		check("read-only fd: returns -1",            r_ft == -1);
		check("read-only fd: errno matches libc",    errno_ft == errno_std);
		close(rofd);
	}
}

/* ================================================================
 * ft_read
 * ================================================================ */
static void test_read(void)
{
	ssize_t r_ft, r_std;
	int errno_ft, errno_std;
	char buf[256];
	int devzero, wfd, rfd, wofd;
	const char *tmpfile = "/tmp/ft_read_test_libasm.txt";

	printf("\n=== ft_read ===\n");

	/* --- /dev/zero gives zero bytes --- */
	devzero = open("/dev/zero", O_RDONLY);
	if (devzero >= 0)
	{
		r_ft = ft_read(devzero, buf, 10);
		check("read 10 bytes from /dev/zero: returns 10", r_ft == 10);

		int all_zero = 1;
		for (int i = 0; i < 10; i++)
			if (buf[i] != '\0') { all_zero = 0; break; }
		check("read from /dev/zero: all bytes are 0", all_zero);

		r_ft = ft_read(devzero, buf, 0);
		check("read 0 bytes: returns 0",               r_ft == 0);

		close(devzero);
	}

	/* --- read back a known file --- */
	wfd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (wfd >= 0)
	{
		write(wfd, "libasm_test_content", 19);
		close(wfd);

		rfd = open(tmpfile, O_RDONLY);
		if (rfd >= 0)
		{
			memset(buf, 0, sizeof(buf));
			r_ft = ft_read(rfd, buf, 19);
			check("read temp file: returns 19",          r_ft == 19);
			check("read temp file: content correct",
				memcmp(buf, "libasm_test_content", 19) == 0);
			close(rfd);
		}
		unlink(tmpfile);
	}

	/* --- read partial (smaller buffer than file content) --- */
	wfd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (wfd >= 0)
	{
		write(wfd, "ABCDE", 5);
		close(wfd);

		rfd = open(tmpfile, O_RDONLY);
		if (rfd >= 0)
		{
			memset(buf, 0, sizeof(buf));
			r_ft = ft_read(rfd, buf, 3);
			check("partial read (3 of 5): returns 3",    r_ft == 3);
			check("partial read: first 3 bytes correct",
				memcmp(buf, "ABC", 3) == 0);
			close(rfd);
		}
		unlink(tmpfile);
	}

	/* --- invalid fd --- */
	errno = 0;
	r_std = read(-1, buf, 10);
	errno_std = errno;

	errno = 0;
	r_ft = ft_read(-1, buf, 10);
	errno_ft = errno;

	check("invalid fd (-1): returns -1",             r_ft == -1);
	check("invalid fd (-1): errno == EBADF",         errno_ft == EBADF);
	check("invalid fd (-1): errno matches libc",     errno_ft == errno_std);
	(void)r_std;

	/* --- read from write-only fd --- */
	wofd = open("/dev/null", O_WRONLY);
	if (wofd >= 0)
	{
		errno = 0;
		r_std = read(wofd, buf, 10);
		errno_std = errno;

		errno = 0;
		r_ft = ft_read(wofd, buf, 10);
		errno_ft = errno;

		check("write-only fd: returns -1",           r_ft == -1);
		check("write-only fd: errno matches libc",   errno_ft == errno_std);
		close(wofd);
	}
}

/* ================================================================
 * ft_strdup
 * ================================================================ */
static void test_strdup(void)
{
	char *p;
	char long_src[10001];

	printf("\n=== ft_strdup ===\n");

	/* NULL input — assembly checks rdi == 0 and returns NULL */
	p = ft_strdup(NULL);
	check("NULL input: returns NULL", p == NULL);

	/* empty string */
	p = ft_strdup("");
	check("empty: not NULL",                    p != NULL);
	if (p) { check("empty: content correct",    strcmp(p, "") == 0);   free(p); }

	/* single char */
	p = ft_strdup("a");
	check("single char: not NULL",              p != NULL);
	if (p) { check("single char: content",      strcmp(p, "a") == 0);  free(p); }

	/* normal string */
	const char *orig = "Hello, World!";
	p = ft_strdup(orig);
	check("normal: not NULL",                   p != NULL);
	if (p)
	{
		check("normal: content matches",        strcmp(p, orig) == 0);
		check("normal: different pointer",      p != orig);
		check("normal: length correct",         strlen(p) == strlen(orig));
		free(p);
	}

	/* special characters */
	p = ft_strdup("!@#$%^&*()\n\t\r");
	check("special chars: not NULL",            p != NULL);
	if (p)
	{
		check("special chars: content",         strcmp(p, "!@#$%^&*()\n\t\r") == 0);
		free(p);
	}

	/* high-ASCII */
	p = ft_strdup("\x80\xFF\xAA\x01");
	check("high-ASCII: not NULL",               p != NULL);
	if (p)
	{
		check("high-ASCII: content (memcmp)",
			memcmp(p, "\x80\xFF\xAA\x01", 5) == 0);
		free(p);
	}

	/* very long string */
	memset(long_src, 'C', 10000);
	long_src[10000] = '\0';
	p = ft_strdup(long_src);
	check("very long (10000): not NULL",        p != NULL);
	if (p)
	{
		check("very long: content matches",     strcmp(p, long_src) == 0);
		check("very long: length == 10000",     strlen(p) == 10000);
		check("very long: different pointer",   p != long_src);
		free(p);
	}
}

/* ================================================================
 * main
 * ================================================================ */
int main(void)
{
	printf("========================================\n");
	printf("   libasm mandatory functions tester\n");
	printf("========================================\n");

	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write();
	test_read();
	test_strdup();

	printf("\n========================================\n");
	printf("Results: %d passed, %d failed\n", g_passed, g_failed);
	printf("========================================\n");

	return (g_failed > 0) ? 1 : 0;
}
