/*
 *	Chemical Equation Syntax Analyzer
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
#include <string.h>
#include <ctype.h>
#include "../include/fraction.h"
#include "../include/polynomial.h"
#include "../include/string_ext.h"
#include "../include/molecule.h"
#include "../include/syntax.h"
#include "../include/kasprintf.h"
#include "../include/matrix_io.h"

/*
 *	redirect_print_result()
 *
 *	Convert the results to a human-readable type.
 *
 *	@nptr: the formula of a chemical equation
 *	@ce_num: the results
 *	@count: the count of items listed in @ce_num
 */
char* redirect_print_result(char *nptr, exp *ce_num, int count) {
	int counter = 0, b_stack;
	char *ptr = NULL, *equal = NULL, *dot = NULL, *ret = NULL, *temp = NULL, *exchange = NULL, *pe;

	ret = (char*) malloc(sizeof(char));
	if (!ret)
		return(NULL);
	*ret = '\0';

	/*  Look for the separator  */
	for (equal = nptr; *equal != '\0' && *equal != '='; equal++)
		;

	if (*equal == '\0')
		return(NULL);

	for (dot = nptr, b_stack = 0, ptr = nptr; ptr < nptr + strlen(nptr); ptr++) {
		switch(*ptr) {
			case '(':
				b_stack++;
				break;
			case ')':
				b_stack--;
				break;
		}

		if (b_stack != 0)
			continue;

		if (*ptr == '+' || *ptr == '=' || ptr == nptr + strlen(nptr) - 1) {
			temp = strpush(dot, (ptr == nptr + strlen(nptr) - 1) ? ptr : ptr - 1);
			if (!temp) {
				free(ret);
				return(NULL);
			}

			exchange = ret;
			pe = sprint_expression(ce_num[counter]);
			if (!pe) {
				free(ret);
				return(NULL);
			}

			/*  Mix strings  */
			if (*pe == '\0') {
				kasprintf(&ret, (ptr == equal) ? "%s=" : "%s", ret);
			} else {
				if (strcmp(pe, "1") == 0) {
					kasprintf(&ret, (ptr == equal) ? "%s%s=" : "%s%s+", ret, temp);
				} else {
					if (isdigit((int)(*temp)))
						kasprintf(&ret, (ptr == equal) ? "%s(%s)(%s)=" : "%s(%s)(%s)+", ret, pe, temp);
					else
						kasprintf(&ret, (ptr == equal) ? "%s(%s)%s=" : "%s(%s)%s+", ret, pe, temp);
				}
			}
			free(pe);
			free(temp);
			free(exchange);
			if (!ret)
				return(NULL);
			counter++;
			dot = ptr + 1;
		}
	}
	if (*ret != '\0')
		ret[strlen(ret) - 1] = '\0';
	return(ret);
}

/*
 *	get_balance_matrix()
 *
 *	Convert a chemical equation into matrix form.
 *
 *	@nptr: the formula of a chemical equation
 *	@high_x, @high_y: variables that will be used to contain the size of the result matrix
 *
 */
fact** get_balance_matrix(char *nptr, int *high_x, int *high_y) {
	fact *ip_ptr, *header, **ret;
	char *ptr = NULL, *equal = NULL, *dot = NULL;
	int c = 0, ec = 0, b_stack = 0, solve_count, counter, idx;
	bmem solver = empty_block_memory(DEFAULT_PAGE_SIZE), prefetch = empty_block_memory(DEFAULT_PAGE_SIZE);
	element *p1, *p2;

	/*  Look for the separator  */
	for (equal = nptr; *equal != '\0' && *equal != '='; equal++)
		;

	if (*equal == '\0')
		return(NULL);

	for (b_stack = 0, dot = nptr, ptr = nptr; ptr < nptr + strlen(nptr); ptr++) {
		switch(*ptr) {
			case '(':
				b_stack++;
				break;
			case ')':
				b_stack--;
				break;
		}

		if (b_stack != 0)
			continue;

		if (*ptr == '+' || *ptr == '=' || ptr == nptr + strlen(nptr) - 1) {
			if (solve_molecule_hydrate(dot, (ptr == nptr + strlen(nptr) - 1) ? ptr : ptr - 1, &prefetch, &ec, 1) != ERRNO_MOLECULE_SUCCESS) {
				free_block_memory(&prefetch);
				return(NULL);
			}
			dot = ptr + 1;
			c++;
		}
	}
	if (!c) {
		free_block_memory(&prefetch);
		return(NULL);
	}

	/*  Allocate memory areas for the matrix  */
	ret = allocate_matrix(c + 1, ec + 1);
	if (!ret) {
		free_block_memory(&prefetch);
		return(NULL);
	}
	for (header = get_matrix_header(ret, ec + 1), ip_ptr = header; ip_ptr < header + (c + 1) * (ec + 1); ip_ptr++)
		*ip_ptr = fraction_create(0, 1);

	for (counter = 0, b_stack = 0, dot = nptr, ptr = nptr; ptr < nptr + strlen(nptr); ptr++) {
		switch(*ptr) {
			case '(':
				b_stack++;
				break;
			case ')':
				b_stack--;
				break;
		}

		if (b_stack != 0)
			continue;

		if (*ptr == '+' || *ptr == '=' || ptr == nptr + strlen(nptr) - 1) {
			solve_count = 0;

			if (solve_molecule_hydrate(dot, (ptr == nptr + strlen(nptr) - 1) ? ptr : ptr - 1, &solver, &solve_count, 1) != ERRNO_MOLECULE_SUCCESS) {
				free_block_memory(&solver);
				free_block_memory(&prefetch);
				free_matrix(ret, ec + 1);
				return(NULL);
			}

			for (p1 = (element*)solver.ptr; p1 < (element*)solver.ptr + solve_count; p1++)
				for (p2 = (element*)prefetch.ptr, idx = 0; p2 < (element*)prefetch.ptr + ec; p2++, idx++)
					if (strpqcomp(p1->begin, p1->end, p2->begin, p2->end) == STRLIB_TRUE) {
						write_matrix(ret, counter, idx, fraction_create((dot < equal ? p1->count : -p1->count), 1));
						break;
					}

			free_block_memory(&solver);
			dot = ptr + 1;
			counter++;
		}
	}
	free_block_memory(&prefetch);

	write_matrix(ret, 0, ec, fraction_create(1, 1));
	write_matrix(ret, c, ec, fraction_create(1, 1));

	*high_x = c + 1;
	*high_y = ec + 1;

	return(ret);
}

