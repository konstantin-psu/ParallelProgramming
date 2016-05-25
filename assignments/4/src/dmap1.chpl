//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Domain Map Example 1
//
// linux> ./dmap1 -nl 4
// 0 0 0 0 0 0 0 0
// 0 0 1 1 2 2 3 3
// 0 0 0 1 2 3 3 3
// 0 0 0 1 1 1 2 2
//

use BlockDist;

config const n = 8;
const D: domain(1) = {1..n};
const BD1 = D dmapped Block(D);
const BD2 = D dmapped Block({2..6});	// map with bounding box {2..6}
const BD3 = D dmapped Block({1..12});   // map with bounding box {1..12}

var a: [D] int;
var b1: [BD1] int;
var b2: [BD2] int;
var b3: [BD3] int;

forall e in a do e = here.id;
forall e in b1 do e = here.id;
forall e in b2 do e = here.id;
forall e in b3 do e = here.id;

writeln("[D]");
writeln(a);
writeln("[BD1] block");
writeln(b1);
writeln("[BD2] block(2..6)");
writeln(b2);
writeln("[BD3] block(1..12)");
writeln(b3);
