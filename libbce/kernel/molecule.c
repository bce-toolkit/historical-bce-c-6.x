/*
 *	Molecule Analyzer
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
#include <stdlib.h>
#include <ctype.h>
#include "../include/blockmem.h"
#include "../include/string_ext.h"
#include "../include/element.h"
#include "../include/molecule.h"

int solve_molecule_hydrate(char *begin, char *end, bmem *eptr, int *ecount, int suffix) {
	char *dot = begin;
	int ret, b_stack = 0;

	for (; begin <= end; begin++) {
		switch (*begin) {
			case '(':
				b_stack++;
				break;
			case ')':
				b_stack--;
				break;
		}
		if (b_stack != 0)
			continue;

		if (*begin == '.' || begin == end) {
			ret = solve_molecule(dot, begin == end ? begin : begin - 1, eptr, ecount, suffix);
			if (ret != ERRNO_MOLECULE_SUCCESS)
				return(ret);
			dot = begin + 1;
		}
	}

	if (b_stack != 0)
		return(ERRNO_MOLECULE_SYNTAX);
	else
		return(ERRNO_MOLECULE_SUCCESS);
}

int solve_molecule(char *begin, char *end, bmem *eptr, int *ecount, int suffix) {
	char *ptr, *b_ptr, *temp, *start;
	int b_stack, p_ret, b_prefix;
	element *eap_ptr;
	if (strplen(begin, end) <= 0)
		return(ERRNO_MOLECULE_SUCCESS);

	/*  Get the prefix number  */
	b_prefix = strptoi(begin, end, &temp);
	if (!temp)
		return(ERRNO_MOLECULE_MEMORY);

	if (b_prefix <= 0)
		b_prefix = 1;

	/*  Fix the string  */
	if (temp != begin) {
		start = temp;
		suffix *= b_prefix;
	} else {
		start = begin;
	}

	if (strplen(start, end) <= 0)
		return(ERRNO_MOLECULE_SUCCESS);

	if (strpcomp(start, end, SPECIAL_SIGNATURE_SOLID) == STRLIB_TRUE || strpcomp(start, end, SPECIAL_SIGNATURE_LIQUIT) == STRLIB_TRUE || strpcomp(start, end, SPECIAL_SIGNATURE_GAS) == STRLIB_TRUE)
		return(ERRNO_MOLECULE_SUCCESS);

	/*  Recognize whether the string is a electronic expression  */
	if (strpcomp(start, end, ELECTRIC_E_PLUS) == STRLIB_TRUE || strpcomp(start, end, ELECTRIC_E_MINUS) == STRLIB_TRUE) {
		for (eap_ptr = (element*)eptr->ptr; eap_ptr < (element*)eptr->ptr + *ecount; eap_ptr++)
			if (strpcomp(eap_ptr->begin, eap_ptr->end, ELECTRIC_E_NAME) == STRLIB_TRUE) {
				/*  Modify the value  */
				eap_ptr->count += strpcomp(start, end, ELECTRIC_E_PLUS) == STRLIB_TRUE ? suffix : -suffix;
				return(ERRNO_MOLECULE_SUCCESS);
			}

		/*  Add the electronic descriptor in the element stack  */
		if (reallocate_block_memory(eptr, (++(*ecount)) * sizeof(element)) != BLOCKMEM_SUCCESS)
			return(ERRNO_MOLECULE_MEMORY);

		eap_ptr = (element*)eptr->ptr + *ecount - 1;

		eap_ptr->begin = start;
		eap_ptr->end = start;
		eap_ptr->count = (strpcomp(start, end, ELECTRIC_E_PLUS) == STRLIB_TRUE) ? suffix : -suffix;

		return(ERRNO_MOLECULE_SUCCESS);
	}

	/*  Solve the bracket  */
	for (b_stack = 0, ptr = start; ptr <= end; ptr++) {
		if (*ptr != '(')
			continue;

		for (b_ptr = ptr; b_ptr <= end; b_ptr++) {
			switch (*b_ptr) {
				case '(':
					b_stack++;
					break;
				case ')':
					b_stack--;
					break;
			}

			if (b_stack == 0) {
				/*  Solve the molecular on the left  */
				p_ret = solve_molecule_hydrate(start, ptr - 1, eptr, ecount, suffix);
				if (p_ret != ERRNO_MOLECULE_SUCCESS)
					return(p_ret);

				/*  Get the suffix after the bracket  */
				b_prefix = strptoi(b_ptr + 1, end, &temp);

				if (!temp)
					return(ERRNO_MOLECULE_MEMORY);

				if (b_prefix <= 0)
					b_prefix = 1;

				/*  Solve the molecular in the bracket  */
				p_ret = solve_molecule_hydrate(ptr + 1, b_ptr - 1, eptr, ecount, suffix * b_prefix);
				if (p_ret != ERRNO_MOLECULE_SUCCESS)
					return(p_ret);

				/*  Solve the molecular on the right  */
				p_ret = solve_molecule_hydrate(temp, end, eptr, ecount, suffix);
				if (p_ret != ERRNO_MOLECULE_SUCCESS)
					return(p_ret);

				return (ERRNO_MOLECULE_SUCCESS);
			}
		}
	}

	/*  Look for the next upcase character  */
	for (ptr = start + 1; strplen(ptr, end) > 0 && (!isupper((int)(*ptr))); ptr++)
		;

	if (strplen(ptr, end) <= 0) {
		/*  Only one element, divide it into a symbol and a number  */
		for (ptr = start; strplen(ptr, end) >= 0; ptr++) {
			if (strpisnum(ptr, end) == STRLIB_TRUE) {
				/*  Get the suffix number  */
				b_prefix = strptoi(ptr, end, NULL);
				if (b_prefix <= 0)
					b_prefix = 1;

				for (eap_ptr = (element*)eptr->ptr; eap_ptr < (element*)eptr->ptr + *ecount; eap_ptr++)
					if (strpqcomp(eap_ptr->begin, eap_ptr->end, start, ptr - 1) == STRLIB_TRUE) {
						/*  Modify the value  */
						eap_ptr->count += suffix * b_prefix;
						return(ERRNO_MOLECULE_SUCCESS);
					}

				/*  Add the electronic descriptor in the element stack  */
				if (reallocate_block_memory(eptr, (++(*ecount)) * sizeof(element)) != BLOCKMEM_SUCCESS)
					return(ERRNO_MOLECULE_MEMORY);

				eap_ptr = (element*)eptr->ptr + *ecount - 1;

				eap_ptr->begin = start;
				eap_ptr->end = ptr - 1;
				eap_ptr->count = b_prefix * suffix;

				break;
			}
		}
	} else {
		p_ret = solve_molecule_hydrate(start, ptr - 1, eptr, ecount, suffix);
		if (p_ret != ERRNO_MOLECULE_SUCCESS)
			return(p_ret);

		p_ret = solve_molecule_hydrate(ptr, end, eptr, ecount, suffix);
		if (p_ret != ERRNO_MOLECULE_SUCCESS)
			return(p_ret);
	}

	return(ERRNO_MOLECULE_SUCCESS);
}

