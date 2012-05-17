/*
 *	FCE - a functional shell for BCE kernel
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

#define FCE_MANUAL "FCE - Manual\n\nBefore using fce, you have to read this manual.\n\nSynopsis:\n	fce [-o [filename]] [-m [1/2/3]] [filename[filename] (or stdin)] ...\n\nDescription:\n	fce is a command-line chemical equation balancer. It supports chemical equations and ion equation.\n\nOptions:\n	-h\n		Help: show syntax.\n	-o [filename]\n		Select a file which you want to save the answers in\n	-m\n		Selecte an answer format (from 1 - 3), The default is one.\n		'1' means 'only balanced-prefix-number'\n		'2' means 'only balanced-chemical-equation'\n		'3' means 'both ok'\n\nSample:\n	fce stdin\n		will wait for user to type the chemical equation, balance it and print the answers on the screen\n	fce -o fce.out stdin\n		will wait for user to type the chemical equation, balance it and save the answers to fce.out\n	fce -o fce.out -m 2 fce.in\n		will balance chemical equations listed in fce.in and save the answers to fce.out\n\nAuthor:\n	FCE is by Ji WenCong <whs_jwc@163.com>\n"
#define FCE_LANG_101 "FCE: No parameters, please type 'fce -h' for more help!\n"
#define FCE_LANG_102 "ERROR: Too many output files have been defined.\n"
#define FCE_LANG_103 "ERROR: Can't open file '%s'.\n"
#define FCE_LANG_104 "stdin"
#define FCE_LANG_105 "ERROR: Invalid mode.\n"
#define FCE_LANG_106 "ERROR: I/O error.\n"
#define FCE_LANG_107 "exit"
#define FCE_LANG_108 '#'
#define FCE_LANG_109 "-\n"
#define FCE_PROMPT "[CE]$ "
#define FCE_LANG_201 "-o"
#define FCE_LANG_202 "-m"
#define FCE_LANG_203 "-h"
#define FCE_LANG_204 "mode 1"
#define FCE_LANG_205 "mode 2"
#define FCE_LANG_206 "mode 3"
#define FCE_LANG_207 "-noprompt"

#define ERRNO_SUCCEED 0
#define ERRNO_TOO_MANY_OFILES 101
#define ERRNO_INVALID_MODE 102
#define ERRNO_FILE_IO 103

int main(int argc, char *argv[], char *envp[]);

