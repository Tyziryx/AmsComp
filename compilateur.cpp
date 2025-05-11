//  A compiler from a very simple Pascal-like structured language LL(k)
//  to 64-bit 80x86 Assembly langage
//  Copyright (C) 2019 Pierre Jourlin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

// Build with "make compilateur"

// =========================================================================
// SECTION 1: INCLUSIONS ET ESPACES DE NOMS
// =========================================================================

#include <string>          // Pour manipuler les chaînes de caractères
#include <iostream>        // Pour l'entrée/sortie standard (cin, cout)
#include <cstdlib>         // Pour les fonctions standard comme exit()
#include <set>             // Pour stocker l'ensemble des variables déclarées
#include <FlexLexer.h>     // Pour l'analyseur lexical Flex
#include "tokeniser.h"     // Définit les types de tokens (enum TOKEN)
#include <cstring>         // Pour strcmp() utilisé pour comparer les opérateurs

using namespace std;

// =========================================================================
// SECTION 2: CONSTANTES ET TYPES
// =========================================================================

// Enumération pour les différents opérateurs relationnels
enum OPREL {
    EQU,    // égal (==)
    DIFF,   // différent (!=)
    INF,    // inférieur (<)
    SUP,    // supérieur (>)
    INFE,   // inférieur ou égal (<=)
    SUPE,   // supérieur ou égal (>=)
    WTFR    // opérateur relationnel inconnu (What The F... Relation)
};

// Enumération pour les différents opérateurs additifs
enum OPADD {
    ADD,    // addition (+)
    SUB,    // soustraction (-)
    OR,     // ou logique (||)
    WTFA    // opérateur additif inconnu (What The F... Addition)
};

// Enumération pour les différents opérateurs multiplicatifs
enum OPMUL {
    MUL,    // multiplication (*)
    DIV,    // division (/)
    MOD,    // modulo (%)
    AND,    // et logique (&&)
    WTFM    // opérateur multiplicatif inconnu (What The F... Multiplication)
};

// =========================================================================
// SECTION 3: VARIABLES GLOBALES
// =========================================================================

TOKEN current;             // Token courant analysé (défini dans tokeniser.h)

// Création de l'analyseur lexical Flex qui lira depuis l'entrée standard
FlexLexer* lexer = new yyFlexLexer; 
// Note: lexer->yylex() renvoie le type du token suivant
// et lexer->YYText() renvoie le texte du token sous forme de chaîne

// Ensemble des variables déclarées dans le programme source
set<string> DeclaredVariables;

// Compteur pour générer des étiquettes uniques dans le code assembleur
unsigned long TagNumber = 0;

// =========================================================================
// SECTION 4: FONCTIONS UTILITAIRES
// =========================================================================

// Vérifie si un identifiant a été déclaré
bool IsDeclared(const char *id){
    // Recherche l'identifiant dans l'ensemble des variables déclarées
    return DeclaredVariables.find(id) != DeclaredVariables.end();
}

// Affiche un message d'erreur et quitte le programme
void Error(string s){
    cerr << "Ligne n°" << lexer->lineno() << ", lu : '" << lexer->YYText() 
         << "'(" << current << "), mais ";
    cerr << s << endl;
    exit(-1);  // Quitte avec code d'erreur
}

// Déclarations anticipées
void Statement(void);
void IfStatement(void);
void WhileStatement(void);
void ForStatement(void);
void BlockStatement(void);
TYPE Expression(void);
TYPE SimpleExpression(void);
TYPE Term(void);
TYPE Factor(void);

// =========================================================================
// SECTION 5: GRAMMAIRE DU LANGAGE
// =========================================================================

// La grammaire du langage source est définie comme suit:
// Program := [DeclarationPart] StatementPart
// DeclarationPart := "[" Letter {"," Letter} "]"
// StatementPart := Statement {";" Statement} "."

