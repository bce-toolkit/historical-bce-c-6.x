/*
 *	Polynomial Subsystem with FMA
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
#include "../include/blockmem.h"
#include "../include/kasprintf.h"
#include "../include/fraction.h"
#include "../include/mmdiv.h"
#include "../include/polynomial.h"

/*  This table was used to generating unknown symbols  */
static char *unknown_table = "abcdefghijklmnopqrstuvwxyz";
#define unknown_table_length 26

exp expression_create(int numerator, int denominator) {
	/*  Generate a new expression with no unknown  */
	exp ret;
	ret.count = 0;
	ret.stack = empty_block_memory(DEFAULT_PAGE_SIZE);
	ret.cst = fraction_create(numerator, denominator);
	return(ret);
}

exp fraction_to_expression(fact src) {
	/*  Call expression_create function to create a new expression  */
	return(expression_create(src.denominator, src.denominator));
}

void free_expression(exp *ptr) {
	/*  Free the stack  */
	free_block_memory(&(ptr->stack));
	/*  AC the counter  */
	ptr->count = 0;
}

void free_expression_stack(exp *src, int count) {
	exp *ptr;
	/*  Free each node  */
	for (ptr = src; ptr < src + count; ptr++)
		free_expression(ptr);
	/*  Free the expression stack  */
	free(src);
}

expnode* query_expression_node(bmem stack, int count, int flag) {
	return(query_expression_node_ex((expnode*)stack.ptr, ((expnode*)stack.ptr) + count - 1, flag));
}

expnode* query_expression_node_ex(expnode *begin, expnode *end, int flag) {
	expnode *ptr;
	/*  Check input values  */
	if (!begin || !end)
		return(NULL);
	if (end - begin <= 1) {
		/*  Only one or two node(s) in the table, just check it or them.  */
		if (begin->flag == flag)
			return(begin);
		if (end->flag == flag)
			return(end);
		return(NULL);
	} else {
		/*  Search the node in the table  */
		ptr = begin + (end - begin) / 2;
		if (flag == ptr->flag) {
			return(ptr);
		} else if (flag < ptr->flag) {
			return(query_expression_node_ex(begin, ptr - 1, flag));
		} else {
			return(query_expression_node_ex(ptr + 1, end, flag));
		}
	}
}

expnode* get_insert_position(expnode *p, int count, int flag) {
	expnode *ptr;
	if (!p)
		return(p);
	/*  Get the position the new node should be located in (the node to be inserted mustn't be existed in the table)  */
	if (flag < p->flag)
		return(p);
	for (ptr = p; ptr < p + count - 1; ptr++)
		if (flag > ptr->flag && flag < (ptr + 1)->flag)
			return(ptr + 1);
	if (flag == ptr->flag)
		return(ptr);
	return(p + count);
}

int push_expression_node(exp *target, expnode nd, factop op) {
	expnode *ptr, *move;
	int offset;
	/*  Check whether the unknown was existed  */
	ptr = query_expression_node(target->stack, target->count, nd.flag);
	if (ptr) {
		ptr->pfx = op(ptr->pfx, nd.pfx);
	} else {
		/*  Get the offset where the new node should be located in  */
		offset = get_insert_position((expnode*)target->stack.ptr, target->count, nd.flag) - (expnode*)target->stack.ptr;

		/*  Reallocate memory spaces  */
		if (reallocate_block_memory(&(target->stack), (target->count + 1) * sizeof(expnode)) != BLOCKMEM_SUCCESS)
			return(EXPMODULE_FAILED);

		/*  Add the new node  */
		for (move = (expnode*)target->stack.ptr + target->count; move > (expnode*)target->stack.ptr + offset; move--)
			*move = *(move - 1);
		nd.pfx = op(F_ZERO, nd.pfx);
		*((expnode*)target->stack.ptr + offset) = nd;
		target->count++;
	}
	return(EXPMODULE_SUCCESS);
}

int push_expression_node_ex(exp *target, int flag, fact pfx, factop op) {
	expnode nd;
	/*  Generate a new node  */
	nd.flag = flag;
	nd.pfx = pfx;
	return(push_expression_node(target, nd, op));
}

void push_expression_constant(exp *target, fact src, factop op) {
	target->cst = op(target->cst, src);
}

