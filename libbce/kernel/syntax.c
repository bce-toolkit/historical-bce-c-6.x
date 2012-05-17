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

char *redirect_print_result(char *nptr, exp *ce_num, int count) {
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

	for (dot = nptr, b_stack = 0, ptr = nptr; ptr < equal; ptr++) {
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

		if (*ptr == '+' || ptr == equal - 1) {
			temp = strpush(dot, (ptr == equal - 1) ? ptr : ptr - 1);
			if (!temp) {
				if (ret)
					free(ret);
				return(NULL);
			}

			exchange = ret;
			pe = sprint_expression(*(ce_num + counter));
			if (!pe) {
				if (ret)
					free(ret);
				return(NULL);
			}

			/*  Mix strings  */
			if (strcmp(pe, "") == 0) {
				kasprintf(&ret, "%s%s", ret, (ptr == equal - 1) ? "=" : "");
				goto jp1;
			}

			if (strcmp(pe, "1") == 0) {
				kasprintf(&ret, "%s%s%s", ret, temp, (ptr == equal - 1) ? "=" : "+");
			} else {
				if (isdigit((int)(*temp)))
					kasprintf(&ret, "%s(%s)(%s)%s", ret, pe, temp, (ptr == equal - 1) ? "=" : "+");
				else
					kasprintf(&ret, "%s(%s)%s%s", ret, pe, temp, (ptr == equal - 1) ? "=" : "+");
			}
jp1:
			free(pe);
			free(temp);
			free(exchange);
			if (!ret)
				return(NULL);
			counter++;
			dot = ptr + 1;
		}
	}

	for (dot = equal + 1, b_stack = 0, ptr = equal + 1; ptr < nptr + strlen(nptr); ptr++) {
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

		if (*ptr == '+' || ptr == nptr + strlen(nptr) - 1) {
			temp = strpush(dot, (ptr == nptr + strlen(nptr) - 1) ? ptr : ptr - 1);
			if (!temp) {
				if (ret)
					free(ret);
				return(NULL);
			}

			pe = sprint_expression(*(ce_num + counter));
			if (!pe) {
				if (ret)
					free(ret);
				return(NULL);
			}

			exchange = ret;

			/*  Mix strings  */
			if (strcmp(pe, "") == 0) {
				kasprintf(&ret, "%s", ret);
				goto jp2;
			}

			if (strcmp(pe, "1") == 0) {
				kasprintf(&ret, "%s%s%s", ret, temp, (ptr == nptr + strlen(nptr) - 1) ? "" : "+");
			} else {
				if (isdigit((int)(*temp)))
					kasprintf(&ret, "%s(%s)(%s)%s", ret, pe, temp, (ptr == nptr + strlen(nptr) - 1) ? "" : "+");
				else
					kasprintf(&ret, "%s(%s)%s%s", ret, pe, temp, (ptr == nptr + strlen(nptr) - 1) ? "" : "+");
			}
jp2:
			free(pe);
			free(temp);
			free(exchange);

			if (!ret)
				return(NULL);

			counter++;
			dot = ptr + 1;
		}
	}

	return(ret);
}

