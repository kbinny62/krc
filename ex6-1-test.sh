#!/bin/sh
set -e

EXE=`dirname $0`/ex6-1-getword
CC=`which cc` || cc
$CC ex6-1-getword.c -o "$EXE"


#
# string literal test: 2 int, 1 char, 1 return
#
OUTPUT=`"$EXE" 2>/dev/null <<EOF | tr -s ' \t' ' \t'
int main(int argc, char *argv[]) {
	return strlen("the following 3 keywords will not count: long short void");
}
EOF`

if test "$OUTPUT" != " 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 "; then
	echo "Failed str literal test, expected 2 int, 1 char, 1 return"
	exit 1
fi

#
# comment test: 1 extern, 1 int, 1 sizeof, 1 return
#
OUTPUT=`"$EXE" 2>/dev/null <<EOF | tr -s ' \t' ' \t'
extern char **environ;
/* The follow 2 keywords don't count:
sizeof, void
*/
int main() {
	return sizeof environ;
EOF`

if test "$OUTPUT" != " 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 "; then
	echo "Failed comment test, expected 1 extern, 1 int, 1 sizeof, 1 return"
	exit 1;
fi


#
# underscore and preprocessor keywords test
# 1 void, 2 int, 0 goto, 0 double, 1 if
#
OUTPUT=`"$EXE" 2>/dev/null <<EOF | tr -s ' \t' ' \t'
#include <stdio.h>
#include <goto.h>
void fun(int do_double_run) {
	putchar('X'); if (double_double_run) putchar('X');
}
int main() { fun(); }
EOF`

if test "$OUTPUT" != " 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 2 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 "; then
	echo "Failed underscore/preprocessor test, expected 1 void, 2 int, 1 if";
	exit 1;
fi

echo "Pass"
exit 0
