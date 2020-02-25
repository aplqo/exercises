line[{x1_, y1_}, {x2_, y2_}] := 
 First[Solve[  
   Rationalize[x1]^2 a + Rationalize[x1] b == Rationalize[y1] && 
    Rationalize[x2]^2 a + Rationalize[x2] b == Rationalize[y2], {a, 
    b}]]
eval[x_, {a -> a_, b -> b_}] := a x^2 + b x