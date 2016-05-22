config const buffSize = 10,
       numTasks = 2000,
       numProds = 2,
       N        = 100,
       P        = 10,
       numCons = 3;

var sum: sync int  = 0; 

proc compute(i: int) {
  return i*i;
}

proc worker(tid: int) {
  var i, low, high, psum: int;
  low = (N/P) * tid; 	/* a simplistic partition scheme */
  high = low + (N/P);
  psum = 0;
  for i in low..(high - 1) {
    psum += compute(i);
  }
  var si = sum; // the sync variable is set to empty
  si = si + psum;
  sum = si;     // full the variable will allow the next process in
} 

proc main() {
  var k: int; 
  coforall k in 0..(P-1) {
      worker(k);
  }
  var s = sum;
  writeln("The result sum is " + s);
}  