void expression_vf_basic(exp *target, fact src, factop op) {
	expnode *ptr;
	push_expression_constant(target, src, op);
	for (ptr = (expnode*)target->stack.ptr; ptr < (expnode*)target->stack.ptr + target->count; ptr++)
		ptr->pfx = op(ptr->pfx, src);
}

void expression_plus(exp *target, fact src) {
	push_expression_constant(target, src, fraction_plus);
}

void expression_minus(exp *target, fact src) {
	push_expression_constant(target, src, fraction_minus);
}

void expression_multiplination(exp *target, fact src) {
	expression_vf_basic(target, src, fraction_multiplination);
}

void expression_division(exp *target, fact src) {
	expression_vf_basic(target, src, fraction_division);
}

int simplify_expression_node(exp *target) {
	int cnt = 0;
	bmem newstack;
	expnode *ptr, *tgptr;

	/*  Pre-calculate the size which needs to be allocated  */
	for (ptr = (expnode*)target->stack.ptr; ptr < (expnode*)target->stack.ptr + target->count; ptr++)
		if (fraction_compare(ptr->pfx, F_ZERO) != 0)
			cnt++;

	if (cnt == target->count)
		return(EXPMODULE_SUCCESS);

	/*  Copy the data  */
	if (!cnt) {
		free_expression(target);
	} else {
		if (allocate_block_memory(&newstack, cnt * sizeof(expnode), DEFAULT_PAGE_SIZE) != BLOCKMEM_SUCCESS)
			return(EXPMODULE_FAILED);
		for (ptr = (expnode*)target->stack.ptr, tgptr = (expnode*)newstack.ptr; ptr < (expnode*)target->stack.ptr + target->count; ptr++)
			if (fraction_compare(ptr->pfx, F_ZERO) != 0)
				*(tgptr++) = *ptr;
		free_block_memory(&(target->stack));
		target->stack = newstack;
		target->count = cnt;
	}
	return(EXPMODULE_SUCCESS);
}

int expression_double_operation(exp *exp1, factop op1, exp exp2, factop op2, fact ft) {
	expnode *ptr;
	/*  Do operation: exp1 = exp1 op1 (exp2 op2 ft)  */
	/*  op1 = {fraction_plus, fraction_minus), op2 = {fraction_multiplination, fraction_division}  */
	push_expression_constant(exp1, op2(exp2.cst, ft), op1);
	for (ptr = (expnode*)exp2.stack.ptr; ptr < (expnode*)exp2.stack.ptr + exp2.count; ptr++)
		if (push_expression_node_ex(exp1, ptr->flag, op2(ptr->pfx, ft), op1) != EXPMODULE_SUCCESS)
			return(EXPMODULE_FAILED);
	return(EXPMODULE_SUCCESS);
}

void finishing_expression_stack(exp *src, int len) {
	expnode **fsort, **sp;
	exp *ptr;
	int min_flag, sorted = 0, mf;

	fsort = (expnode**) malloc(len * sizeof(expnode*));
	if (!fsort)
		return;

	/*  Initialize  */
	for (ptr = src, sp = fsort; ptr < src + len; ptr++, sp++)
		*sp = (expnode*) ptr->stack.ptr;

	/*  Sort them and make their ID continous  */
	do {
		for (mf = EXPMODULE_FALSE, ptr = src, sp = fsort; ptr < src + len; ptr++, sp++)
			if (*sp < (expnode*)ptr->stack.ptr + ptr->count) {
				if (mf == EXPMODULE_FALSE) {
					mf = EXPMODULE_TRUE;
					min_flag = (*sp)->flag;
				} else {
					if ((*sp)->flag < min_flag)
						min_flag = (*sp)->flag;
				}
			}
		if (mf == EXPMODULE_TRUE)
			for (sorted++, ptr = src, sp = fsort; ptr < src + len; ptr++, sp++)
				if (*sp < (expnode*)ptr->stack.ptr + ptr->count)
					if ((*sp)->flag == min_flag) {
						(*sp)->flag = sorted;
						(*sp)++;
					}
	} while(mf == EXPMODULE_TRUE);

	free(fsort);
}

