VAR
    a, b, sum: INTEGER;
    is_even, is_positive: BOOLEAN;
    pi, radius, area: DOUBLE;
    first_letter, next_letter: CHAR.

a := 42;
b := 10;
sum := a + b;
DISPLAY sum;

a := sum - b;
DISPLAY a;

b := a * 2;
DISPLAY b;

a := b / 4;
DISPLAY a;

b := a % 3;
DISPLAY b;

is_even := (a % 2) == 0;
is_positive := a > 0;
DISPLAY is_even;
DISPLAY is_positive;

is_positive := is_even && is_positive;
DISPLAY is_positive;

is_positive := FALSE || TRUE;
DISPLAY is_positive;

is_positive := !FALSE;
DISPLAY is_positive;

pi := 3.14159;
radius := 5;
area := 78.53975;
DISPLAY pi;
DISPLAY radius;
DISPLAY area;

first_letter := 'A';
DISPLAY first_letter;

next_letter := 66;
DISPLAY next_letter;

a := 1;
IF is_positive THEN
BEGIN
    a := a * 10;
    DISPLAY a
END
ELSE
BEGIN
    a := 0;
    DISPLAY a
END;

a := 0;
WHILE a < 5 DO
BEGIN
    DISPLAY a;
    a := a + 1
END;

FOR a := 1 TO 3 DO
BEGIN
    b := a * 2;
    DISPLAY b;
    radius := a;
    DISPLAY radius;
    next_letter := a + 64;
    DISPLAY next_letter
END.
