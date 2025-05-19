VAR
    i, j: INTEGER;
    d: DOUBLE;
    c: CHAR;
    b: BOOLEAN.

i := 42;
d := 3.14;
c := 'A';
b := TRUE;

DISPLAY i;
DISPLAY d;
DISPLAY c;
DISPLAY b;

i := 0;
WHILE i < 3 DO
BEGIN
    j := i;
    DISPLAY j;
    i := i + 1
END;

FOR i := 1 TO 3 DO
BEGIN
    d := i;
    DISPLAY d
END.