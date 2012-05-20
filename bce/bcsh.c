/*
 *	BCE Shell
 *
 *	Copyright (C) 2011-2012 XiaoJSoft Studio. All Rights Reserved.
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
#include <unistd.h>
#include <string.h>
#include "../include/bcsh.h"
#include "../libbce/include/bce.h"
#include "../libbce/include/blockmem.h"
#include "../libbce/include/polynomial.h"
#include "../include/stream_io.h"
#include "../include/lang/bce_lang.h"

#ifdef _SINGLE_
	#include "../single/single.h"
#endif

#ifdef _WIN32_
	#define HELPFILE_PATH "help.txt"
	#define BANNER_PATH "banner.txt"
#else
	#define HELPFILE_PATH "/usr/share/bce/help"
	#define BANNER_PATH "/usr/share/bce/banner"
#endif

/*  Argument options  */
static char *options = "m:hs";

/*
 *	main()
 *
 *	Main processor of BCE
 */
int main(int argc, char *argv[], char *envp[]) {
	char *p,lcc, *r;
	int mode = 2, silent = 0, sh = 0, opt, mrcc;
	exp *mr, *mp;
	/*  Deal with the arguments  */
	while ((opt = getopt(argc, argv, options)) != -1) {
		switch (opt) {
			case 'm':
				/*  Change output mode  */
				mode = atoi(optarg);
				if (mode != 1 && mode != 2) {
					printf(LANG_ERROR_MODE, optarg);
					return(BCENO_IVARG);
				}
				break;
			case 'h':
				/*  Show help  */
				sh = 1;
				break;
			case 's':
				/*  Not to show the banner  */
				silent = 1;
				break;
			case ':':
				return(BCENO_NOVALUE);
				break;
			case '?':
				return(BCENO_IVARG);
				break;
			default:
				break;
		}
	}
	/*  Show the banner  */
	if (!silent)
		#ifdef _SINGLE_
			printf(SINGLE_BANNER_STR);
		#else
			if (print_textfile(BANNER_PATH, stdout) != LIO_SUCCESS)
				printf(LANG_ERROR_OPENFILE, BANNER_PATH);
		#endif
	/*  Show the help  */
	if (sh)
		#ifdef _SINGLE_
			printf(SINGLE_HELP_STR);
		#else
			if (print_textfile(HELPFILE_PATH, stdout) != LIO_SUCCESS)
				printf(LANG_ERROR_OPENFILE, HELPFILE_PATH);
		#endif
	while(!0) {
		/*  Show the prompt if stdin refers to tty  */
		if (isatty(fileno(stdin)))
			printf(CMD_PROMPT);
		/*  Input a string  */
		p = linein(stdin, &lcc);
		if (!p)
			return(BCENO_MEMFAIL);
		/*  Check the string  */
		if (lcc == EOF || (strcmp(p, CMD_EXIT) == 0))
			break;
		if (*p == '\0')
			goto jump;
		/*  Balance the chemical equation  */
		if (mode == 1) {
			mr = balance_chemical_equation(p, &mrcc);
			if (!mr) {
				printf(LANG_ERROR_CE1);
				goto jump;
			}
			for (mp = mr; mp < mr + mrcc; mp++) {
				r = sprint_expression(*mp);
				if (!r) {
					if (mp == mr + mrcc - 1)
						printf(LANG_ERROR_CE1);
					else
						printf(LANG_ERROR_CE2);
				} else {
					if (mp == mr + mrcc - 1)
						printf("%s\n", r);
					else
						printf("%s,", r);
					free(r);
				}
			}
			free_expression_stack(mr, mrcc);
		} else {
			r = automatic_balance_ce(p);
			if (r) {
				printf("%s\n", r);
				free(r);
			} else {
				printf(LANG_ERROR_CE1);
			}
		}
jump:
		free(p);
	}
	/*  Destroy the p if its value isn't NULL  */
	if (p)
		free(p);
	return(BCENO_SUCCESS);
}

