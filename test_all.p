[a, b, c, d, e, i, j, result, temp]

a := 100;
b := 7;
c := 3;

d := ((a / b) + (b * c)) - ((a % b) + 1);  

a := 0;
i := 5;
while i > 0 do
begin
    a := a + i;
    if a > 10 then
        i := 0
    else
        i := i - 1
end;

result := 0;
for i := 1 to a do
begin
    if i % b == 0 then
        result := result + i;
    if result > 50 then
        i := a
end;

result := a * a;
if result > 100 then
    result := 100;

a := 5;
i := 0;
while i < a do
begin
    if i % 2 == 0 then
    begin
        for j := 1 to i do
            result := result + j
    end
    else
    begin
        j := i;
        while j > 0 do
        begin
            result := result - j;
            j := j - 1
        end
    end;
    i := i + 1
end.