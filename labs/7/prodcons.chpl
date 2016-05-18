//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A producer-consumer program with a bounded buffer.
//

config const buffSize = 10,
             numTasks = 20,
             numCons = 3;

var buff$: [0..buffSize-1] sync int;	// circular task buffer
var head$: sync int = 0;		// head position (for removal)
var tail: int = 0;			// tail position (for addition)
var total$: sync int = 0;		// total removed tasks
var moreWork: bool = true;		// termination flag 

// The producer routine
// 
proc producer() { 
  writeln("producer started ...");
  for i in 1..numTasks {
    buff$[tail] = i;
    writeln("producer added task " + i 
	    + " to buff[" + tail + "]");
    tail = (tail + 1) % buffSize;
  }
}

// The consumer routine
// 
proc consumer(id: int) { 
  writeln("producer " + id + " started ...");
  var cnt, idx, val: int;
  while moreWork {
    cnt = total$;
    if cnt == numTasks - 1 then
      moreWork = false;
    total$ = cnt + 1;
    if cnt < numTasks {
      idx = head$;
      val = buff$[idx];
      head$ = (idx + 1) % buffSize;
      writeln("consumer " + id + " removed task " 
	      + val + " from buff[" + idx + "]");
      }
    }
}

// The main routine
// 
proc main() {
  begin producer();
  for i in 1..numCons {
      begin consumer(i);
  }
}
