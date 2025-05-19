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
