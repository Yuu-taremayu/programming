#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

static struct option longopts[] = {
	{"lines", required_argument, NULL, 'n'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
	int opt;
	long nlines = DEFAULT_N_LINES;

	while ((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
		switch (opt) {
			case 'n':
				nlines = atol(optarg);
				break;
			case 'h':
				fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]", argv[0]);
				return EXIT_SUCCESS;
			case '?':
				fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]", argv[0]);
				return EXIT_FAILURE;
		}
	}

	if (argc == optind) {
		do_head(stdin, nlines);
	}
	else {
		int i;

		for (i = optind; i < argc; i++) {
			FILE *f;

			f = fopen(argv[i], "r");
			if (f == NULL) {
				perror(argv[i]);
				return EXIT_FAILURE;
			}
			do_head(f, nlines);
			fclose(f);
		}
	}

	return EXIT_SUCCESS;
}

static void do_head(FILE *f, long nlines)
{
	int c;

	if (nlines <= 0) return;

	while ((c = getc(f)) != EOF) {
		if (putchar(c) < 0) return;
		if (c == '\n') {
			nlines--;
			if (nlines == 0) return;
		}
	}
}
