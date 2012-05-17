/*
 *	Keyboard Input
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
#include <memory.h>
#include "../include/keyb.h"

#define BUFFER_STDIO_LEN 64

char *fline(FILE *fp, int *lastchar)
{
	char buffer[BUFFER_STDIO_LEN];
	char *ptr = NULL, *ret = NULL;
	char in;
	int end = 1, times = 0, scan = EOF;

	ptr = buffer;

	while (end) {
		/*  get a character  */
		scan = fscanf(fp, "%c", &in);

		/*  whether it's the end of the string  */
		if (scan == EOF || in == '\n' || in == '\0') {
			if (lastchar != NULL)
				*lastchar = scan;
			in = '\0';
			end = 0;
		}

		*(ptr++) = in;
		/*  buffer is full or EOF, fill the store with the buffer and clean the buffer  */
		if (ptr == buffer + BUFFER_STDIO_LEN || !end) {
			ret = (char*) realloc(ret, (times + 1) * BUFFER_STDIO_LEN * sizeof(char));
			if (!ret)
				return(NULL);
			memcpy(ret + (times++) * BUFFER_STDIO_LEN, buffer, BUFFER_STDIO_LEN * sizeof(char));
			ptr = buffer;
		}
	}

	return(ret);
}