// Statement := AssignementStatement | IfStatement | WhileStatement | ForStatement | BlockStatement
// AssignementStatement := Letter ":=" Expression
// IfStatement := "if" Expression "then" Statement ["else" Statement]
// WhileStatement := "while" Expression "do" Statement
// ForStatement := "for" Letter ":=" Expression "to" Expression "do" Statement
// BlockStatement := "begin" Statement {";" Statement} "end"

// Expression := SimpleExpression [RelationalOperator SimpleExpression]
// SimpleExpression := Term {AdditiveOperator Term}
// Term := Factor {MultiplicativeOperator Factor}
// Factor := Number | Letter | "(" Expression ")"| "!" Factor
// Number := Digit{Digit}

// AdditiveOperator := "+" | "-" | "||"
// MultiplicativeOperator := "*" | "/" | "%" | "&&"
// RelationalOperator := "==" | "!=" | "<" | ">" | "<=" | ">="  
// Digit := "0"|"1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"
// Letter := "a"|...|"z"

// =========================================================================
// SECTION 6: FONCTIONS D'ANALYSE SYNTAXIQUE ET DE GÉNÉRATION DE CODE
// =========================================================================

// Traite un identifiant (variable) et génère le code pour l'empiler
TYPE Identifier(void){
    cout << "\tpush " << lexer->YYText() << endl;
    current = (TOKEN) lexer->yylex();
    return UNSIGNED_INT; // For now, return UNSIGNED_INT for all identifiers
}

// Traite un nombre et génère le code pour l'empiler
TYPE Number(void){
    // Génère une instruction pour pousser la valeur littérale sur la pile
    // Note: le $ indique une valeur immédiate en assembleur x86
    cout << "\tpush $" << atoi(lexer->YYText()) << endl;
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    return UNSIGNED_INT;
}

// Déclaration anticipée: Expression sera définie plus tard
TYPE Expression(void);  // Appelé par Factor() et utilisera à son tour Term()

// Analyse un facteur (nombre, variable ou expression parenthésée)
TYPE Factor(void){
    TYPE result_type;
    
    // Ajout du support de l'opérateur NOT
    if(current == NOT){
        // Cas d'une négation: !Factor
        current = (TOKEN) lexer->yylex();  // Consomme le '!'
        result_type = Factor();            // Analyse le facteur à nier
        
        // Vérifier que l'opérande est de type BOOLEAN
        if (result_type != BOOLEAN) {
            Error("L'opérande de '!' doit être de type booléen");
        }
        
        // Générer le code pour la négation
        cout << "\tpop %rax" << endl;                  // Récupère la valeur
        cout << "\tnot %rax" << endl;                  // NOT bit à bit
        cout << "\tpush %rax\t# NOT" << endl;          // Empile le résultat
        
        return BOOLEAN;
    }
    else if(current == RPARENT){
        // Cas d'une expression parenthésée: (Expression)
        current = (TOKEN) lexer->yylex();  // Consomme la parenthèse ouvrante
        result_type = Expression();        // Analyse l'expression
        if(current != LPARENT)
            Error("')' était attendu");    // Vérifie la parenthèse fermante
        else
            current = (TOKEN) lexer->yylex(); // Consomme la parenthèse fermante
    }
    else if (current == NUMBER)
        // Cas d'un nombre
        result_type = Number();
    else if (current == ID)
        // Cas d'un identifiant
        result_type = Identifier();
    else {
        Error("'(' ou chiffre ou lettre attendue");
        result_type = UNSIGNED_INT; // Default value, never reached due to Error()
    }
    
    return result_type;
}

// Analyse un opérateur multiplicatif et renvoie son type
OPMUL MultiplicativeOperator(void){
    OPMUL opmul;
    
    // Identifie le type d'opérateur multiplicatif
    if(strcmp(lexer->YYText(), "*") == 0)
        opmul = MUL;
    else if(strcmp(lexer->YYText(), "/") == 0)
        opmul = DIV;
    else if(strcmp(lexer->YYText(), "%") == 0)
        opmul = MOD;
    else if(strcmp(lexer->YYText(), "&&") == 0)
        opmul = AND;
    else 
        opmul = WTFM;  // Opérateur inconnu
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    return opmul;
}

