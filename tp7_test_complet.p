VAR
  i:INTEGER;
  b:BOOLEAN;
  d:DOUBLE;
  c:CHAR.

i := 42;
b := TRUE;
d := 3.14159;
c := 65;

DISPLAY i;
DISPLAY b;
DISPLAY d;
DISPLAY c;

i := 66;
c := i;
DISPLAY c;

i := 2;
d := i;
DISPLAY d;

IF b THEN
BEGIN
  DISPLAY i;
  DISPLAY d
END
ELSE
  DISPLAY c;

i := 0;
WHILE i < 3 DO
BEGIN
  DISPLAY i;
  i := i + 1
END;

FOR i := 5 TO 7 DO
  DISPLAY i.
