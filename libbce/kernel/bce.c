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

/*
 *	balance_chemical_equation()
 *
 *	Balance a chemical equation.
 *
 *	@nptr: the formula of the chemical equation
 *	@count: the count of balance results will be put in this variable
 */
exp* balance_chemical_equation(char *nptr, int *count) {
	int idx, mx, my;
	fact **mtx;
	exp *ret, *ep;
	mtx = get_balance_matrix(nptr, &mx, &my);
	if (!mtx)
		return(NULL);

	ret = solve_equations(mtx, mx, my, 0, 0, 0);
	if (!ret) {
		free_matrix(mtx, my);
		return(NULL);
	}

	if (check_equation_result(mtx, ret, mx, my) != EXPMODULE_TRUE) {
		free_matrix(mtx, my);
		free_expression_stack(ret, mx - 1);
		return(NULL);
	}

	free_matrix(mtx, my);

	finishing_expression_stack(ret, mx - 1);
	expression_to_number(ret, mx - 1, EXPMODULE_TRUE);

	for (idx = 0, ep = ret; idx < mx - 1; idx++, ep++)
		if (ep->count == 0 && fraction_compare(ep->cst, F_ZERO) == 0) {
			free_expression_stack(ret, mx - 1);
			return(NULL);
		}

	*count = mx - 1;

	return(ret);
}

/*
 *	automatic_balance_ce()
 *
 *	Balance a chemical equation and return the result automatically.
 *
 *	@nptr: the formula of the chemical equation
 */
char* automatic_balance_ce(char *nptr) {
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