fact **get_balance_matrix(char *nptr, int *high_x, int *high_y) {
	fact *ip_ptr, *header;
	char *ptr = NULL, *equal = NULL, *dot = NULL;
	int c_left = 0, c_right = 0, ec_left = 0, b_stack = 0, solve_count, counter, idx;
	bmem solver = empty_block_memory(DEFAULT_PAGE_SIZE), e_prep = empty_block_memory(DEFAULT_PAGE_SIZE);
	element *p1, *p2;
	fact **ret;

	/*  Look for the separator  */
	for (equal = nptr; *equal != '\0' && *equal != '='; equal++)
		;

	if (*equal == '\0')
		return(NULL);

	for (b_stack = 0, dot = nptr, ptr = nptr; ptr < equal; ptr++) {
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

		if (*ptr == '+' || ptr == equal - 1) {
			if (solve_molecule_hydrate(dot, (ptr == equal - 1) ? ptr : ptr - 1, &e_prep, &ec_left, 1) != ERRNO_MOLECULE_SUCCESS) {
				free_block_memory(&e_prep);
				return(NULL);
			}

			dot = ptr + 1;
			c_left++;
		}
	}

	for (b_stack = 0, dot = equal + 1, ptr = equal + 1; ptr < nptr + strlen(nptr); ptr++) {
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

		if (*ptr == '+' || ptr == nptr + strlen(nptr) - 1) {
			if (solve_molecule_hydrate(dot, (ptr == nptr + strlen(nptr) - 1) ? ptr : ptr - 1, &e_prep, &ec_left, 1) != ERRNO_MOLECULE_SUCCESS) {
				free_block_memory(&e_prep);
				return(NULL);
			}

			dot = ptr + 1;
			c_right++;
		}
	}

	if (!c_left || !c_right) {
		free_block_memory(&e_prep);
		return(NULL);
	}

	/*  Allocate memory areas for the matrix  */
	ret = allocate_matrix(c_left + c_right + 1, ec_left + 1);
	if (!ret) {
		free_block_memory(&e_prep);
		return(NULL);
	}

	for (header = get_matrix_header(ret, ec_left + 1), ip_ptr = header; ip_ptr < header + (c_left + c_right + 1) * (ec_left + 1); ip_ptr++)
		*ip_ptr = fraction_create(0, 1);

	for (counter = 0, b_stack = 0, dot = nptr, ptr = nptr; ptr < equal; ptr++) {
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

		if (*ptr == '+' || ptr == equal - 1) {
			solve_count = 0;

			if (solve_molecule_hydrate(dot, (ptr == equal - 1) ? ptr : ptr - 1, &solver, &solve_count, 1) != ERRNO_MOLECULE_SUCCESS) {
				free_block_memory(&solver);
				free_block_memory(&e_prep);
				free_matrix(ret, ec_left + 1);
				return(NULL);
			}

			for (p1 = (element*)solver.ptr; p1 < (element*)solver.ptr + solve_count; p1++)
				for (p2 = (element*)e_prep.ptr, idx = 0; p2 < (element*)e_prep.ptr + ec_left; p2++, idx++)
					if (strpqcomp(p1->begin, p1->end, p2->begin, p2->end) == STRLIB_TRUE) {
						write_matrix(ret, counter, idx, fraction_create(p1->count, 1));
						break;
					}

			free_block_memory(&solver);
			dot = ptr + 1;
			counter++;
		}
	}

	for (b_stack = 0, dot = equal + 1, ptr = equal + 1; ptr < nptr + strlen(nptr); ptr++) {
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

		if (*ptr == '+' || ptr == nptr + strlen(nptr) - 1) {
			solve_count = 0;

			if (solve_molecule_hydrate(dot, (ptr == nptr + strlen(nptr) - 1) ? ptr : ptr - 1, &solver, &solve_count, 1) != ERRNO_MOLECULE_SUCCESS) {
				free_block_memory(&solver);
				free_block_memory(&e_prep);
				free_matrix(ret, ec_left + 1);
				return(NULL);
			}

			for (p1 = (element*)solver.ptr; p1 < (element*)solver.ptr + solve_count; p1++)
				for (p2 = (element*)e_prep.ptr, idx = 0; p2 < (element*)e_prep.ptr + ec_left; p2++, idx++)
					if (strpqcomp(p1->begin, p1->end, p2->begin, p2->end) == STRLIB_TRUE) {
						write_matrix(ret, counter, idx, fraction_create(-p1->count, 1));
						break;
					}

			free_block_memory(&solver);
			dot = ptr + 1;
			counter++;
		}
	}

	free_block_memory(&e_prep);

	write_matrix(ret, 0, ec_left, fraction_create(1, 1));
	write_matrix(ret, c_left + c_right, ec_left, fraction_create(1, 1));

	*high_x = c_left + c_right + 1;
	*high_y = ec_left + 1;

	return(ret);
}

