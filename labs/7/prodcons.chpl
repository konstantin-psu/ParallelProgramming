//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A producer-consumer program with a bounded buffer.
//

config const buffSize = 10,
       numTasks = 2000,
       numProds = 2,
       numCons = 3;

var buff$: [0..buffSize-1] sync int;	// circular task buffer
var head$: sync int = 0;		// head position (for removal)
var tail: int = 0;			// tail position (for addition)
var total$: sync int = 0;		// total removed tasks
var moreWork: bool = true;		// termination flag 
var produced: int = 0;
var lock$: sync bool;

// The producer routine
// 
proc producer(id: int) { 
    writeln("producer " + id + " started ...");
    var prod: int = 0;
    while true {
    // for i in 1..numTasks {
 
        lock$ = true; // the sync variable is set to full
        produced += 1;
        prod = produced;
        buff$[tail] = prod;
        writeln("producer added task " + prod
                + " to buff[" + tail + "]");
        tail = (tail + 1) % buffSize;
        var unlock = lock$; // empty the variable allowing the next process in 
        if prod >= numTasks {
            break;
        }
    }
}

// The consumer routine
// 
proc consumer(id: int) { 
    writeln("consumer " + id + " started ...");
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
    for i in 1..numProds {
        begin producer(i);
    }
    coforall i in 1..numCons {
        consumer(i);
    }
    var prod = produced;
    writeln("Total tasks produced " + prod);
}
