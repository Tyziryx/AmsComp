// tokeniser.h : shared definition for tokeniser.l and compilateur.cpp

enum TOKEN {FEOF, UNKNOWN, NUMBER, ID, STRINGCONST, RBRACKET, LBRACKET, RPARENT, LPARENT, COMMA, 
SEMICOLON, DOT, ADDOP, MULOP, RELOP, NOT, ASSIGN,
// tokens pour les mots-clés
IF, THEN, ELSE, WHILE, DO, FOR, TO, BEG, END};
enum TYPE{
    UNSIGNED_INT,
    BOOLEAN,
};

