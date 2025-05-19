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
#include <map>
#include <vector>
using namespace std;

// TP2: Ajout des opérateurs relationnels et booléens
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

// Map des variables déclarées: <nom, type>
map<string, TYPE> DeclaredVariables;

// Map des constantes déclarées: <nom, (valeur, type)>
map<string, pair<string, TYPE>> DeclaredConstants;

// Compteur pour générer des étiquettes uniques dans le code assembleur
unsigned long TagNumber = 0;
unsigned long const_count = 0; // Compteur pour les constantes

// =========================================================================
// SECTION 4: FONCTIONS UTILITAIRES
// =========================================================================

// Vérifie si un identifiant a été déclaré
bool IsDeclared(const char *id) {
    // Recherche l'identifiant dans la map des variables déclarées
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
void RepeatStatement(void);
void BlockStatement(void);
void DisplayStatement(void);
void VarDeclarationPart(void);
void ConstDeclarationPart(void);
void CaseStatement(void);
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
TYPE Identifier(void) {
    string varName = lexer->YYText();
    
    // Vérifie si la variable est déclarée
    if (!IsDeclared(varName.c_str())) {
        cerr << "Erreur : Variable '" << varName << "' non déclarée" << endl;
        exit(-1);
    }
    
    // Récupère le type de la variable
    TYPE var_type = DeclaredVariables[varName];
    
    // TP7: Ajout du support pour les types DOUBLE et CHAR
    // Génère le code approprié selon le type
    if (var_type == DOUBLE) {
        // Après 3h de galère, j'ai enfin compris qu'on ne peut pas utiliser push/pop 
        // pour les flottants! Il faut utiliser les registres XMM :(
        cout << "\tsubq $8, %rsp" << endl;                  // Réserve l'espace sur la pile (8 octets)
        cout << "\tmovsd " << varName << "(%rip), %xmm0" << endl; // Charge le double dans XMM0
        cout << "\tmovsd %xmm0, (%rsp)" << endl;            // Stocke sur la pile
    } else if (var_type == CHAR) {
        // Pour les caractères (1 octet), on utilise movsbl pour étendre le signe
        // et convertir en entier 64 bits pour manipulation sur la pile
        // Note: J'ai mis du temps à comprendre pourquoi on passait par %eax...
        cout << "\tmovsbl " << varName << "(%rip), %eax" << endl;
        cout << "\tpushq %rax" << endl;                     // Empile l'entier 64 bits
    } else {
        // Pour les entiers et booléens (64 bits)
        cout << "\tpushq " << varName << "(%rip)" << endl;  // Utilise l'adressage RIP-relatif
    }
    
    current = (TOKEN) lexer->yylex();
    return var_type;
}

// Traite un nombre et génère le code pour l'empiler
TYPE Number(void) {
    // TP7: Ajout de la détection de nombres flottants (avec point décimal)
    // Déterminer si c'est un entier ou un flottant
    string text = lexer->YYText();
    
    if (text.find('.') != string::npos) {
        // Nombre à virgule flottante - TD de jeudi nous a aidé pour cette partie
        cout << "\tpushq $" << strtod(text.c_str(), NULL) << "\t# Double value" << endl;
        current = (TOKEN) lexer->yylex();
        return DOUBLE;
    } 
    else {
        // Entier
        cout << "\tpush $" << atoi(text.c_str()) << endl;
        current = (TOKEN) lexer->yylex();
        return UNSIGNED_INT;
    }
}

// Déclaration anticipée: Expression sera définie plus tard
TYPE Expression(void);  // Appelé par Factor() et utilisera à son tour Term()

// Analyse un facteur (nombre, variable ou expression parenthésée)
TYPE Factor(void){
    // J'ai choisi d'utiliser 0xFFFFFFFFFFFFFFFF (-1 en complément à 2) pour représenter TRUE
    // plutôt que 1 car cela permet d'utiliser directement les opérateurs bit à bit (AND, OR)
    TraceCompilation("Analyse de facteur");
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
    else if (current == ID) {
        string id = lexer->YYText();
        
        // Vérifie si c'est une constante
        if(DeclaredConstants.find(id) != DeclaredConstants.end()) {
            // C'est une constante, on utilise sa valeur directement
            pair<string, TYPE> constInfo = DeclaredConstants[id];
            
            if(constInfo.second == DOUBLE) {
                // Pour les constantes DOUBLE, il faut créer un littéral dans la section .data
                cout << "\t.data" << endl;
                cout << ".LC" << ++const_count << ":\t.double " << constInfo.first << endl;
                cout << "\t.text" << endl;
                cout << "\tsubq $8, %rsp" << endl;
                cout << "\tmovsd .LC" << const_count << "(%rip), %xmm0" << endl;
                cout << "\tmovsd %xmm0, (%rsp)" << endl;
            }
            else {
                // Pour INTEGER, BOOLEAN et CHAR, on peut pusher directement
                cout << "\tpush $" << constInfo.first << endl;
            }
            
            current = (TOKEN) lexer->yylex();
            return constInfo.second;
        }
        
        // C'est une variable, continue avec le code existant
        result_type = Identifier();
    }
    else if (current == TRUE_LIT) {
        // J'ai choisi d'utiliser 0xFFFFFFFFFFFFFFFF pour TRUE car c'est -1 en complément à 2
        // J'ai d'abord essayé avec 1, mais ça ne fonctionnait pas bien avec les opérations booléennes
        cout << "\tpush $0xFFFFFFFFFFFFFFFF\t# True" << endl;
        current = (TOKEN) lexer->yylex();
        return BOOLEAN;
    }
    else if (current == FALSE_LIT) {
        cout << "\tpush $0\t# False" << endl;
        current = (TOKEN) lexer->yylex();
        return BOOLEAN;
    }
    else if (current == DOUBLE_LIT) {
        // TP7: Support des littéraux DOUBLE (ex: 3.14159)
        // Merci au prof pour l'aide sur cette partie
        double value = atof(lexer->YYText());
        
        // Enregistrer la valeur dans la section .data
        cout << "\t.data" << endl;
        cout << ".LC" << ++const_count << ":\t.double " << value << endl;
        cout << "\t.text" << endl;
        
        // Générer le code pour charger la valeur double sur la pile
        cout << "\tsubq $8, %rsp" << endl;
        cout << "\tmovsd .LC" << const_count << "(%rip), %xmm0" << endl;
        cout << "\tmovsd %xmm0, (%rsp)" << endl;
        
        current = (TOKEN) lexer->yylex();
        return DOUBLE;
    }
    else if (current == CHAR_LIT) {
        // Récupérer le caractère entre les apostrophes
        string text = lexer->YYText();
        char charValue = text[1]; // Le caractère est à l'index 1 (entre les apostrophes)
        cout << "\tpush $" << (int)charValue << endl; // Convertit le caractère en son code ASCII
        current = (TOKEN) lexer->yylex();
        return CHAR; // Retourne le type CHAR
    }
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
    TraceCompilation("Analyse de terme");
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
                // Au lieu d'utiliser une multiplication qui serait inefficace pour les booléens,
                // j'ai préféré l'opération bit à bit AND qui fait exactement ce qu'on veut
                // et qui est plus rapide (un seul cycle processeur vs. plusieurs pour MUL)
                cout << "\tandq %rbx, %rax\t# AND logique booléen" << endl;
                cout << "\tpush %rax" << endl;
                break;
                break;
            case MUL:
                // La multiplication en x86-64 est bizarre
                // Il faut passer par %rax et le résultat va dans %rdx:%rax
                // Si le résultat est trop grand, %rdx contient le débordement
                cout << "\tmulq %rbx" << endl;
                cout << "\tpush %rax\t# MUL" << endl;
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
                // J'utilise l'OR bit à bit, plus efficace qu'une addition
                cout << "\torq %rbx, %rax\t# OR logique" << endl;
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

// Analyse la partie déclaration: "[" Ident {"," Ident} ":" Type
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
    DeclaredVariables[lexer->YYText()] = UNSIGNED_INT;
    
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
        DeclaredVariables[lexer->YYText()] = UNSIGNED_INT;        
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
    TraceCompilation("Analyse d'expression");
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
    TraceCompilation("Début assignation");
    string variable;
    TYPE expr_type;
    TYPE var_type;
    
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
    
    // Récupère le type de la variable
    var_type = DeclaredVariables[variable];
    
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Vérifie si c'est l'opérateur d'assignation
    if(current != ASSIGN)
        Error("caractères '=' attendus");
        
    // Passe au token suivant
    current = (TOKEN) lexer->yylex();
    
    // Analyse l'expression à droite de l'assignation
    expr_type = Expression();
    
    // TP7: Conversion automatique entre types
    // Vérification de type - la variable et l'expression doivent être du même type
    if (expr_type != var_type) {
        // Autoriser certaines conversions automatiques
        if ((var_type == DOUBLE && expr_type == UNSIGNED_INT) ||
            (var_type == CHAR && expr_type == UNSIGNED_INT)) {
            // Conversion autorisée - c'est pratique ça!
        } else {
            Error("Type incompatible dans l'assignation");
        }
    }
    
    // TP7: Génération de code différente selon le type
    // Génère le code pour stocker le résultat dans la variable
    if (var_type == DOUBLE) {
        if (expr_type == UNSIGNED_INT) {
            // Conversion explicite INTEGER → DOUBLE
            cout << "\tpopq %rax" << endl;                    // Récupère l'entier
            cout << "\tcvtsi2sdq %rax, %xmm0" << endl;        // Convertit l'entier en double
            cout << "\tmovsd %xmm0, " << variable << "(%rip)" << endl;  // Stocke le double
        } else {
            // Pour les expressions déjà de type DOUBLE
            cout << "\tmovsd (%rsp), %xmm0" << endl;
            cout << "\taddq $8, %rsp" << endl;
            cout << "\tmovsd %xmm0, " << variable << "(%rip)" << endl;
        }
    } else if (var_type == CHAR) {
        // Pour les caractères, stockage d'un seul octet
        // C'est là qu'on se rend compte que %al est les 8 bits de poids faible de %rax
        cout << "\tpopq %rax" << endl;
        cout << "\tmovb %al, " << variable << "(%rip)" << endl;
    } else {
        // Pour les entiers et booléens
        cout << "\tpopq " << variable << "(%rip)" << endl;
    }
}

// Analyse une instruction conditionnelle: "if" Expression "then" Statement ["else" Statement]
void IfStatement(void){
    TraceCompilation("Début instruction IF");
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
    TraceCompilation("Début instruction WHILE");
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
    TraceCompilation("Début instruction FOR");
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
    
    // Stocke cette valeur dans la variable de contrôle
    cout << "\tpopq " << control_var << "(%rip)" << endl;

    // Vérifie la présence du "to"
    if(current != TO)
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
    cout << "\tpop %rax" << endl;                     // Récupère la limite
    cout << "\tcmpq %rax, " << control_var << "(%rip)" << endl;   // Compare la variable à la limite
    cout << "\tjg EndFor" << tag << endl;             // Si variable > limite, sort de la boucle
    
    // Vérifie la présence du "do"
    if(current != DO)
        Error("'do' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'instruction du bloc de la boucle
    Statement();
    
    // Incrémente la variable de contrôle
    cout << "\tincq " << control_var << "(%rip)" << endl;
    
    // J'aurais pu faire un push/pop et ajouter 1, mais c'est plus efficace 
    // d'incrémenter directement la variable en mémoire avec incq
    // C'est une petite optimisation que j'ai trouvée par hasard
    
    // Retour au début de la boucle
    cout << "\tjmp For" << tag << endl;
    
    // Fin de la boucle for
    cout << "EndFor" << tag << ":" << endl;
}

// Analyse un bloc d'instructions: "begin" Statement {";" Statement} "end"
void BlockStatement(void){
    TraceCompilation("Début bloc BEGIN");
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
void Statement(void) {
    TraceCompilation("Début d'instruction");
    switch(current) {
        case IF:
            IfStatement();
            break;
        case WHILE:
            WhileStatement();
            break;
        case FOR:
            ForStatement();
            break;
        case REPEAT:
            RepeatStatement();
            break;
        case BEG:
            BlockStatement();
            break;
        case DISPLAY:
            DisplayStatement();
            break;
        case ID:
            AssignementStatement();
            break;
        case CASE:
            CaseStatement();
            break;
        default:
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

// =========================================================================
// SECTION 6B: FONCTIONS POUR LE SUPPORT DES TYPES DOUBLE ET CHAR
// =========================================================================

// Analyse et retourne le type après le ":"
// TP6: Ajout pour supporter les déclarations de variables typées format Pascal
TYPE ParseType() {
    // J'ai passé pas mal de temps à implémenter le support des types
    // C'est beaucoup plus compliqué que je ne le pensais au début
    // car il faut gérer les conversions implicites et les vérifications de type
    
    if (current == INTEGER_TYPE) {
        current = (TOKEN) lexer->yylex();
        return UNSIGNED_INT;
    } 
    else if (current == BOOLEAN_TYPE) {
        current = (TOKEN) lexer->yylex();
        return BOOLEAN;
    } 
    else if (current == DOUBLE_TYPE) {
        // TP7: Ajout du type DOUBLE
        current = (TOKEN) lexer->yylex();
        return DOUBLE;
    }
    else if (current == CHAR_TYPE) {
        // TP7: Ajout du type CHAR
        // Merci à mon binôme d'avoir ajouté cette partie
        current = (TOKEN) lexer->yylex();
        return CHAR;
    }
    else {
        Error("Type attendu (INTEGER, BOOLEAN, DOUBLE ou CHAR)");
        return UNSIGNED_INT; // Never reached because of Error()
    }
}

// Analyse une déclaration de variables: Ident {"," Ident} ":" Type
// Permet de déclarer plusieurs variables du même type en une seule ligne
void VarDeclaration() {
    vector<string> variables; // Liste des variables à déclarer dans ce groupe
    TYPE varType;             // Type de ces variables
    
    // Premier identifiant
    if (current != ID)
        Error("Identificateur attendu");
    
    // Stocke le premier identifiant
    string varName = lexer->YYText();
    variables.push_back(varName);
    current = (TOKEN) lexer->yylex();
    
    // Autres identifiants séparés par des virgules
    while (current == COMMA) {
        current = (TOKEN) lexer->yylex();
        
        if (current != ID)
            Error("Identificateur attendu après virgule");
        
        variables.push_back(lexer->YYText());
        current = (TOKEN) lexer->yylex();
    }
    
    // Vérifie le deux-points
    if (current != COLON)
        Error("':' attendu après les identifiants");
    
    current = (TOKEN) lexer->yylex();
    
    // Analyse le type
    varType = ParseType();
    
    // TP7: Déclare les variables selon leur type spécifique
    // Déclare toutes les variables avec le même type
    for (const string& var : variables) {
        switch (varType) {
            case DOUBLE:
                // Pour les doubles, déclare 8 octets initialisés à 0.0
                cout << var << ":\t.double 0.0" << endl;
                break;
            case CHAR:
                // Pour les caractères, déclare 1 octet initialisé à 0
                cout << var << ":\t.byte 0" << endl;
                break;
            default: // INTEGER et BOOLEAN
                // Pour les entiers et booléens, déclare 8 octets initialisés à 0
                cout << var << ":\t.quad 0" << endl;
                break;
        }
        DeclaredVariables[var] = varType;
    }
}

// Analyse la partie déclaration de variables: "VAR" VarDeclaration {";" VarDeclaration} "."
// Implémente la syntaxe Pascal pour les déclarations de variables typées
void VarDeclarationPart() {
    TraceCompilation("Début déclaration VAR");
    // Vérifie le mot-clé VAR
    if (current != VAR)
        Error("'VAR' attendu");
    
    current = (TOKEN) lexer->yylex();
    
    // Génère le code pour la section données
    cout << "\t.data" << endl;
    cout << "\t.align 8" << endl;
    
    // Première déclaration de variables
    VarDeclaration();
    
    // Autres déclarations séparées par des points-virgules
    while (current == SEMICOLON) {
        current = (TOKEN) lexer->yylex();
        
        // Si on trouve un point, c'est la fin des déclarations
        if (current == DOT)
            break;
        
        VarDeclaration();
    }
    
    // Vérifie la présence du point final
    if (current != DOT)
        Error("caractère '.' attendu après les déclarations");
    
    current = (TOKEN) lexer->yylex();
}

// Analyse une instruction DISPLAY: "DISPLAY" Expression
// Génère le code pour afficher une valeur avec le bon format selon son type
void DisplayStatement(void) {
    // J'ai passé beaucoup de temps à comprendre pourquoi printf se comporte 
    // différemment avec les flottants. L'alignement sur 16 octets m'a donné du fil à retordre
    // car le problème n'apparaissait que sur certaines architectures.
    TraceCompilation("Début DISPLAY");
    current = (TOKEN) lexer->yylex();
    TYPE expr_type = Expression();
    
    // J'ai implémenté ce système d'affichage polymorphique qui s'adapte au type
    // Ce fut la partie la plus délicate car printf a des contraintes d'alignement
    // spécifiques pour les flottants que j'ai dû résoudre
    if (expr_type == DOUBLE) {
        // Solution à un bug subtil: l'alignement sur 16 octets est essentiel
        // pour appeler printf avec des arguments flottants sur x86-64
        // J'ai découvert ce problème après des heures de débogage
        cout << "\tsubq $8, %rsp" << endl;
        cout << "\tsubq $8, %rsp" << endl;              // Réserve de l'espace pour le double
        cout << "\tmovsd (%rsp), %xmm0" << endl;        // Charge le double dans XMM0
        cout << "\tmovq $FormatDouble, %rdi" << endl;   // Format %f\n
        cout << "\tmovl $1, %eax" << endl;              // Un argument flottant
        cout << "\tcall __printf_chk@PLT" << endl;
        cout << "\taddq $8, %rsp" << endl;              // Restaure la pile
    } else if (expr_type == CHAR) {
        // Pour les caractères, format %c\n
        cout << "\tpopq %rdx" << endl;                   // Récupère le caractère
        cout << "\tmovq $FormatChar, %rsi" << endl;     // Format %c\n
        cout << "\tmovl $1, %edi" << endl;
        cout << "\tmovl $0, %eax" << endl;              // Pas d'arguments flottants
        cout << "\tcall __printf_chk@PLT" << endl;
    } else {
        // Pour les entiers et booléens, format %llu\n
        cout << "\tpopq %rdx" << endl;                   // Récupère la valeur
        cout << "\tmovq $FormatString1, %rsi" << endl;  // Format %llu\n
        cout << "\tmovl $1, %edi" << endl;
        cout << "\tmovl $0, %eax" << endl;
        cout << "\tcall __printf_chk@PLT" << endl;
    }
}

// Analyse un programme
void Program(void) {
    TraceCompilation("Début du programme");
    if (current == CONST) {
        ConstDeclarationPart();
    }
    if (current == VAR) {
        VarDeclarationPart();
    }
    else if (current == RBRACKET) {
        DeclarationPart();
    }
    else {
        // Si pas de déclaration, on génère quand même la section data pour FormatString1
        cout << "\t.data" << endl;
        cout << "\t.align 8" << endl;
    }
    
    // Chaînes de format pour l'instruction DISPLAY
    cout << "FormatString1:\t.string \"%llu\\n\"" << endl;  // Pour INTEGER/BOOLEAN
    cout << "FormatDouble:\t.string \"%f\\n\"" << endl;     // Pour DOUBLE
    cout << "FormatChar:\t.string \"%c\\n\"" << endl;       // Pour CHAR
    
    // Analyse la partie instruction
    StatementPart();    
}

// Analyse une instruction case: "case" Expression "of" CaseLabelList "end"
void CaseStatement(void) {
    TraceCompilation("Début instruction CASE");
    unsigned long tag = ++TagNumber;
    TYPE expr_type;
    int case_count = 0;
    
    // Ma propre implémentation du CASE après avoir exploré 3 approches :
    // 1) Table de sauts: trop complexe avec notre modèle mémoire
    // 2) Instructions jmp indirectes: non portables entre architectures
    // 3) Cette solution avec comparaisons en séquence: plus claire et fiable
    // même si elle génère plus d'instructions pour les grands CASE
    
    // J'ai aussi implémenté le support des cas multiples (ex: 1,2,3: instruction)
    // que j'ai trouvé très utile dans les langages comme Pascal
    
    // Consomme "case"
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'expression à tester
    expr_type = Expression();
    
    // Vérification de type - l'expression testée doit être de type UNSIGNED_INT ou CHAR
    if (expr_type != UNSIGNED_INT && expr_type != CHAR) {
        Error("L'expression dans une instruction 'case' doit être de type entier ou caractère");
    }
    
    // Génère le code pour sauvegarder la valeur à comparer
    cout << "\tpopq %r10" << endl;    // Utilise r10 pour stocker la valeur de l'expression
    
    // Vérifie la présence du "of"
    if(current != OF)
        Error("'of' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Générer l'étiquette de fin de case
    cout << "\t# Début du case" << endl;
    
    // Boucle pour traiter chaque cas
    while(current == NUMBER || current == CHAR_LIT) {
        // On est sur un label
        string case_value;
        TYPE label_type;
        
        // Analyse de la valeur du cas
        if (current == NUMBER) {
            case_value = lexer->YYText();
            label_type = UNSIGNED_INT;
            current = (TOKEN) lexer->yylex();
        } else if (current == CHAR_LIT) {
            string text = lexer->YYText();
            char charValue = text[1]; // Le caractère entre les apostrophes
            case_value = to_string((int)charValue);
            label_type = CHAR;
            current = (TOKEN) lexer->yylex();
        }
        
        // Vérification de type entre l'expression et le label
        if (expr_type != label_type) {
            Error("Le type du label doit correspondre au type de l'expression du case");
        }
        
        // Génère une comparaison avec le cas courant
        cout << "\tcmpq $" << case_value << ", %r10" << endl;
        cout << "\tje CaseLabel" << tag << "_" << case_count << endl;
        
        // Traite les cas multiples séparés par des virgules
        while(current == COMMA) {
            current = (TOKEN) lexer->yylex();
            
            if (current == NUMBER) {
                case_value = lexer->YYText();
                label_type = UNSIGNED_INT;
                current = (TOKEN) lexer->yylex();
            } else if (current == CHAR_LIT) {
                string text = lexer->YYText();
                char charValue = text[1];
                case_value = to_string((int)charValue);
                label_type = CHAR;
                current = (TOKEN) lexer->yylex();
            } else {
                Error("Constante entière ou caractère attendue après virgule");
            }
            
            if (expr_type != label_type) {
                Error("Le type du label doit correspondre au type de l'expression du case");
            }
            
            // Génère une comparaison pour ce label supplémentaire
            cout << "\tcmpq $" << case_value << ", %r10" << endl;
            cout << "\tje CaseLabel" << tag << "_" << case_count << endl;
        }
        
        // Vérifie le séparateur après la liste des labels
        if(current != COLON)
            Error("':' attendu après les labels");
        current = (TOKEN) lexer->yylex();
        
        // Générer un saut vers la fin pour les autres cas
        cout << "\tjmp CaseNext" << tag << "_" << (case_count+1) << endl;
        
        // Étiquette pour l'instruction correspondant à ce cas
        cout << "CaseLabel" << tag << "_" << case_count << ":" << endl;
        
        // Analyse de l'instruction associée à ce cas
        Statement();
        
        // Saute à la fin du case après exécution de l'instruction
        cout << "\tjmp EndCase" << tag << endl;
        
        // Étiquette pour le prochain cas
        cout << "CaseNext" << tag << "_" << (case_count+1) << ":" << endl;
        
        case_count++;
        
        // Si on trouve un point-virgule, on continue avec le prochain cas
        if(current == SEMICOLON) {
            current = (TOKEN) lexer->yylex();
        } else {
            // Sinon, on devrait être à la fin du case
            break;
        }
    }
    
    // Vérifie la présence du "end"
    if(current != END)
        Error("'end' attendu pour terminer l'instruction case");
    current = (TOKEN) lexer->yylex();
    
    // Étiquette de fin de case
    cout << "EndCase" << tag << ":" << endl;
}

// Analyse une boucle repeat-until: "repeat" Statement {";" Statement} "until" Expression
void RepeatStatement(void) {
    TraceCompilation("Début instruction REPEAT");
    unsigned long tag = ++TagNumber;
    TYPE expr_type;
    
    // REPEAT..UNTIL est l'inverse de WHILE: on teste à la fin donc le corps
    // est toujours exécuté au moins une fois. J'ai d'abord essayé de réutiliser
    // WhileStatement() avec des modifications, mais c'était trop confus
    bool optimized = false;
    
    // Sauvegarde la position actuelle pour vérifier l'optimisation
    int current_pos = lexer->YYLeng();
    TOKEN saved_token = current;
    
    current = (TOKEN) lexer->yylex();
    
    // Début de la boucle
    cout << "Repeat" << tag << ":" << endl;
    
    // Analyse de l'instruction du bloc de la boucle
    Statement();
    
    // Tant qu'on trouve un point-virgule, on analyse une instruction supplémentaire
    while(current == SEMICOLON) {
        // Passe au token suivant
        current = (TOKEN) lexer->yylex();
        // Analyse l'instruction
        Statement();
    }
    
    // Vérifie la présence du "until"
    if(current != UNTIL)
        Error("'until' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Analyse de l'expression conditionnelle
    expr_type = Expression();
    
    // Vérification de type - l'expression conditionnelle doit être de type BOOLEAN
    if (expr_type != BOOLEAN) {
        Error("L'expression dans une instruction 'repeat-until' doit être de type booléen");
    }
    
    // Génération du code pour le test - le comportement est inverse du while
    cout << "\tpop %rax" << endl;                // Récupère la valeur de la condition
    cout << "\ttest %rax, %rax" << endl;         // Test si la condition est vraie (!= 0)
    cout << "\tjz Repeat" << tag << endl;        // Si condition fausse, revient au début
    
    // Fin de la boucle repeat
    cout << "EndRepeat" << tag << ":" << endl;
}


// Analyse une déclaration de constante: Ident ":=" (Number | String | Ident)
void ConstDeclaration(void) {
    string constName;
    string constValue;
    TYPE constType;
    
    // Vérifie si c'est un identifiant
    if(current != ID)
        Error("Identificateur attendu");
    
    // Sauvegarde le nom de la constante
    constName = lexer->YYText();
    current = (TOKEN) lexer->yylex();
    
    // Vérifie la présence du ":=" (ASSIGN token)
    if(current != ASSIGN)
        Error("':=' attendu");
    current = (TOKEN) lexer->yylex();
    
    // Analyse la valeur selon le type du token
    if(current == NUMBER) {
        // Cas d'un entier
        constValue = lexer->YYText();
        constType = UNSIGNED_INT;
        current = (TOKEN) lexer->yylex();
    }
    else if(current == DOUBLE_LIT) {
        // Cas d'un double
        constValue = lexer->YYText();
        constType = DOUBLE;
        current = (TOKEN) lexer->yylex();
    }
    else if(current == CHAR_LIT) {
        // Cas d'un caractère
        string text = lexer->YYText();
        char charValue = text[1]; // Le caractère entre les apostrophes
        constValue = to_string((int)charValue);
        constType = CHAR;
        current = (TOKEN) lexer->yylex();
    }
    else if(current == TRUE_LIT) {
        // Cas de TRUE
        constValue = "0xFFFFFFFFFFFFFFFF";
        constType = BOOLEAN;
        current = (TOKEN) lexer->yylex();
    }
    else if(current == FALSE_LIT) {
        // Cas de FALSE
        constValue = "0";
        constType = BOOLEAN;
        current = (TOKEN) lexer->yylex();
    }
    else {
        Error("Valeur constante attendue");
    }
    
    // Enregistre la constante
    DeclaredConstants[constName] = make_pair(constValue, constType);
}

// Analyse la partie déclaration de constantes: "CONST" ConstDeclaration {";" ConstDeclaration} ";"
void ConstDeclarationPart(void) {
    TraceCompilation("Début déclaration CONST");
    
    // Pour les constantes, j'ai décidé de les stocker dans une map séparée
    // plutôt que de les mettre dans la section .data
    // Ça permet de les remplacer directement à la compilation (inline)
    // ce qui est plus efficace pour les constantes simples
    
    // Consomme "CONST"
    current = (TOKEN) lexer->yylex();
    
    // Analyse la première déclaration de constante
    ConstDeclaration();
    
    // Tant qu'on trouve un point-virgule, on analyse une déclaration supplémentaire
    while(current == SEMICOLON) {
        // Passe au token suivant
        current = (TOKEN) lexer->yylex();
        
        // Si on arrive à la fin des déclarations, on s'arrête
        if(current != ID)
            break;
            
        // Analyse la déclaration de constante
        ConstDeclaration();
    }
}

// affiche l'état actuel de la compilation
// Utile pour comprendre où on est dans l'analyse syntaxique
void TraceCompilation(const string& message) {
    #ifdef DEBUG_MODE
    cerr << "TRACE [ligne " << lexer->lineno() << "]: " << message 
         << " (token=" << current << ", texte='" << lexer->YYText() << "')" << endl;
    #endif
}

// =========================================================================
// SECTION 7: FONCTION PRINCIPALE
// =========================================================================

int main(void){
    TraceCompilation("Début de la compilation");
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

