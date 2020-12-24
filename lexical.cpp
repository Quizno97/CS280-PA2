#include <iostream>
#include <regex>
#include <map>

#include "lex.h"


using namespace std;



inline LexItem getNextToken(istream& in, int& linenum)
{
    LexItem currToken;
    LexItem prevToken;

    enum TokState {START, INID, INSTRING, ININT, INREAL, INCOMMENT, SIGN}
            lexstate = START;


    string lexeme;
    char ch;

    while (in.get(ch))
    {
        switch (lexstate)
        {
            case START:
                if (ch == '\n')
                    linenum++;

                if (in.peek() == -1)
                    return LexItem(DONE, lexeme, linenum);


                if (isspace(ch))
                    continue;

                lexeme = ch;

                if (isalpha(ch))
                {
                    lexstate = INID;
                    continue;
                }

                if (ch == '\"')
                {
                    lexstate = INSTRING;
                    continue;
                }

                if (isdigit(ch))
                {
                    lexstate = ININT;
                    continue;
                }

                if (ch == '.')
                {
                    lexstate = INREAL;
                    continue;
                }

                if (ch == '/' && char(in.peek() == '/'))
                {
                    lexstate = INCOMMENT;
                    continue;
                }

                if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' ||
                    ch == '(' || ch == ')' || ch == ',' || ch == ';')
                {
                    lexstate = SIGN;
                    continue;
                }

                return LexItem(ERR, lexeme, linenum);


            case INID:
                if (regex_match(lexeme + ch, regex("[a-zA-Z][a-zA-z0-9]*")))
                    lexeme += ch;

                if (in.peek() == -1 || !regex_match(lexeme + ch, regex("[a-zA-Z][a-zA-Z0-9]*")))
                {
                    lexstate = START;
                    in.putback(ch);

                    if (lexeme == "begin")
                    {
                        if (prevToken.GetToken() != ERR)
                            return LexItem(ERR, lexeme, linenum);

                        currToken = LexItem(BEGIN, lexeme, linenum);
                        prevToken = currToken;
                    }

                    else if (lexeme == "print") {
                        currToken = LexItem(PRINT, lexeme, linenum);
                        prevToken = currToken;

                    }

                    else if (lexeme == "end")
                    {
                        currToken = LexItem(END, lexeme, linenum);
                        prevToken = currToken;
                    }

                    else if (lexeme == "if") {
                        currToken = LexItem(IF, lexeme, linenum);
                        prevToken = currToken;
                    }

                    else if (lexeme == "then") {
                        currToken = LexItem(THEN, lexeme, linenum);
                        prevToken = currToken;
                    }

                    else
                    {
                        if (prevToken.GetToken() == IDENT)
                            return LexItem(ERR, lexeme, linenum);

                        return LexItem(IDENT, lexeme, linenum);
                    }


                    if (currToken != BEGIN && prevToken == ERR)
                        return LexItem(ERR, "No BEGIN Token", currToken.GetLinenum());

                    return currToken;

                }

                break;


            case INSTRING:
                if (ch == 10)
                    return LexItem(ERR, lexeme, linenum);


                if (regex_match(lexeme + ch, regex("\"[ -~]*")))
                {
                    if (ch == '\\' && in.peek() == '\"')
                    {
                        lexeme += ch;
                        in.get(ch);
                        lexeme += ch;
                        continue;
                    }
                    else
                        lexeme += ch;
                }


                if (regex_match(lexeme + ch, regex("\"[ -~]*\"")))
                {
                    lexstate = START;
                    return LexItem(SCONST, lexeme, linenum);
                }

                break;


            case ININT:

                if (isalpha(ch))
                    return LexItem(ERR, lexeme + ch, linenum);

                if (regex_match(lexeme + ch, regex("[0-9]+")))
                    lexeme += ch;


                else if(ch == '.')
                {
                    lexstate = INREAL;
                    in.putback(ch);

                    continue;
                }

                else
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(ICONST, lexeme, linenum);
                }

                break;


            case INREAL:
                if (isalpha(ch))
                    return LexItem(ERR, lexeme + ch, linenum);

                if (regex_match(lexeme + ch, regex("[0-9]*\\.[0-9]+")))
                    lexeme += ch;

                else if (regex_match(lexeme + ch, regex("[0-9]*\\.[0-9]*")))
                    lexeme += ch;

                else
                {
                    if (lexeme[lexeme.length() - 1] == '.')
                        return LexItem(ERR, lexeme, linenum);

                    lexstate = START;
                    in.putback(ch);

                    return LexItem(RCONST, lexeme, linenum);
                }

                break;


            case INCOMMENT:
                if (ch == '\n')
                {
                    linenum++;
                    lexstate = START;
                }

                continue;


            case SIGN:
                if (lexeme == "+" || lexeme == "*" || lexeme == "/")
                {
                    lexstate = START;
                    in.putback(ch);

                    if (lexeme == "+")
                        return LexItem(PLUS, lexeme, linenum);

                    else if (lexeme == "*")
                        return LexItem(MULT, lexeme, linenum);

                    else
                        return LexItem(DIV, lexeme, linenum);
                }

                if (lexeme == "-")
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(MINUS, lexeme, linenum);
                }

                if (lexeme == "=")
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(EQ, lexeme, linenum);
                }

                if (lexeme == "(")
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(LPAREN, lexeme, linenum);
                }

                if (lexeme == ")")
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(RPAREN, lexeme, linenum);
                }

                if (lexeme == ",")
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(COMA, lexeme, linenum);
                }

                if (lexeme == ";")
                {
                    lexstate = START;
                    in.putback(ch);

                    return LexItem(SCOMA, lexeme, linenum);
                }

                break;

        }
    }

    return LexItem(DONE, "", linenum);

}



