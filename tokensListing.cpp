#include <iostream>
#include "lex.h"
#include "io.cpp"

/*
 * CS280 - Fall 2020
 */

#include <cctype>
#include <map>
using std::map;

using namespace std;



int main()
{
	LexItem toks[] = {
		LexItem(PRINT,"PRINT",3),
		LexItem(IF,"IF",3),
		LexItem(BEGIN,"BEGIN",3),
		LexItem(END,"END",3),
		LexItem(IDENT,"area",3),
		LexItem(ICONST,"579",3),
		LexItem(SCONST,"hello NJIT",3),
		LexItem(RCONST,"5.79",3),
		LexItem(PLUS,"+",3),
		LexItem(MINUS,"-",3),
		LexItem(MULT,"*",3),
		LexItem(DIV,"/",3),
		LexItem(EQ,"=",3),
		LexItem(LPAREN,"(",3),
		LexItem(RPAREN,")",3),
		LexItem(SCOMA,";",3),
		LexItem(COMA,",",3),
		LexItem(ERR, "ERR", 3),
		LexItem(DONE,"DONE",3),
	};
    
    for( int i = 0; toks[i] != DONE; i++ )
        cout << toks[i] << endl;
    
    return 0;
}
