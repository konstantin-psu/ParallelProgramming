//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Domain Map Example 2
//
// linux> ./dmap2 -nl 4
// [BD]              [CD1]             [CD2]             [BCD]
// 0 0 0 0 1 1 1 1   0 1 0 1 0 1 0 1   3 2 3 2 3 2 3 2   0 0 0 1 1 1 0 0 
// 0 0 0 0 1 1 1 1   2 3 2 3 2 3 2 3   1 0 1 0 1 0 1 0   0 0 0 1 1 1 0 0 
// 0 0 0 0 1 1 1 1   0 1 0 1 0 1 0 1   3 2 3 2 3 2 3 2   2 2 2 3 3 3 2 2 
// 0 0 0 0 1 1 1 1   2 3 2 3 2 3 2 3   1 0 1 0 1 0 1 0   2 2 2 3 3 3 2 2 
// 2 2 2 2 3 3 3 3   0 1 0 1 0 1 0 1   3 2 3 2 3 2 3 2   0 0 0 1 1 1 0 0 
// 2 2 2 2 3 3 3 3   2 3 2 3 2 3 2 3   1 0 1 0 1 0 1 0   0 0 0 1 1 1 0 0 
// 2 2 2 2 3 3 3 3   0 1 0 1 0 1 0 1   3 2 3 2 3 2 3 2   2 2 2 3 3 3 2 2 
// 2 2 2 2 3 3 3 3   2 3 2 3 2 3 2 3   1 0 1 0 1 0 1 0   2 2 2 3 3 3 2 2
// 

use BlockDist, CyclicDist, BlockCycDist;

config const n = 8;
const D = {1..n, 1..n};
const BD = D dmapped Block(D);
const CD1 = D dmapped Cyclic(startIdx=D.low);
const CD2 = D dmapped Cyclic(startIdx=D.high);
const BCD = D dmapped BlockCyclic(startIdx=D.low, blocksize=(2,3));

var b: [BD] int;
var c1: [CD1] int;
var c2: [CD2] int;
var bc: [BCD] int;

forall e in b do e = here.id;
forall e in c1 do e = here.id;
forall e in c2 do e = here.id;
forall e in bc do e = here.id;

writeln("[BD]");
writeln(b);
writeln("[CD1] (cyclic from D.low)");
writeln(c1);
writeln("[CD2] (cyclic from D.high)");
writeln(c2);
writeln("[BCD] (block-cyclic (2,3) from D.low)");
writeln(bc);
