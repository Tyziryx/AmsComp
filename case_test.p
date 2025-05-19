VAR
  day: INTEGER;
  grade: CHAR.

day := 3;

CASE day OF
  1: DISPLAY 'M';
  2: DISPLAY 'T';
  3: DISPLAY 'W';
  4: DISPLAY 'T';
  5: DISPLAY 'F';
  6, 7: DISPLAY 'S'
END;

grade := 'B';

CASE grade OF
  'A': DISPLAY 90;
  'B': DISPLAY 80;
  'C': DISPLAY 70;
  'D', 'F': DISPLAY 60
END.
