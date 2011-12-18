/* btee.c */
/*
 * This code is public domain as declared by Sturm Mabie.
 */

#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int c, *fd, nr, nw, fdtmp, n, tmp;
	int aflg;
	char ftmp[] = "/tmp/btee.XXXXXXXXXX";
	char buf[BUFSIZ];

	aflg = 0;
	while ((c = getopt(argc, argv, "ai")) != -1) {
		switch (c) {
		case 'a':
			aflg = 1;
			break;
		case 'i':
			(void)signal(SIGINT, SIG_IGN);
			break;
		default:
			(void)fprintf(stderr, "usage: btee [-ai] [file ...]\n");
			exit(1);
		}
	}
	argv += optind;
	argc -= optind;

	if ((fd = calloc(argc + 1, sizeof(int))) == NULL)
		err(1, "calloc");
	if ((fdtmp = mkstemp(ftmp)) == -1)
		err(1, "mkstemp: %s", ftmp);

	while ((tmp = nr = read(STDIN_FILENO, buf, BUFSIZ)) != 0 && nr != -1) {
		do {
			if ((nw = write(fdtmp, buf + tmp - nr, nr)) == -1)
				err(1, "write: %s", ftmp);
		} while (nr -= nw);
	}
	if (nr == -1)
		err(1, "read: stdin");

	for (c = 0; c < argc; c++) {
		if ((fd[c] = open(argv[c],
				  aflg ? O_WRONLY | O_CREAT | O_APPEND :
				  O_WRONLY | O_CREAT | O_TRUNC,
				  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
			err(1, "open: %s", argv[c]);
	}
	fd[argc] = STDOUT_FILENO;

	(void)lseek(fdtmp, 0, SEEK_SET);
	while ((tmp = nr = read(fdtmp, buf, BUFSIZ)) != 0 && nr != -1) {
		for (c = 0, n = nr; c < argc + 1; c++, nr = n) {
			do {
				if ((nw = write(fd[c], buf + tmp - nr, nr)) 
				    == -1)
					err(1, "write: %s", 
					    c == argc ? "stdout" : argv[c]);
			} while (nr -= nw);
		}
	}
	if (nr == -1)
		err(1, "read: %s", ftmp);

	for (c = 0; c < argc; c++)
		(void)close(fd[c]);
	(void)close(fdtmp);
	(void)unlink(ftmp);
	free(fd);

	return 0;
}
