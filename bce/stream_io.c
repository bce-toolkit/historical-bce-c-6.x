/*
 *	Stream I/O Controller
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
#include "../include/stream_io.h"
#include "../libbce/include/blockmem.h"

/*
 *	linein()
 *
 *	Input of a string line.
 *
 *	fp: Input file
 *	lcc: variable to storage the last character in this string
 */
char *linein(FILE *fp, char *lcc) {
	bmem buffer = empty_block_memory(DEFAULT_PAGE_SIZE);
	int ch, n = 0;
	do {
		ch = fgetc(fp);
		if (ch == '\n' || ch == EOF) {
			if (lcc != NULL)
				*lcc = (char) ch;
			ch = '\0';
		}
		if (reallocate_block_memory(&buffer, (++n) * sizeof(char)) != BLOCKMEM_SUCCESS)
			return(NULL);
		*((char*)buffer.ptr + n - 1) = (char) ch;
	} while(ch != '\0');
	return(buffer.ptr);
}

/*
 *	print_textfile()
 *
 *	Read a file and print it out.
 *
 *	path: Path of the file
 *	t: Output file
 */
int print_textfile(char *path, FILE *t) {
	FILE *f;
	char *p, lcc = EOF;
	f = fopen(path, "r");
	if (!f)
		return(LIO_ERROR);
	do {
		p = linein(f, &lcc);
		if (!p)
			break;
		if (!(*p == '\0' && lcc == EOF))
			fprintf(t, "%s\n", p);
		free(p);
	} while(lcc != EOF);
	fclose(f);
	return(LIO_SUCCESS);
}