// Analyse un terme: Factor {MultiplicativeOperator Factor}
TYPE Term(void){
    OPMUL mulop;
    TYPE left_type, right_type;
    
    // Analyse le premier facteur
    left_type = Factor();
    
    // Tant qu'on trouve un opérateur multiplicatif, on continue l'analyse
    while(current == MULOP){
        mulop = MultiplicativeOperator();  // Sauvegarde l'opérateur
        right_type = Factor();             // Analyse le facteur suivant
        
        // Vérification de type
        if (mulop == AND) {
            // Pour l'opérateur AND, les deux opérandes doivent être de type BOOLEAN
            if (left_type != BOOLEAN || right_type != BOOLEAN) {
                Error("Les opérandes de '&&' doivent être de type booléen");
            }
            // Le résultat est de type BOOLEAN
            left_type = BOOLEAN;
        } else {
            // Pour les opérateurs arithmétiques (MUL, DIV, MOD), les deux opérandes doivent être de type UNSIGNED_INT
            if (left_type != UNSIGNED_INT || right_type != UNSIGNED_INT) {
                Error("Les opérandes des opérateurs arithmétiques doivent être de type entier");
            }
            // Le résultat reste de type UNSIGNED_INT
            left_type = UNSIGNED_INT;
        }
        
        // Génération du code assembleur pour l'opération multiplicative
        cout << "\tpop %rbx" << endl;      // Récupère le deuxième opérande
        cout << "\tpop %rax" << endl;      // Récupère le premier opérande
        
        // Traitement spécifique selon l'opérateur
        switch(mulop){
            case AND:
                cout << "\tmulq	%rbx" << endl;        // ET logique implémenté comme mult
                cout << "\tpush %rax\t# AND" << endl;  // Empile le résultat
                break;
            case MUL:
                cout << "\tmulq	%rbx" << endl;        // Multiplication
                cout << "\tpush %rax\t# MUL" << endl;  // Empile le résultat
                break;
            case DIV:
                cout << "\tmovq $0, %rdx" << endl;     // Partie haute du dividende = 0
                cout << "\tdiv %rbx" << endl;          // Division, quotient dans %rax
                cout << "\tpush %rax\t# DIV" << endl;  // Empile le résultat (quotient)
                break;
            case MOD:
                cout << "\tmovq $0, %rdx" << endl;     // Partie haute du dividende = 0
                cout << "\tdiv %rbx" << endl;          // Division, reste dans %rdx
                cout << "\tpush %rdx\t# MOD" << endl;  // Empile le résultat (reste)
                break;
            default:
                Error("opérateur multiplicatif attendu");
        }
    }
    
    return left_type;
}

// Analyse un opérateur additif et renvoie son type
OPADD AdditiveOperator(void){
    OPADD opadd;
    
    // Identifie le type d'opérateur additif
    if(strcmp(lexer->YYText(), "+") == 0)
        opadd = ADD;
    else if(strcmp(lexer->YYText(), "-") == 0)
        opadd = SUB;
    else if(strcmp(lexer->YYText(), "||") == 0)
        opadd = OR;
    else 
        opadd = WTFA;  // Opérateur inconnu
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    return opadd;
}

