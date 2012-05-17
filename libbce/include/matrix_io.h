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

/*  ADT type  */
#include "fraction.h"

#ifndef MATRIX_ADT
	#define MATADT fact
	#define MATRIX_ADT
#endif

MATADT *get_matrix_header(MATADT **matrix, int my);
void free_matrix(MATADT **matrix, int my);
MATADT **allocate_matrix(int x, int y);
void write_matrix(MATADT **matrix, int offset_x, int offset_y, MATADT data);
MATADT read_matrix(MATADT **matrix, int offset_x, int offset_y);

#ifndef MATRIX_IO_DEFINEDED
	#define matrix_offset(matrix, offset_x, offset_y) ((*((matrix) + (offset_y))) + (offset_x))
	#define MATRIX_IO_DEFINEDED
#endif

