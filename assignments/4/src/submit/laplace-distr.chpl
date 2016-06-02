//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Jacobi method for solving a Laplace equation.  
// (shared-memory version)
//
use BlockDist;
use Time;
var timer, timer1, timerTotal: Timer;

// Parameters
config const epsilon = 0.001;  // convergence tolerance
config const n = 8;            // n - matrix dimension size, verbose - verbosity flag
config const verbose = 0;  // --ver verbosity [true|false] - false default
const D = {0..n-1, 0..n-1};    // 2 dimensional domain
const id = {1..n-2,1..n-2};    // inner domain
const BD = D dmapped Block(D); // distribute among locales
const innerDomain = BD[id];

// Cell is red if i + j is even
const BDr1 = BD[1..n-2 by 2, 1..n-2 by 2]; // Red 1 - odd column odd row
const BDr2 = BD[2..n-2 by 2, 2..n-2 by 2]; // Red 2 - even column even row
// Cell is black if i + j is odd
const BDb1 = BD[1..n-2 by 2, 2..n-2 by 2]; // Black 1 - odd column even row
const BDb2 = BD[2..n-2 by 2, 1..n-2 by 2]; // Black 2 - even column odd row

proc verbosePrint(x: [BD] real, xnew: [BD] real) {
}

// Jacobi distributed -- return the iteration count.
// 
proc jacobi(x: [BD] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [BD] real = x;

    do {	
        forall ij in innerDomain do { // innerDomain is distributed so job is done on appropriate locale
                                      // Will Generate lots of spam
                                      // Will Generate lots of spam
                                      // here.id is the locale that is running current iteration
                                      // x(ij).locale.id is the locale that holds the data
                                      // when x(ij+offset).locale.id != here.id message passing is required
            if (verbose) {
                writeln("here.id, x(ij).locale.id: ["+here.id+" , "+x(ij).locale.id + "," +  "]");
            }
            xnew(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        const delta = max reduce abs(xnew[innerDomain] - x[innerDomain]);
        x = xnew;
        cnt+= 1;
    } while (delta > epsilon);

    writeln("");
    return cnt;
}

// Gauss-Seidel Normal Order distributed -- return the iteration count.
// 
proc gauss(x: [BD] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [BD] real = x;

    do {	
        xnew = x;
        forall ij in innerDomain do {
            if (verbose) {
                 writeln("here.id, x(ij).locale.id: ["+here.id+" , "+x(ij).locale.id + "]");
            }
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }
        const delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

// Gauss-Seidel Red-Black Order distributed -- return the iteration count.
// 
proc red_black(x: [BD] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [BD] real = x;

    do {	
        xnew = x;
        forall ij in BDr1 do {
            if (verbose) {
                 writeln("here.id, x(ij).locale.id: ["+here.id+" , "+x(ij).locale.id + "]");
            }
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        forall ij in BDr2 do {
            if (verbose) {
                 writeln("here.id, x(ij).locale.id: ["+here.id+" , "+x(ij).locale.id + "]");
            }
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        forall ij in BDb1 do {
            if (verbose) {
                 writeln("here.id, x(ij).locale.id: ["+here.id+" , "+x(ij).locale.id + "]");
            }
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        forall ij in BDb2 do {
            if (verbose) {
                 writeln("here.id, x(ij).locale.id: ["+here.id+" , "+x(ij).locale.id + "]");
            }
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }
        const delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}


proc run_test(times:[{0..2}] real, res:[{0..2}] int, testType: int, a: [BD] real) {
    var cnt: int = 0;
    init_array(a);
    timer.clear();
    timer.start();
    if (testType == 0) {
        cnt = jacobi(a, epsilon);
    } else if (testType == 1) {
        cnt = gauss(a, epsilon);
    } else if (testType == 2) {
        cnt = red_black(a, epsilon);
    } else {
        timer.stop();
        return;
    }
    timer.stop();
    res(testType) = cnt;
    times(testType) = timer.elapsed();
}

proc jobDoneByNormalOrder(x: [BD] real) {
        x=0.0;
        forall ij in innerDomain do {
            x(ij) = here.id;
        }
        writeln("Normal Order locales distributed (exterior is ignored and always be 0)");
        writeln(x);
        writeln("");
}

proc jobDoneByRedBlackOrder(x: [BD] real) {
        x=0.0 ;
        forall ij in BDr1 do {
            x(ij) = here.id;
        }

        forall ij in BDr2 do {
            x(ij) = here.id;
        }

        forall ij in BDb1 do {
            x(ij) = here.id;
        }

        forall ij in BDb2 do {
            x(ij) = here.id;
        }
        writeln("Red Black Order locales distributed (exterior is ignored and always be 0)");
        writeln(x);
        writeln("");
        x=0.0;
}

proc init_array(x: [BD] real) {
    x = 0.0;
    x[n-1, 0..n-1] = 1.0;         // - setting boundary values
    x[0..n-1, n-1] = 1.0;
}

// Main routine.
//
proc main(args: [] string) {
    var a: [BD] real = 0;	// mesh array
    var res:  [{0..2}] int = 0;
    var times: [{0..2}] real = 0;
    //if (verbose != 0) {
    writeln(" ");
    for i in 0..2 do
        run_test(times, res, i, a);

    if (verbose > 1) {
        jobDoneByNormalOrder(a);
        jobDoneByRedBlackOrder(a);
    }
    writeln("Mesh size: ", n, " x ", n, ", epsilon=", epsilon);
    writeln("Jacobi Gauss-Seidel Red-Black");
    writeln(res, " -- convergence steps");
    writeln(times, " -- total runtime");
    return 0;
}
