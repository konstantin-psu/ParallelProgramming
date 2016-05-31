//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Jacobi method for solving a Laplace equation.  
// (shared-memory version)
//

use Time;
var timer, timer1, timerTotal: Timer;

config const epsilon = 0.0001;	// convergence tolerance
config const n = 8, verbose = 0;
const D = {0..n-1, 0..n-1};   // domain including boundary points
const innerDomain = {1..(n-2), 1..(n-2)};



const BDr1 = {1..n-2 by 2, 1..n-2 by 2};
const BDr2 = {2..n-2 by 2, 2..n-2 by 2};
const BDb1 = {1..n-2 by 2, 2..n-2 by 2};
const BDb2 = {2..n-2 by 2, 1..n-2 by 2};


// Jacobi iteration -- return the iteration count.
// 
proc jacobi(x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [D] real = x;

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

proc gauss(x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [D] real = x;

    do {	
        xnew = x;
        forall ij in innerDomain do {
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        }
        const delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

proc red_black(x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [D] real = x;

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


proc run_test(times:[{0..2}] real, res:[{0..2}] int, testType: int, a: [D] real) {
    var cnt: int = 0;
    init_array(D,a);
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

proc jobDoneByNormalOrder(x: [D] real) {
        forall ij in innerDomain do {
            x(ij) = here.id;
        }
        writeln("Normal Order");
        writeln(x);
        writeln("");
}

proc jobDoneByRedBlackOrder(x: [D] real) {
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

proc init_array(D: domain(2), x: [D] real) {
    x = 0.0;
    x[n-1, 0..n-1] = 1.0;         // - setting boundary values
    x[0..n-1, n-1] = 1.0;
}

// Main routine.
//
proc main() {
    var a: [D] real = 0.0;	// mesh array
    var res:  [{0..2}] int = 0;
    var times: [{0..2}] real = 0;
    if (verbose != 0) {
        jobDoneByNormalOrder(a);
        jobDoneByRedBlackOrder(a);
    }
    writeln(" ");
    for i in 0..2 do
        run_test(times, res, i, a);
    writeln("Mesh size: ", n, " x ", n, ", epsilon=", epsilon);
    writeln("Jacobi Gauss-Seidel Red-Black");
    writeln(res, " -- convergence steps");
    writeln(times, " -- total runtime");
}
