#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "io.cpp"
#include "lexical.cpp"



using namespace std;



int main(int argc, char *argv[]){

    bool vF = false;
    bool iconstsF = false;
    bool rconstsF = false;
    bool sconstsF = false;
    bool idsF = false;

    vector<string> identVec;
    vector<string> stringVec;
    vector<int> intVec;
    vector<float> realVec;
    vector<LexItem> tokVec;

    istream *in = &cin;
    ifstream myFile;
    string file;
    LexItem tok;
    int lineNumber = 1;
    int tokenCount = 0;



    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "-v")
            vF = true;

        else if (arg == "-iconsts")
            iconstsF = true;

        else if (arg == "-rconsts")
            rconstsF = true;

        else if (arg == "-sconsts")
            sconstsF = true;

        else if (arg == "-ids")
            idsF = true;

        else if (arg[0] == '-') {
            cerr << "UNRECOGNIZED FLAG " << arg << endl;
            return 0;
        }

        else if( in != &cin ) {
            cerr << "ONLY ONE FILE " << endl;
            return 0;
        }

        else {
            myFile.open(arg);
            if( myFile.is_open() == false ) {
                cerr << "COULD NOT OPEN " << arg << endl;
                return 0;
            }

        }
    }



    tok=getNextToken(myFile,lineNumber);
    tokenCount++;

    while(tok.GetToken()!=DONE && tok.GetToken()!=ERR) {

        tok = getNextToken(myFile, lineNumber);
        tokenCount++;

        if (vF){
            cout << tok << endl;
            tok=getNextToken(myFile, lineNumber);
            tokenCount++;
        }

        if (iconstsF && tok.GetToken() == ICONST) {
            intVec.push_back(stoi(tok.GetLexeme()));
        }

        if (rconstsF && tok.GetToken() == RCONST) {
            realVec.push_back(stof(tok.GetLexeme()));
        }

        if (sconstsF && tok.GetToken() == SCONST) {
            stringVec.push_back(tok.GetLexeme());
        }

        if (idsF && tok.GetToken() == IDENT) {
            identVec.push_back(tok.GetLexeme());
        }

        if (tok== ERR) {
            cout << "ERROR " << tok.GetLinenum() << " " << tok.GetLexeme() << endl;
            exit(1);
        }

    }




    if (sconstsF && stringVec.size() > 0) {
        for (int i = 0; i < stringVec.size(); i++)
            cout << stringVec.at(i) << " ";
    }


    if (iconstsF && intVec.size() > 0) {
        for (int i = 0; i < intVec.size(); i++)
            cout << intVec.at(i) << " ";
    }


    if (rconstsF && realVec.size() > 0) {
        for (int i = 0; i < realVec.size(); i++)
            cout << realVec.at(i) << " ";
    }


    if (idsF && identVec.size() > 0) {
        for (int i = 0; i < identVec.size(); i++)
            cout << identVec.at(i) << " ";
    }


    cout << "TOKEN: " << tokenCount;
    cout << "LINE NUMBER " << tok.GetLinenum();


}





