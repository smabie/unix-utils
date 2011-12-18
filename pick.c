/* pick.c */
/* 
 * This code is public domain as declared by Sturm Mabie.
 */

#include <err.h>
#include <errno.h>
#include <getopt.h> 
#include <paths.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ask(FILE *);

FILE *tty;

int
main(int argc, char **argv)
{
	int c;
	FILE *fp;

	while ((c = getopt(argc, argv, "")) != -1) {
		(void)fprintf(stderr, "usage: pick [file ...]\n");
		exit(1);
	}
	argc--, argv++;

	if ((tty = fopen(_PATH_TTY, "r")) == NULL)
		err(1, "fopen: %s", _PATH_TTY);
	
	if (argc == 0) {
		ask(stdin);
		return 0;
	}
	for (; argc > 0; argc--, argv++) {
		if (strcmp(*argv, "-") == 0) {
			ask(stdin);
			continue;
		}
		if ((fp = fopen(*argv, "r")) == NULL)
			err(1, "fopen: %s", *argv);
		ask(fp);
		(void)fclose(fp);
	}
	return 0;
}

void
ask(FILE *fp)
{
	char buf[1024], in[1024];

	clearerr(fp);
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		buf[strcspn(buf, "\n")] = '\0';

		(void)fprintf(stderr, "%s ? ", buf);
		if (fgets(in, sizeof(in), tty) == NULL)
			err(1, "fgets: %s", _PATH_TTY);
		if (in[0] == 'y')
			(void)printf("%s\n", buf);
		else if (in[0] == 'q')
			return;
	}
	if (!feof(fp))
		err(1, "fgets");
}
