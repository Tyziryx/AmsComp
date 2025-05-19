(* test_types.p *)
VAR
  i: INTEGER;
  d: DOUBLE;
  c: CHAR.

i := 65;
d := i;    (* Conversion INTEGER → DOUBLE *)
c := i;    (* Conversion INTEGER → CHAR - affiche 'A' *)

DISPLAY i; (* Affiche 65 *)
DISPLAY d; (* Affiche 65.0 *)
DISPLAY c. (* Affiche A *)
