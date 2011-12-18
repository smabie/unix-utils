/* slice.c */
/* 
 * This code is public domain as declared by Sturm Mabie.
 */

#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void usage();
void slice(FILE *, long, long);

int bflg, lflg, wflg;

int
main(int argc, char **argv)
{
	int c, many, tmp;
	FILE *fp;
	long b, e;
	char buf[22]; 		/* 2 * maxlen(long) + 2 */
	char *s;
	const char *er;

	bflg = lflg = wflg = c = 0;
	while ((c = getopt(argc, argv, "blw")) != -1) {
		switch (c) {
		case 'b':
			bflg = 1;
			break;
		case 'l':
			lflg = 1;
			break;
		case 'w':
			wflg = 1;
			break;
		default:
			usage();
		}
	}
	argv += optind;
	argc -= optind;

	if (bflg + lflg + wflg == 0)
		lflg = 1;
	if (argc == 0 || bflg + lflg + wflg > 1)
		usage();

	(void)strlcpy(buf, *argv, sizeof(buf));
	if ((s = strchr(buf, '-')) == NULL) {
		b = e = strtonum(buf, 1, LONG_MAX, &er);
		if (er != NULL)
			errx(1, "the number %s is %s", buf, er);
	} else {
		*s = '\0';
		b = strtonum(buf, 1, LONG_MAX, &er);
		if (er != NULL)
			errx(1, "the number %s is %s", buf, er);

		e = strtonum(s + 1, 1, LONG_MAX, &er);
		if (er != NULL)
			errx(1, "the number %s is %s", buf, er);

		if (b > e) {
			tmp = b;
			b = e;
			e = tmp;
		}
	}
	argc--, argv++;

	if (argc == 0) {
		slice(stdin, b, e);
		return 0;
	} else if (argc > 1) {
		many = 1;
	} else
		many = 0;

	for (; argc > 0; argc--, argv++) {
		if (strcmp(*argv, "-") == 0) {
			if (many)
				(void)puts("==> stdin <==");
			slice(stdin, b, e);
			continue;
		}
		if ((fp = fopen(*argv, "r")) == NULL)
			err(1, "fopen: %s", *argv);
		if (many) {
			if ((s = realpath(*argv, NULL)) == NULL)
				err(1, "realpath: %s", *argv);
			(void)printf("==> %s <==\n", s);
			free(s);
		}
		slice(fp, b, e);
		(void)fclose(fp);
	}
	return 0;
}

void
usage()
{
	(void)fprintf(stderr, "usage: slice [-b | -l | -w] range [file ...]\n");
	exit(1);
}

void
slice(FILE *fp, long b, long e)
{
#define IF_PRINT(a) do {			\
		if ((a) >= b && (a) <= e)	\
			(void)putchar(c);	\
		else if ((a) > e) {		\
			(void)putchar('\n');	\
			return;			\
		}				\
	} while (0)				

	long nl, nw, nb;
	enum { IN, OUT } state;
	int c;

	state = OUT;
	nw = nb = 0;
	nl = 1;

	while ((c = fgetc(fp)) != EOF) {
		if (state == IN && isspace(c))
			state = OUT;
		if (state == OUT && !isspace(c)) {
			state = IN;
			nw++;
		} 
		if (c == '\n')
			nl++;
		nb++;

		if (lflg) IF_PRINT(nl);
		if (bflg) IF_PRINT(nb);
		if (wflg) IF_PRINT(nw);
	}
}