int expression_memcpy(exp *target, exp src, int destroy_after_copy) {
	free_expression(target);
	*target = src;
	if (src.count) {
		if (allocate_block_memory(&(target->stack), src.count * sizeof(expnode), DEFAULT_PAGE_SIZE) != BLOCKMEM_SUCCESS)
			return(EXPMODULE_FAILED);
		memcpy(target->stack.ptr, src.stack.ptr, src.count * sizeof(expnode));
	}
	if (destroy_after_copy == EXPMODULE_TRUE)
		free_expression(&src);
	return(EXPMODULE_SUCCESS);
}

void expression_to_number(exp *src, int len, int inc_args) {
	exp *ptr;
	expnode *subptr;
	fact mt;
	int ml = 1;
	for (ptr = src; ptr < src + len; ptr++) {
		ml = mmul(ptr->cst.denominator, ml);
		if (inc_args == EXPMODULE_TRUE)
			for (subptr = (expnode*)ptr->stack.ptr; subptr < (expnode*)ptr->stack.ptr + ptr->count; subptr++)
				ml = mmul(subptr->pfx.denominator, ml);
	}
	mt = fraction_create(ml < 0 ? -ml : ml, 1);
	for (ptr = src; ptr < src + len; ptr++)
		expression_multiplination(ptr, mt);
}

char *get_unknown_symbol(int id) {
	int temp = id, cnt = 0;
	char *ret, *ptr;
	id--;
	temp--;
	do {
		cnt++;
		temp /= unknown_table_length;
	} while (temp != 0);
	ret = (char*) malloc((cnt + 1) * sizeof(char));
	if (!ret)
		return(NULL);
	ptr = ret + cnt - 1;
	*(ret + cnt) = '\0';
	do {
		*(ptr--) = unknown_table[id % unknown_table_length];
		id /= unknown_table_length;
	} while (id != 0);
	return(ret);
}

char* sprint_expression(exp sz) {
	char *new, *temp, *symbol;
	expnode *szptr;
	fact pfx;
	/*  Build a new string  */
	new = (char*) malloc(sizeof(char));
	if (!new)
		return(NULL);
	*new = '\0';
	for (szptr = (expnode*)sz.stack.ptr; szptr < (expnode*)sz.stack.ptr + sz.count; szptr++) {
		pfx = szptr->pfx;
		symbol = get_unknown_symbol(szptr->flag);
		if (!symbol) {
			free(new);
			return(NULL);
		}
		temp = new;
		if (pfx.numerator % pfx.denominator == 0) {
			if (pfx.numerator / pfx.denominator == 1) {
				kasprintf(&new, "%sX%s+", new, symbol);
			} else {
				if (pfx.numerator / pfx.denominator != 0 && pfx.numerator / pfx.denominator != -1) {
					if (strlen(new) > 0 && pfx.numerator / pfx.denominator < 0)
						if (*(new + strlen(new) - 1) == '+')
							*(new + strlen(new) - 1) = '\0';
					kasprintf(&new, "%s%dX%s+", new, pfx.numerator / pfx.denominator, symbol);
				} else {
					kasprintf(&new, "%s-X%s+", new, symbol);
				}
			}
		} else {
			kasprintf(&new, "%s(%d/%d)X%s+", new, pfx.numerator, pfx.denominator, symbol);
		}
		free(symbol);
		if (new != temp)
			free(temp);
		if (!new)
			return(NULL);
	}
	/*  Print out the constant  */
	pfx = sz.cst;
	temp = new;
	if (fraction_compare(pfx, F_ZERO) < 0) {
		if (pfx.numerator % pfx.denominator == 0)
			kasprintf(&new, "%s%d", new, pfx.numerator / pfx.denominator);
		else
			kasprintf(&new, "%s%d/%d", new, pfx.numerator, pfx.denominator);
	} else if (fraction_compare(pfx, F_ZERO) > 0) {
		if (strlen(new) > 0)
			*(new + strlen(new) - 1) = '\0';
		if (pfx.numerator % pfx.denominator == 0) {
			kasprintf(&new, "%s%d", new, pfx.numerator / pfx.denominator);
		} else {
			pfx.numerator = pfx.numerator > 0 ? -pfx.numerator : pfx.numerator;
			pfx.denominator = pfx.denominator < 0 ? -pfx.denominator : pfx.denominator;
			kasprintf(&new, "%s%d/%d", new, pfx.numerator, pfx.denominator);
		}
	} else {
		if (strlen(new) > 0)
			*(new + strlen(new) - 1) = '\0';
	}
	if (new != temp)
		free(temp);
	return(new);
}

