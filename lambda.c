/* lambda.c */
/* 
 * This code is public domain as declared by Sturm Mabie.
 */

#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

#define TMPDIR "/tmp/lambda-cache/"

void usage();

int
main(int argc, char **argv)
{
	FILE *fp;
	char ftmp[] = TMPDIR"lambda.XXXXXXXXXX", *sh;
	int fdtmp, c;

	sh = "/bin/sh";
	while ((c = getopt(argc, argv, "i:")) != -1) {
		switch (c) {
		case 'i':
			sh = optarg;
			break;
		default:
			usage();
		}
	}
	argv += optind;
	argc -= optind;

	if (mkdir(TMPDIR, S_IRWXU) == -1) {
		if (errno != EEXIST)
			err(1, "mkdir: %s", TMPDIR);
	}
	if ((fdtmp = mkstemp(ftmp)) == -1)
		err(1, "mkstemp: %s", ftmp);
	if ((fp = fdopen(fdtmp, "r+")) == NULL)
		err(1, "fdopen: %s", ftmp);

	if (argc < 1)
		usage();

	(void)fprintf(fp, "#!%s\n", sh);
	for (; *argv != NULL; argv++)
		(void)fprintf(fp, "%s\n", *argv);

	if (chmod(ftmp, S_IRWXU) == -1)
		err(1, "chmod: %s", ftmp);

	(void)puts(ftmp);

	(void)fclose(fp);
	(void)close(fdtmp);
	return 0;
}

void
usage()
{
	(void)fprintf(stderr, "usage: lambda [-i shell] expression ...\n");
	exit(1);
}
