/*
 *	BCE Kernel
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
#include "../include/bce.h"
#include "../include/fraction.h"
#include "../include/equation.h"
#include "../include/matrix_io.h"
#include "../include/syntax.h"
#include "../include/polynomial.h"

exp *balance_chemical_equation(char *nptr, int *count) {
	int idx, max_x, max_y;
	fact **equ_matrix;
	exp *equ_ret, *ep;
	equ_matrix = get_balance_matrix(nptr, &max_x, &max_y);
	if (!equ_matrix)
		return(NULL);

	equ_ret = solve_equations(equ_matrix, max_x, max_y, 0, 0, 0);
	if (!equ_ret) {
		free_matrix(equ_matrix, max_y);
		return(NULL);
	}

	if (check_equation_result(equ_matrix, equ_ret, max_x, max_y) != EXPMODULE_TRUE) {
		free_matrix(equ_matrix, max_y);
		free_expression_stack(equ_ret, max_x - 1);
		return(NULL);
	}

	free_matrix(equ_matrix, max_y);

	finishing_expression_stack(equ_ret, max_x - 1);
	expression_to_number(equ_ret, max_x - 1, EXPMODULE_TRUE);

	for (idx = 0, ep = equ_ret; idx < max_x - 1; idx++, ep++)
		if (ep->count == 0 && fraction_compare(ep->cst, F_ZERO) == 0) {
			free_expression_stack(equ_ret, max_x - 1);
			return(NULL);
		}

	*count = max_x - 1;

	return(equ_ret);
}

char *automatic_balance_ce(char *nptr) {
	int count;
	char *p;
	exp *ret;

	ret = balance_chemical_equation(nptr, &count);
	if (!ret)
		return(NULL);

	p = redirect_print_result(nptr, ret, count);
	free_expression_stack(ret, count);

	return(p);
}

