# Compilateur Pascal vers Assembleur x86-64

Ce compilateur traduit un sous-ensemble du langage Pascal en assembleur x86-64 exécutable. Il a été développé dans le cadre d'un projet universitaire.

## Fonctionnalités implémentées
## Mon travail personnel

J'ai personnellement implémenté les composants et fonctionnalités suivants :

- **Structure de contrôle FOR complète** (lignes 802-879)
  - Initialisation, test de limite, incrémentation automatique
  - Vérification des types pour la variable de contrôle et la limite
  - Optimisation de l'incrémentation directement en mémoire

- **Structure REPEAT/UNTIL** (lignes 1267-1337)
  - Exécution garantie au moins une fois (contrairement à WHILE)
  - Test de condition à la fin de la boucle
  - Gestion efficace des étiquettes de saut

- **Structure CASE avec cas multiples** (lignes 1162-1266)
  - Support de plusieurs valeurs pour un même cas (ex: 'A','B': instruction)
  - Compatible avec les types INTEGER et CHAR
  - Structure de contrôle avec sauts conditionnels optimisés

- **Support des types avancés**
  - DOUBLE avec registres XMM (lignes 970-975 pour le type, 1095-1114 pour l'affichage)
  - CHAR avec conversion (lignes 976-980 pour le type, 1115-1124 pour l'affichage)
  - Conversions implicites entre types (INTEGER→DOUBLE, INTEGER→CHAR)

- **Système de constantes (CONST)** (lignes 1372-1435)
  - Support pour tous les types (INTEGER, BOOLEAN, DOUBLE, CHAR)
  - Remplacement direct à la compilation (inline) pour plus d'efficacité
  - Syntaxe étendue compatible avec Pascal

- **Système de débogage intégré** (lignes 1436-1442)
  - Fonction TraceCompilation pour suivre l'analyse syntaxique
  - Affichage conditionnel avec #ifdef DEBUG_MODE
  - Informations détaillées sur les tokens et le contexte

- **Instruction DISPLAY polymorphique** (lignes 1095-1132)
  - Affichage adaptatif selon le type de l'expression
  - Gestion spéciale pour l'alignement des flottants
  - Formats distincts pour chaque type (INTEGER, DOUBLE, CHAR, BOOLEAN)

### Défis techniques surmontés

- **Alignement mémoire pour les flottants**: J'ai découvert que les appels à printf avec des arguments flottants nécessitent un alignement spécifique sur 16 octets qui n'était pas évident au départ.
  
- **Implémentation de CASE optimisée**: J'ai exploré plusieurs approches (table de sauts, jmp indirects) avant de trouver une solution avec des comparaisons séquentielles qui est à la fois fiable et lisible.

- **Gestion des types et conversions**: La détection et l'implémentation des conversions implicites entre types différents (particulièrement INTEGER→DOUBLE) ont demandé une compréhension approfondie des registres x86-64.

### Extensions principales

- **Support complet des nombres flottants (DOUBLE)**
  - Implémentation des registres XMM pour la manipulation
  - Conversion automatique INTEGER → DOUBLE
  - Alignement spécifique de la pile pour les appels à printf

- **Support des caractères (CHAR)**
  - Stockage optimisé sur un octet
  - Conversion implicite depuis les codes ASCII

- **Structure CASE/OF avec cas multiples**
  - Syntaxe étendue permettant `cas1, cas2: instruction`
  - Compatible avec les types INTEGER et CHAR

- **Structure REPEAT/UNTIL**
  - Exécution garantie au moins une fois du corps de boucle
  - Test de condition en fin de boucle (contrairement à WHILE)

- **Système de constantes (CONST)**
  - Support de tous les types (INTEGER, BOOLEAN, DOUBLE, CHAR)
  - Optimisations à la compilation pour les constantes en ligne

- **Système de débogage intégré**
  - Fonction TraceCompilation pour suivre l'analyse syntaxique
  - Affichage adapté selon les types pour l'instruction DISPLAY

### Fonctionnalités de base

- Expressions avec opérateurs arithmétiques, logiques et relationnels
- Structures de contrôle IF/THEN/ELSE, WHILE/DO, FOR/TO/DO
- Déclarations de variables typées avec la syntaxe Pascal (VAR)
- Blocs d'instructions (BEGIN/END)

## Difficultés surmontées

- Gestion des registres XMM pour les nombres flottants
- Alignement correct de la pile pour les appels système
- Mise en œuvre des sauts conditionnels pour CASE
- Conversion entre types avec préservation de la sémantique

## Exemples de programmes compilables

Tous les exemples ci-dessous sont compilables avec notre compilateur.

### Exemple 1: Types et conversions

```pascal
(* test_types.p *)
VAR
  i: INTEGER;
  d: DOUBLE;
  c: CHAR.
i := 65;
d := i;    (* Conversion INTEGER → DOUBLE *)
c := i;    (* Conversion INTEGER → CHAR - affiche 'A' *)
DISPLAY i; (* Affiche 65 *)
DISPLAY d; (* Affiche 65.0 *)
DISPLAY c. (* Affiche A *)
```

**Résultat :**
```
65
65.000000
A
```

### Exemple 2: Structures de contrôle avancées

```pascal
(* test_structures.p *)
CONST
  MAX := 5;
  PI := 3.14159
VAR
  x, somme: INTEGER;
  c: CHAR.
(* Test REPEAT..UNTIL *)
x := 0;
somme := 0;
REPEAT
  somme := somme + x;
  x := x + 1;
  DISPLAY somme
UNTIL x > MAX;
(* Test CASE avec valeurs multiples *)
c := 'C';
CASE c OF
  'A': DISPLAY 1;
  'B', 'C', 'D': DISPLAY 2;
  'E': DISPLAY 3
END.
```

**Résultat :**
```
0
1
3
6
10
15
2
```

### Exemple 3: Boucles et conditions

```pascal
(* test_loops.p *)
VAR
  i, j: INTEGER;
  result: INTEGER.
result := 0;
(* Boucle FOR *)
FOR i := 1 TO 5 DO
BEGIN
  (* Boucle WHILE imbriquée *)
  j := i;
  WHILE j > 0 DO
  BEGIN
    result := result + 1;
    j := j - 1
  END;
  DISPLAY result
END;
(* IF avec opérations logiques *)
IF (result > 10) && (result < 20) THEN
  DISPLAY 'Y'
ELSE
  DISPLAY 'N'.
```

**Résultat :**
```
1
3
6
10
15
Y
```

## Instructions d'utilisation

Pour compiler et exécuter un programme Pascal avec le compilateur :

```bash
# Compiler un fichier Pascal en assembleur
./compilateur < programme.p > programme.s

# Générer l'exécutable
gcc -no-pie -o programme programme.s

# Exécuter le programme
./programme
```

## Notes importantes sur la syntaxe

1. Les déclarations de constantes utilisent `:=` et non `=`
2. Ne pas mettre de point après chaque constante
3. Mettre un point à la fin des déclarations de variables et à la fin du programme
4. Pour CASE, utiliser une virgule pour les cas multiples : `'A', 'B': DISPLAY 1;`




## Récit du développement

J'ai appris beaucoup de choses en développant ce compilateur. Voici les principales difficultés et réussites que j'ai rencontrées:

### Les défis rencontrés

- **L'alignement pour les flottants** m'a posé beaucoup de problèmes. J'ai passé presque deux jours à comprendre pourquoi mon programme crashait aléatoirement lors des appels à printf avec des DOUBLE. C'était lié à l'alignement sur 16 octets requis par la convention d'appel System V AMD64.

- **Les registres XMM** étaient nouveaux pour moi. J'ai dû chercher des informations sur les instructions SSE et apprendre à manipuler ces registres. Les instructions comme MOVSD et ADDSD n'étaient pas intuitives au début.

- Pour **l'implémentation de CASE**, j'ai d'abord essayé de faire une table de sauts (jump table), mais j'ai rencontré des problèmes avec les adresses relatives. J'ai finalement opté pour une solution plus simple avec des comparaisons séquentielles qui fonctionnait mieux.

### Les moments importants

- Quand j'ai finalement réussi à implémenter les **conversions implicites entre INTEGER et DOUBLE**, j'étais vraiment content. Faire fonctionner correctement la transformation d'un entier en flottant dans les registres XMM a été une étape importante.

- Pour **REPEAT..UNTIL**, je pensais au début que ce serait simplement un WHILE inversé, mais la logique des sauts est différente et j'ai dû tout repenser.

- J'ai été content de trouver comment **remplacer les constantes directement dans le code** au lieu de les stocker en mémoire. C'est une optimisation simple qui améliore les performances.

### Les bugs difficiles

- Un problème persistant concernait la **gestion de la pile** lors des appels imbriqués. Dans certains cas, la pile se désalignait et causait des plantages difficiles à comprendre.

- Les **valeurs booléennes** m'ont donné du fil à retordre dans les expressions conditionnelles. J'ai fini par utiliser 0xFFFFFFFFFFFFFFFF pour TRUE (au lieu de simplement 1) pour que les opérations bit à bit fonctionnent correctement.

- La **syntaxe de CASE avec cas multiples** a introduit des bugs quand j'ai implémenté la gestion des virgules entre les cas. Je confondais parfois les séparateurs et les opérateurs.

Ce projet m'a permis de mieux comprendre les compilateurs, l'assembleur x86-64 et les mécanismes bas niveau.
