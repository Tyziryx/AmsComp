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

Pour compiler et exécuter un programme Pascal avec notre compilateur :

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

Le développement de ce compilateur a été un véritable parcours d'apprentissage, avec plusieurs moments mémorables:

### Les défis rencontrés

- **L'alignement pour les flottants** a été un véritable casse-tête! Je me souviens avoir passé presque deux jours à comprendre pourquoi mon programme crashait aléatoirement lors de l'appel à printf pour les DOUBLE. Le problème venait de l'alignement sur 16 octets requis par la convention d'appel System V AMD64 pour les arguments flottants.

- **Les registres XMM** étaient complètement nouveaux pour moi. J'ai dû me documenter sur les instructions SSE (Streaming SIMD Extensions) et comprendre comment manipuler ces registres spéciaux. La syntaxe des instructions comme MOVSD et ADDSD était déroutante au début.

- **L'optimisation de CASE** a pris plusieurs itérations. Ma première tentative utilisait une table de sauts (jump table), mais j'ai rencontré des problèmes avec la génération d'adresses relatives. Je suis passé à une approche avec des comparaisons séquentielles qui s'est avérée plus simple à implémenter et à déboguer.

### Les moments "Eureka"

- Le moment où j'ai réussi à comprendre et implémenter les **conversions implicites entre INTEGER et DOUBLE** a été particulièrement gratifiant. Voir un entier se transformer correctement en flottant dans les registres XMM était une petite victoire.

- L'implémentation de **REPEAT..UNTIL** a d'abord semblé facile (juste un WHILE inversé), mais en réalité, la logique des sauts était différente et j'ai dû repenser entièrement son fonctionnement.

- Quand j'ai compris que je pouvais **remplacer les constantes "inline"** au lieu de les stocker en mémoire, j'ai été content de cette optimisation simple mais efficace.

### Les bugs mémorables

- Le bug le plus tenace était lié à la **gestion de la pile** lors des appels imbriqués de fonction. J'ai remarqué que dans certains cas complexes, la pile devenait désalignée, causant des crashes mystérieux.

- J'ai passé beaucoup de temps à déboguer un problème où les **valeurs booléennes** n'étaient pas correctement propagées dans les expressions conditionnelles, jusqu'à ce que je comprenne qu'en utilisant 0xFFFFFFFFFFFFFFFF pour TRUE (au lieu de simplement 1), les opérations bit à bit fonctionnaient mieux.

- La syntaxe du CASE avec valeurs multiples a introduit quelques bugs amusants lorsque j'ai essayé de gérer les virgules entre les cas. La distinction entre séparateurs et opérateurs était parfois confuse.

Ces défis ont rendu le projet passionnant et m'ont permis d'approfondir ma compréhension des compilateurs, de l'assembleur x86-64 et des systèmes bas niveau en général.
