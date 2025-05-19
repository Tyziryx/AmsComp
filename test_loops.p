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
