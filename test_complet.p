(* test_complet.p *)
CONST
  PI := 3.14159;
  MAX := 10;
  IS_DEBUG := TRUE

VAR
  a, b, sum: INTEGER;
  is_even, is_pos: BOOLEAN;
  d: DOUBLE;
  c: CHAR;
  day: INTEGER.

a := 42;
b := 10;
sum := a + b;
DISPLAY sum;

is_even := (a % 2) == 0;
is_pos := a > 0;
DISPLAY is_even;
DISPLAY is_pos;

d := PI;
DISPLAY d;

c := 'A';
DISPLAY c;

REPEAT
  a := a - 1;
  DISPLAY a
UNTIL a < 40;

day := 3;
CASE day OF
  1: DISPLAY 'L';
  2: DISPLAY 'M';
  3: DISPLAY 'W';
  4: DISPLAY 'T';
  5: DISPLAY 'F';
  6, 7: DISPLAY 'S'
END.
