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

config const epsilon = 0.001;	// convergence tolerance
config const n = 8; 	        // mesh size (including boundary)
const D = {0..n-1, 0..n-1};
const id = {1..n-2,1..n-2};
const BD = D dmapped Block(D);

const innerDomain = BD[id];
const BDr1 = BD[1..n-2 by 2, 1..n-2 by 2];
const BDr2 = BD[2..n-2 by 2, 2..n-2 by 2];
const BDb1 = BD[1..n-2 by 2, 2..n-2 by 2];
const BDb2 = BD[2..n-2 by 2, 1..n-2 by 2];
var a: [BD] real = 0;	// mesh array

// Jacobi iteration -- return the iteration count.
// 
proc jacobi(x: [BD] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [BD] real = x;

    do {	
        forall ij in innerDomain do {
            xnew(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        const delta = max reduce abs(xnew[innerDomain] - x[innerDomain]);

        x = xnew;

        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

proc gauss(x: [BD] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [BD] real = x;

    do {	
        xnew = x;
        forall ij in innerDomain do {
            // write(here.id);
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }
        const delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

proc red_black(x: [BD] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [BD] real = x;

    do {	
        xnew = x;
        forall ij in BDr1 do {
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        forall ij in BDr2 do {
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        forall ij in BDb1 do {
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        forall ij in BDb2 do {
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }

        const delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}


proc run_test(testType: int) {
    writeln("");
    var cnt: int = 0;
    a=0.0;
    init_array(a);
    timer.clear();
    timer.start();
    if (testType == 0) {
        writeln("jacobi");
        cnt = jacobi(a, epsilon);
    } else if (testType == 1) {
        writeln("gauss");
        cnt = gauss(a, epsilon);
    } else if (testType == 2) {
        writeln("red black");
        cnt = red_black(a, epsilon);
    } else {
        timer.stop();
        return;
    }
    timer.stop();

    writeln("Mesh size: ", n, " x ", n, ", epsilon=", epsilon, 
            ", total Jacobi iterations: ", cnt);
    //writeln("Elapsed distr " + timer.elapsed() + " Time reduce " + timer1.elapsed());
    writeln("Total time " + timer.elapsed());
    // writeln(a);
}
proc jobDoneByNormalOrder(x: [BD] real) {
        forall ij in innerDomain do {
            x(ij) = here.id;
        }
        writeln("Normal Order");
        writeln(x);
        writeln("");
}

proc jobDoneByRedBlackOrder(x: [BD] real) {
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
        writeln("Red Black Order");
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
proc main() {
    write("");
    // jobDoneByNormalOrder(a);
    // jobDoneByRedBlackOrder(a);
    for i in 0..2 do
        run_test(i);
}
