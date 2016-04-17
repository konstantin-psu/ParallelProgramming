// The sum program (Chapel version).
//
//
config const N = 100;	/* problem domain size */
const D = {1..N};
var sum: int;

proc compute(i: int): int { 
  return i*i;
}
sum = + reduce [i in D] (compute(i));
writeln("The result sum is " + sum);
