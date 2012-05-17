/*
 *	FCE - a functional shell for BCE kernel
 *
 *	Copyright (C) 2011 XiaoJSoft Studio. All Rights Reserved.
 *	Copyright (C) Ji WenCong <whs_jwc@163.com>
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/bce.h"
#include "include/keyb.h"
#include "include/fce.h"

int main(int argc, char *argv[], char *envp[]) {
	FILE *in, *out;
	int idx, nptr, mode = 1, prompt = 1, count = 0, lch = EOF;
	char *redirect = NULL, *spf = NULL, *oin = NULL;
	exp *rptr = NULL;

	if (argc <= 1) {
		printf(FCE_LANG_101);
		return(ERRNO_SUCCEED);
	}

	/*  look for '-o' the switches  */
	for (idx = 1; idx < argc; idx++) {
		if (strcmp(argv[idx], FCE_LANG_201) == 0) {
			if (redirect) {
				printf(FCE_LANG_102);
				return(ERRNO_TOO_MANY_OFILES);
			} else {
				if (idx + 1 < argc)
					redirect = argv[idx + 1];
			}
			idx++;
			continue;
		}
		if (strcmp(argv[idx], FCE_LANG_203) == 0) {
			printf(FCE_MANUAL);
			continue;
		}
		if (strcmp(argv[idx], FCE_LANG_207) == 0) {
			prompt = 0;
			continue;
		}
	}

	if (redirect) {
		out = fopen(redirect, "w");
		if (!out) {
			printf(FCE_LANG_103, redirect);
			return(ERRNO_FILE_IO);
		}
	} else
		out = stdout;

	for (idx = 1; idx < argc; idx++) {
		if (strcmp(argv[idx], FCE_LANG_203) == 0 || strcmp(argv[idx], FCE_LANG_207) == 0)
			continue;

		if (strcmp(argv[idx], FCE_LANG_201) == 0) {
			idx++;
			continue;
		}

		if (strcmp(argv[idx], FCE_LANG_202) == 0) {
			if (idx + 1 < argc) {
				if (argv[idx + 1])
					mode = atoi(argv[idx + 1]);
				if (mode < 1 || mode > 3) {
					printf(FCE_LANG_105);
					return(ERRNO_INVALID_MODE);
				}
			}
			idx++;
			continue;
		}

		if (strcmp(argv[idx], FCE_LANG_104) == 0)
			in = stdin;
		else
			in = fopen(argv[idx], "r");

		if (!in) {
			printf(FCE_LANG_103, argv[idx]);
			fclose(out);
			return(ERRNO_FILE_IO);
		}

		if (isatty(fileno(in)) && prompt)
			printf(FCE_PROMPT);

		oin = fline(in, &lch);
		while (lch != EOF) {
			if (!oin) {
				printf(FCE_LANG_106);
				fclose(in);
				fclose(out);
				return(ERRNO_FILE_IO);
			}

			if (strlen(oin) == 0 || *oin == FCE_LANG_108)
				goto jump_balance;

			if (strcmp(oin, FCE_LANG_107) == 0)
				break;

			if (strcmp(oin, FCE_LANG_204) == 0) {
				mode = 1;
				goto jump_balance;
			} else if (strcmp(oin, FCE_LANG_205) == 0) {
				mode = 2;
				goto jump_balance;
			} else if (strcmp(oin, FCE_LANG_206) == 0) {
				mode = 3;
				goto jump_balance;
			}

			/*  balance this chemical equation  */
			rptr = balance_chemical_equation(oin, &count);

			if (!rptr) {
				fprintf(out, FCE_LANG_109);
				goto jump_balance;
			}

			/*  print the answer directly  */
			if (mode != 1) {
				spf = redirect_print_result(oin, rptr, count);
				if (!spf) {
					fprintf(out, FCE_LANG_109);
					free_expression_stack(rptr, count);
					goto jump_balance;
				}
				fprintf(out, "%s\n", spf);
				free(spf);
			}

			if (mode != 2) {
				for (nptr = 0; nptr < count; nptr++) {
					spf = sprint_expression(*(rptr+nptr));
					if (!spf) {
						fprintf(out, FCE_LANG_109);
						free_expression_stack(rptr, count);
						goto jump_balance;
					}
					if (nptr == count - 1) {
						fprintf(out, "%s\n", spf);
					} else {
						fprintf(out, "%s,", spf);
					}
				}
			}

			free_expression_stack(rptr, count);

jump_balance:
			free(oin);
			if (isatty(fileno(in)) && prompt)
				printf(FCE_PROMPT);

			oin = fline(in, &lch);
			if (!oin)
				break;
		}

		if (oin)
			free(oin);

		fclose(in);
	}

	fclose(out);

	return(ERRNO_SUCCEED);
}

