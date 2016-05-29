//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Jacobi method for solving a Laplace equation.  
// (shared-memory version)
//
use Time;
var timer: Timer;

config const epsilon = 0.001;	// convergence tolerance
config const n = 8; 	        // mesh size (including boundary)
var lock$: sync bool;

// Jacobi iteration -- return the iteration count.
// 
proc jacobi(D: domain(2), x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [D] real = x;
    const innerDomain = {1..(n-2), 1..(n-2)};

    var delta: real = 0.0;

    do {	
        delta = 0.0;
        for ij in innerDomain do
            xnew(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;

        delta = max reduce abs(xnew[innerDomain] - x[innerDomain]);
        x = xnew;
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

proc gaussIter(D: domain(2), x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    const innerDomain = {1..(n-2), 1..(n-2)};

    var delta: real = 0.0;

    do {	
        delta = 0.0;
        for ij in innerDomain do {
            var old = x(ij);
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0; 
            delta = max (delta, abs(old - x(ij)));
        }
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

proc gauss(D: domain(2), x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    const innerDomain = {1..(n-2), 1..(n-2)};
    var xnew: [D] real = x;

    var delta: real = 0.0;

    do {	
        delta = 0.0;
        xnew = x;
        for ij in innerDomain do
            x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;

        //delta = max reduce abs(x[innerDomain]);
        delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

proc red_black(D: domain(2), x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var start: int = 0;
    /*
     *  how to ensure red and black order?
     */
    const innerDomain = {1..(n-2), 1..(n-2) by 2};
    var xnew: [D] real = x;

    var delta: real = 0.0;

    do {	
        delta = 0.0;
        xnew = x;
        for i in 1..n-2 do {
            if (i%2 == 0) {
                start = 2;
            } else {
                start = 1;
            }
            for j in 1..n-2 do {
                x(i,j) = (x(i-1,j) + x(i,j-1) + x(i + 1,j) + x(i,j+1)) / 4.0;
            }
        }

        for i in 1..n-2 do {
            if (i%2 == 0) {
                start = 1;
            } else {
                start = 2;
            }
            for j in 1..n-2 do {
                x(i,j) = (x(i-1,j) + x(i,j-1) + x(i + 1,j) + x(i,j+1)) / 4.0;
            }
        }
        // forall ij in innerDomain do {
        //     x(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;
        // }

        //delta = max reduce abs(x[innerDomain]);
        delta = max reduce abs(x[innerDomain] - xnew[innerDomain]);
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}


proc run_test(D: domain(2), testType: int) {
    writeln("");
    var a: [D] real = 0.0;	// mesh array
    var cnt: int = 0;
    init_array(D,a);
    if (testType == 0) {
        timer.start();
        writeln("jacobi");
        cnt = jacobi(D, a, epsilon);
        timer.stop();
    } else if (testType == 1) {
        writeln("gauss");
        cnt = gauss(D, a, epsilon);
    } else if (testType == 2) {
        writeln("red black");
        cnt = red_black(D, a, epsilon);
        //writeln(a[0..2*(n-1)]);
        //return;
    } else {
        return;
    }

    writeln("Mesh size: ", n, " x ", n, ", epsilon=", epsilon, 
            ", total Jacobi iterations: ", cnt);
    writeln("Elapsed time " + timer.elapsed());
    // writeln(a);
}

proc init_array(D: domain(2), x: [D] real) {
    x = 0.0;
    x[n-1, 0..n-1] = 1.0;         // - setting boundary values
    x[0..n-1, n-1] = 1.0;
}

// Main routine.
//
proc main() {
    const D = {0..n-1, 0..n-1};   // domain including boundary points
    write(n);
    write("");
    for i in 0..0 do
        run_test(D, i);
}
