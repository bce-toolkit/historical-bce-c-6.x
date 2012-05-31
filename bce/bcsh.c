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
#include "../include/stream_io.h"
#include "../include/lib/libbce.h"
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
static char *options = "hsb:";

/*
 *	balance_ce()
 *
 *	Chemical equation balancer
 */
void balance_ce(char *nptr) {
	char *r;
	r = automatic_balance_ce(nptr);
	if (r) {
		printf("%s\n", r);
		free(r);
	} else {
		printf(LANG_ERROR_CE1);
	}
}

/*
 *	main()
 *
 *	Main processor of BCE
 */
int main(int argc, char *argv[], char *envp[]) {
	char *p = NULL,lcc;
	int silent = 0, sh = 0, opt;
	/*  Deal with the arguments  */
	while ((opt = getopt(argc, argv, options)) != -1) {
		switch (opt) {
			case 'h':
				/*  Show help  */
				sh = 1;
				break;
			case 's':
				/*  Not to show the banner  */
				silent = 1;
				break;
			case 'b':
				balance_ce(optarg);
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
		balance_ce(p);
jump:
		free(p);
	}
	/*  Destroy the p if its value isn't NULL  */
	if (p)
		free(p);
	return(BCENO_SUCCESS);
}

