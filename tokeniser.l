%{
// This is our Lexical tokeniser 
// It should be compiled into cpp with :
// flex++ -d -otokeniser.cpp tokeniser.l 
// And then compiled into object with
// g++ -c tokeniser.cpp
// tokens can be read using lexer->yylex()
// lexer->yylex() returns the type of the lexicon entry (see enum TOKEN in tokeniser.h)
// and lexer->YYText() returns the lexicon entry as a string

#include "tokeniser.h"
#include <iostream>

using namespace std;

%}

%option noyywrap
%option c++
%option yylineno

stringconst  \"[^\n"]+\"
ws      [ \t\n\r]+
alpha   [A-Za-z]
digit   [0-9]
number  {digit}+
double  [0-9]+\.[0-9]*|\.[0-9]+ /* ajout de double pour prendre en compte les "." */
id	{alpha}({alpha}|{digit}|_)*
addop	(\+|\-|\|\|)
mulop	(\*|\/|%|\&\&)
relop	(\<|\>|"=="|\<=|\>=|!=)
unknown [^\"A-Za-z0-9 \n\r\t\(\)\<\>\=\!\%\&\|\}\-\;\.]+

%%
"if"        return IF;     /* Mots-clés en minuscules */
"then"      return THEN;
"else"      return ELSE;
"while"     return WHILE;
"do"        return DO;
"for"       return FOR;
"to"        return TO;
"begin"     return BEG;     /* Mots-clés en minuscules */
"end"       return END;

"IF"        return IF;     /* Mots-clés en majuscules selon la grammaire */
"THEN"      return THEN;
"ELSE"      return ELSE;
"WHILE"     return WHILE;
"DO"        return DO;
"FOR"       return FOR;
"TO"        return TO;
"BEGIN"     return BEG;     /* Mots-clés en majuscules selon la grammaire */
"END"       return END;
"DISPLAY"   return DISPLAY; 
"VAR"       return VAR; 
"INTEGER"   return INTEGER_TYPE; 
"BOOLEAN"   return BOOLEAN_TYPE; 
"DOUBLE"    return DOUBLE_TYPE;
"CHAR"      return CHAR_TYPE; 
":"         return COLON; 



"TRUE"      return TRUE_LIT; 
"FALSE"     return FALSE_LIT; 

{addop}		return ADDOP;
{mulop}		return MULOP;
{relop}		return RELOP;
[0-9]+\.[0-9]*|\.[0-9]+     return DOUBLE_LIT; /* j'ai mis l'instruction au lieu de double car double ne marchait pas */
{number}	return NUMBER;
'[^']'      return CHAR_LIT;  // Reconnaît un caractère entre apostrophes
{id}		return ID;
{stringconst}	return STRINGCONST;
"["		return RBRACKET;
"]"		return LBRACKET;
","		return COMMA;
";"		return SEMICOLON;
"."		return DOT;
":="		return ASSIGN;
"("		return RPARENT;
")"		return LPARENT;
"!"		return NOT;
<<EOF>>		return FEOF;
{ws}    {/* skip blanks and tabs */};
"(*"    { /* Skip comments between '(*' and '*)' */
		int c;
		while((c = yyinput()) != 0){
     			if(c == '*'){
     	    			if((c = yyinput()) == ')')
    	        			break;
    	     			else
  	          			unput(c);
  	   		}	
		}
	};

{unknown}	return UNKNOWN;

%%

