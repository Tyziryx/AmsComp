VAR
a:INTEGER;
b:BOOLEAN;
c:BOOLEAN.

a := 10;

b := TRUE;
c := FALSE;

b := a > 5;

c := !b;

c := (a > 5) && (a < 20);
b := (a < 5) || (a > 7);

IF TRUE THEN
  DISPLAY 42
ELSE
  DISPLAY 0;

IF b THEN
  DISPLAY 1
ELSE
  DISPLAY 0;

DISPLAY a.