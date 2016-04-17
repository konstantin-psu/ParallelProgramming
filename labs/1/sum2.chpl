// The sum program (Chapel version).
//
//
use BlockDist;
config const N = 100;	/* problem domain size */
const D = {1..N};
const BlockD = D dmapped Block(boundingBox=D);
var sum: int;

proc compute(i: int): int { 
  return i*i;
}
sum = + reduce [i in BlockD] (compute(i));
writeln("The result sum is " + sum);
