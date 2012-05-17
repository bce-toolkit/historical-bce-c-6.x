/*
 *	Extra String Library
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

#ifndef STRLIB_VALUES_DEFINED
	#define STRLIB_TRUE 1
	#define STRLIB_FALSE 0
	#define strplen(begin, end) ((end)-(begin)+1)
	#define STRLIB_VALUES_DEFINED
#endif

char *strpush(const char *begin, const char *end);
void pspush(const char *begin, const char *end);
int strpcomp(const char *begin, const char *end, const char *data);
int strpisnum(const char *begin, const char *end);
int strpqcomp(const char *begin1, const char *end1, const char *begin2, const char *end2);
int strptoi(char *begin, char *end, char **endptr);

