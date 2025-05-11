VAR
a:INTEGER;
b:INTEGER;
c:INTEGER;
is_even:BOOLEAN;
is_positive:BOOLEAN;
sum:INTEGER.

a := 10;
b := 5;
c := 0;
sum := 0;

DISPLAY a;
DISPLAY b;

c := a + b;
DISPLAY c;

is_even := (c % 2) == 0;
is_positive := c > 0;

IF is_even && is_positive THEN
BEGIN
    DISPLAY 100;
    a := a * 2
END
ELSE
    DISPLAY 200;

DISPLAY a;

a := 1;
WHILE a < 5 DO
BEGIN
    sum := sum + a;
    a := a + 1
END;

DISPLAY sum;

FOR b := 1 TO 3 DO
BEGIN
    DISPLAY b;
    sum := sum + b
END;

DISPLAY sum;

IF TRUE THEN
    DISPLAY 42
ELSE
    DISPLAY 0;

IF FALSE THEN
    DISPLAY 0
ELSE
    DISPLAY 99;

DISPLAY 9999.
