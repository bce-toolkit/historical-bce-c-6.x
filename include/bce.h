/*
 *	Kernel API
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

#include "blockmem.h"

#ifndef DEFINED_BCE
	struct fractionStruct {
		int denominator;
		int numerator;
	};
	typedef struct fractionStruct fact;

	struct expressionItem {
		int flag;
		fact pfx;
	};
	typedef struct expressionItem expnode;

	struct expressionStack {
		int count;
		bmem stack;
		fact cst;
	};
	typedef struct expressionStack exp;

	#define EXPMODULE_SUCCESS 0
	#define EXPMODULE_FAILED 1
	#define EXPMODULE_TRUE 1
	#define EXPMODULE_FALSE 0

	#define DEFINED_BCE
#endif

char* sprint_expression(exp sz);
exp *balance_chemical_equation(char *nptr, int *count);
char *redirect_print_result(char *nptr, exp *ce_num, int count);
void free_expression_stack(exp *src, int count);