// Analyse une expression simple: Term {AdditiveOperator Term}
TYPE SimpleExpression(void){
    OPADD adop;
    TYPE left_type, right_type;
    
    // Analyse le premier terme
    left_type = Term();
    
    // Tant qu'on trouve un opérateur additif, on continue l'analyse
    while(current == ADDOP){
        adop = AdditiveOperator();  // Sauvegarde l'opérateur
        right_type = Term();        // Analyse le terme suivant
        
        // Vérification de type
        if (adop == OR) {
            // Pour l'opérateur OR, les deux opérandes doivent être de type BOOLEAN
            if (left_type != BOOLEAN || right_type != BOOLEAN) {
                Error("Les opérandes de '||' doivent être de type booléen");
            }
            // Le résultat est de type BOOLEAN
            left_type = BOOLEAN;
        } else {
            // Pour les opérateurs arithmétiques (ADD, SUB), les deux opérandes doivent être de type UNSIGNED_INT
            if (left_type != UNSIGNED_INT || right_type != UNSIGNED_INT) {
                Error("Les opérandes des opérateurs arithmétiques doivent être de type entier");
            }
            // Le résultat reste de type UNSIGNED_INT
            left_type = UNSIGNED_INT;
        }
        
        // Génération du code assembleur pour l'opération additive
        cout << "\tpop %rbx" << endl;  // Récupère le deuxième opérande
        cout << "\tpop %rax" << endl;  // Récupère le premier opérande
        
        // Traitement spécifique selon l'opérateur
        switch(adop){
            case OR:
                // OR logique implémenté comme une addition (non standard)
                cout << "\taddq %rbx, %rax\t# OR" << endl;
                break;            
            case ADD:
                // Addition
                cout << "\taddq %rbx, %rax\t# ADD" << endl;
                break;            
            case SUB:
                // Soustraction    
                cout << "\tsubq %rbx, %rax\t# SUB" << endl;
                break;
            default:
                Error("opérateur additif inconnu");
        }
        
        // Empile le résultat
        cout << "\tpush %rax" << endl;
    }
    
    return left_type;
}

// Analyse la partie déclaration: "[" Ident {"," Ident} "]"
void DeclarationPart(void){
    // Vérifie si le premier token est bien un crochet ouvrant
    if(current != RBRACKET)
        Error("caractère '[' attendu");
        
    // Génération du code pour la section données
    cout << "\t.data" << endl;
    cout << "\t.align 8" << endl;
    
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Vérifie si c'est un identifiant
    if(current != ID)
        Error("Un identificater était attendu");
        
    // Génération du code pour la première variable
    cout << lexer->YYText() << ":\t.quad 0" << endl;
    // Ajoute la variable à l'ensemble des variables déclarées
    DeclaredVariables.insert(lexer->YYText());
    
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Traitement des variables supplémentaires séparées par des virgules
    while(current == COMMA){
        // Passe à l'identifiant suivant
        current = (TOKEN) lexer->yylex();
        
        // Vérifie si c'est un identifiant
        if(current != ID)
            Error("Un identificateur était attendu");
            
        // Génération du code pour cette variable
        cout << lexer->YYText() << ":\t.quad 0" << endl;
        // Ajoute la variable à l'ensemble des variables déclarées
        DeclaredVariables.insert(lexer->YYText());
        
        // Passe au token suivant
        current = (TOKEN) lexer->yylex();
    }
    
    // Vérifie si le token est bien un crochet fermant
    if(current != LBRACKET)
        Error("caractère ']' attendu");
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
}

// Analyse un opérateur relationnel et renvoie son type
OPREL RelationalOperator(void){
    OPREL oprel;
    
    // Identifie le type d'opérateur relationnel
    if(strcmp(lexer->YYText(), "==") == 0)
        oprel = EQU;
    else if(strcmp(lexer->YYText(), "!=") == 0)
        oprel = DIFF;
    else if(strcmp(lexer->YYText(), "<") == 0)
        oprel = INF;
    else if(strcmp(lexer->YYText(), ">") == 0)
        oprel = SUP;
    else if(strcmp(lexer->YYText(), "<=") == 0)
        oprel = INFE;
    else if(strcmp(lexer->YYText(), ">=") == 0)
        oprel = SUPE;
    else 
        oprel = WTFR;  // Opérateur inconnu
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    return oprel;
}

