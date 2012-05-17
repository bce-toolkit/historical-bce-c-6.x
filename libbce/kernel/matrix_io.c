/*
 *	Matrix Manager
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
#include "../include/fraction.h"
#include "../include/matrix_io.h"

MATADT *get_matrix_header(MATADT **matrix, int my) {
	MATADT *min, **eptr = matrix + my;

	for (min = *matrix; matrix < eptr; matrix++)
		if (*matrix < min)
			min = *matrix;

	return(min);
}

void free_matrix(MATADT **matrix, int my) {
	free(get_matrix_header(matrix, my));
	free(matrix);
}

MATADT **allocate_matrix(int x, int y) {
	MATADT *area, **matrix, **ptr;

	area = (MATADT*) malloc(x * y * sizeof(MATADT));
	if (!area)
		return(NULL);

	matrix = (MATADT**) malloc(y * sizeof(MATADT*));
	if (!matrix) {
		free(area);
		return(NULL);
	}

	for (ptr = matrix; y > 0; area+=x, y--)
		*(ptr++) = area;

	return(matrix);
}

void write_matrix(MATADT **matrix, int offset_x, int offset_y, MATADT data) {
	matrix[offset_y][offset_x] = data;
}

MATADT read_matrix(MATADT **matrix, int offset_x, int offset_y) {
	return(matrix[offset_y][offset_x]);
}

