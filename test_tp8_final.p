CONST
  PI := 3.14159;
  MAX := 100;
  NEWLINE := 10;
  IS_DEBUG := TRUE;

VAR
  a, b, sum: INTEGER;
  is_even, is_pos: BOOLEAN;
  d: DOUBLE;
  c: CHAR.

a := 42;
b := 10;
sum := a + b;
DISPLAY sum;

a := sum - b;
DISPLAY a;

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

CASE a OF
  38: DISPLAY 'A';
  39: DISPLAY 'B';
  40: DISPLAY 'C';
  41, 42: DISPLAY 'D'
END;

c := 'X';
CASE c OF
  'A': DISPLAY 1;
  'B': DISPLAY 2;
  'X': DISPLAY 3;
  'Y', 'Z': DISPLAY 4
END;

IF is_pos && is_even THEN
BEGIN
  a := a * 2;
  DISPLAY a
END
ELSE
BEGIN
  a := a + 1;
  DISPLAY a
END;

a := 0;
WHILE a < 3 DO
BEGIN
  DISPLAY a;
  a := a + 1
END;

FOR a := 1 TO 3 DO
BEGIN
  d := a;
  DISPLAY d
END.