// Analyse une expression: SimpleExpression [RelationalOperator SimpleExpression]
TYPE Expression(void){
    OPREL oprel;
    TYPE left_type, right_type;
    
    // Analyse la première expression simple
    left_type = SimpleExpression();
    
    // Si on trouve un opérateur relationnel, on continue l'analyse
    if(current == RELOP){
        oprel = RelationalOperator();      // Sauvegarde l'opérateur
        right_type = SimpleExpression();   // Analyse la deuxième expression simple
        
        // Vérification de type - les deux expressions doivent être du même type
        if (left_type != right_type) {
            Error("Les opérandes des opérateurs de comparaison doivent être du même type");
        }
        
        // Le résultat d'une comparaison est toujours de type BOOLEAN
        left_type = BOOLEAN;
        
        // Génération du code assembleur pour la comparaison
        cout << "\tpop %rax" << endl;      // Récupère le deuxième opérande
        cout << "\tpop %rbx" << endl;      // Récupère le premier opérande
        cout << "\tcmpq %rax, %rbx" << endl;  // Compare les deux opérandes
        
        // Traitement spécifique selon l'opérateur de comparaison
        switch(oprel){
            case EQU:
                // Si égal, saute à l'étiquette Vrai
                cout << "\tje Vrai" << ++TagNumber << "\t# If equal" << endl;
                break;
            case DIFF:
                // Si différent, saute à l'étiquette Vrai
                cout << "\tjne Vrai" << ++TagNumber << "\t# If different" << endl;
                break;
            case SUPE:
                // Si supérieur ou égal, saute à l'étiquette Vrai
                cout << "\tjae Vrai" << ++TagNumber << "\t# If above or equal" << endl;
                break;
            case INFE:
                // Si inférieur ou égal, saute à l'étiquette Vrai
                cout << "\tjbe Vrai" << ++TagNumber << "\t# If below or equal" << endl;
                break;
            case INF:
                // Si inférieur, saute à l'étiquette Vrai
                cout << "\tjb Vrai" << ++TagNumber << "\t# If below" << endl;
                break;
            case SUP:
                // Si supérieur, saute à l'étiquette Vrai
                cout << "\tja Vrai" << ++TagNumber << "\t# If above" << endl;
                break;
            default:
                Error("Opérateur de comparaison inconnu");
        }
        
        // Si la comparaison est fausse, empile 0
        cout << "\tpush $0\t\t# False" << endl;
        // Saute l'instruction qui empile -1 (vrai)
        cout << "\tjmp Suite" << TagNumber << endl;
        // Étiquette pour le cas où la comparaison est vraie
        cout << "Vrai" << TagNumber << ":\tpush $0xFFFFFFFFFFFFFFFF\t\t# True" << endl;
        // Étiquette pour continuer l'exécution
        cout << "Suite" << TagNumber << ":" << endl;
    }
    
    // Return the type - BOOLEAN if we had a relational operator, otherwise the left_type
    return left_type;
}

// Analyse une instruction d'assignation: Identifier ":=" Expression
void AssignementStatement(void){
    string variable;
    TYPE expr_type;
    
    // Vérifie si le premier token est un identifiant
    if(current != ID)
        Error("Identificateur attendu");
        
    // Vérifie si la variable a été déclarée
    if(!IsDeclared(lexer->YYText())){
        cerr << "Erreur : Variable '" << lexer->YYText() << "' non déclarée" << endl;
        exit(-1);
    }
    
    // Sauvegarde le nom de la variable
    variable = lexer->YYText();
    
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Vérifie si c'est l'opérateur d'assignation
    if(current != ASSIGN)
        Error("caractères ':=' attendus");
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Analyse l'expression à droite de l'assignation
    expr_type = Expression();
    
    // Vérification de type - la variable et l'expression doivent être du même type
    // Pour l'instant, toutes les variables sont de type UNSIGNED_INT
    if (expr_type != UNSIGNED_INT) {
        Error("Type incompatible dans l'assignation");
    }
    
    // Génère le code pour stocker le résultat dans la variable
    cout << "\tpop " << variable << endl;
}

// Analyse une instruction conditionnelle: "if" Expression "then" Statement ["else" Statement]
void IfStatement(void){
    unsigned long tag = ++TagNumber; // Étiquette unique pour ce if
    TYPE expr_type;
    
    // Consomme "if"
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'expression conditionnelle
    expr_type = Expression();
    
    // Vérification de type - l'expression conditionnelle doit être de type BOOLEAN
    if (expr_type != BOOLEAN) {
        Error("L'expression dans une instruction 'if' doit être de type booléen");
    }
    
    // Génération du code pour le test
    cout << "\tpop %rax" << endl;             // Récupère la valeur de la condition
    cout << "\ttest %rax, %rax" << endl;      // Test si la condition est fausse (0)
    cout << "\tjz Else" << tag << endl;       // Si fausse, saute au bloc else
    
    // Vérifie la présence du "then"
    if(current != THEN)
        Error("'then' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'instruction du bloc "then"
    Statement();
    
    // Saute la partie else
    cout << "\tjmp EndIf" << tag << endl;
    
    // Début du bloc else
    cout << "Else" << tag << ":" << endl;
    
    // Si un "else" est présent
    if(current == ELSE){
        current = (TOKEN) lexer->yylex();
        // Analyse de l'instruction du bloc "else"
        Statement();
    }
    
    // Fin de l'instruction if
    cout << "EndIf" << tag << ":" << endl;
}

// Analyse une boucle while: "while" Expression "do" Statement
void WhileStatement(void){
    unsigned long tag = ++TagNumber; // Étiquette unique pour cette boucle
    TYPE expr_type;
    
    // Consomme "while"
    current = (TOKEN) lexer->yylex();
    
    // Début de la boucle (pour réévaluer la condition)
    cout << "While" << tag << ":" << endl;
    
    // Analyse de l'expression conditionnelle
    expr_type = Expression();
    
    // Vérification de type - l'expression conditionnelle doit être de type BOOLEAN
    if (expr_type != BOOLEAN) {
        Error("L'expression dans une instruction 'while' doit être de type booléen");
    }
    
    // Génération du code pour le test
    cout << "\tpop %rax" << endl;              // Récupère la valeur de la condition
    cout << "\ttest %rax, %rax" << endl;       // Test si la condition est fausse (0)
    cout << "\tjz EndWhile" << tag << endl;    // Si fausse, sort de la boucle
    
    // Vérifie la présence du "do"
    if(current != DO)
        Error("'do' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'instruction du bloc de la boucle
    Statement();
    
    // Retour au début de la boucle
    cout << "\tjmp While" << tag << endl;
    
    // Fin de la boucle while
    cout << "EndWhile" << tag << ":" << endl;
}

// Analyse une boucle for: "for" Letter ":=" Expression "to" Expression "do" Statement
void ForStatement(void){
    unsigned long tag = ++TagNumber; // Étiquette unique pour cette boucle
    string control_var; // Variable de contrôle de la boucle
    TYPE init_expr_type, limit_expr_type;
    
    // Consomme "for"
    current = (TOKEN) lexer->yylex();
    
    // Vérifie si c'est un identifiant (nom de variable)
    if(current != ID)
        Error("Identificateur attendu après 'for'");
    
    // Sauvegarde le nom de la variable de contrôle
    control_var = lexer->YYText();
    
    // Vérifie si la variable a été déclarée
    if(!IsDeclared(control_var.c_str())){
        cerr << "Erreur : Variable de contrôle '" << control_var << "' non déclarée" << endl;
        exit(-1);
    }
    
    // Passe au token suivant (doit être :=)
    current = (TOKEN) lexer->yylex();
    if(current != ASSIGN)
        Error("':=' attendu dans la boucle for");
    
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Analyse l'expression initiale
    init_expr_type = Expression();
    
    // Vérification de type - l'expression initiale doit être de type UNSIGNED_INT
    if (init_expr_type != UNSIGNED_INT) {
        Error("L'expression d'initialisation dans une boucle 'for' doit être de type entier");
    }
    
    // Vérifie la présence du "to"
    if(current != TO) // <-- Fixed to use TO token
        Error("'to' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Début de la boucle

    cout << "For" << tag << ":" << endl;
    
    // Analyse l'expression finale (limite)
    limit_expr_type = Expression();
    
    // Vérification de type - l'expression limite doit être de type UNSIGNED_INT
    if (limit_expr_type != UNSIGNED_INT) {
        Error("L'expression limite dans une boucle 'for' doit être de type entier");
    }
    
    // Compare la variable de contrôle avec la limite
    cout << "\tpop %rax" << endl;                // Récupère la limite
    cout << "\tcmpq %rax, " << control_var << endl;   // Compare la variable à la limite
    cout << "\tjg EndFor" << tag << endl;        // Si variable > limite, sort de la boucle
    
    // Vérifie la présence du "do"
    if(current != DO)
        Error("'do' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'instruction du bloc de la boucle
    Statement();
    
    // Incrémente la variable de contrôle
    cout << "\tincq " << control_var << endl;
    
    // Retour au début de la boucle
    cout << "\tjmp For" << tag << endl;
    
    // Fin de la boucle for
    cout << "EndFor" << tag << ":" << endl;
}

// Analyse un bloc d'instructions: "begin" Statement {";" Statement} "end"
void BlockStatement(void){
    // Consomme "begin"
    current = (TOKEN) lexer->yylex();
    
    // Analyse la première instruction
    Statement();
    
    // Tant qu'on trouve un point-virgule, on analyse une instruction supplémentaire
    while(current == SEMICOLON){
        // Passe au token suivant
        current = (TOKEN) lexer->yylex();
        // Analyse l'instruction suivante
        Statement();
    }
    
    // Vérifie la présence du "end"
    if(current != END)
        Error("'end' attendu");
    
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
}

// Analyse une instruction
void Statement(void){
    // Identifie le type d'instruction en fonction du token courant
    if (current == ID) {
        // Si c'est un identifiant, c'est une instruction d'assignation
        AssignementStatement();
    }
    else if (current == IF) {
        // Si c'est "if", c'est une instruction conditionnelle
        IfStatement();
    }
    else if (current == WHILE) {
        // Si c'est "while", c'est une boucle while
        WhileStatement();
    }
    else if (current == FOR) {
        // Si c'est "for", c'est une boucle for
        ForStatement();
    }
    else if (current == BEG) {  // BEG au lieu de BEGIN
        // Si c'est "begin", c'est un bloc d'instructions
        BlockStatement();
    }
    else {
        Error("Instruction attendue");
    }
}

// Analyse la partie instruction: Statement {";" Statement} "."
void StatementPart(void){
    // Génération du code pour la section texte (instructions)
    cout << "\t.text\t\t# The following lines contain the program" << endl;
    cout << "\t.globl main\t# The main function must be visible from outside" << endl;
    cout << "main:\t\t\t# The main function body :" << endl;
    cout << "\tmovq %rsp, %rbp\t# Save the position of the stack's top" << endl;
    
    // Analyse la première instruction
    Statement();
    
    // Tant qu'on trouve un point-virgule, on analyse une instruction supplémentaire
    while(current == SEMICOLON){
        // Passe au token suivant
        current = (TOKEN) lexer->yylex();
        // Analyse l'instruction
        Statement();
    }
    
    // Vérifie si le dernier token est un point
    if(current != DOT)
        Error("caractère '.' attendu");
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
}

// Analyse un programme: [DeclarationPart] StatementPart
void Program(void){
    // Si le premier token est un crochet ouvrant, c'est une partie déclaration
    if(current == RBRACKET)
        DeclarationPart();
    
    // Analyse la partie instruction
    StatementPart();    
}




// =========================================================================
// SECTION 7: FONCTION PRINCIPALE
// =========================================================================

int main(void){
    // Génère le commentaire d'en-tête
    cout << "\t\t\t# This code was produced by the CERI Compiler" << endl;
    
    // Lance l'analyse lexicale et syntaxique
    current = (TOKEN) lexer->yylex();  // Lit le premier token
    Program();                          // Analyse le programme
    
    // Génère le code de terminaison
    cout << "\tmovq %rbp, %rsp\t\t# Restore the position of the stack's top" << endl;
    cout << "\tret\t\t\t# Return from main function" << endl;
    
    // Vérifie s'il y a des tokens en trop à la fin
    if(current != FEOF){
        cerr << "Caractères en trop à la fin du programme : [" << current << "]";
        Error("."); // Caractères inattendus à la fin du programme
    }
}







