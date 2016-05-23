//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Jacobi method for solving a Laplace equation.  
// (shared-memory version)
//

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
        forall ij in innerDomain do
            xnew(ij) = (x(ij+(-1,0)) + x(ij+(0,-1)) + x(ij+(1,0)) + x(ij+(0,1))) / 4.0;

        delta = max reduce abs(xnew[innerDomain] - x[innerDomain]);
        x = xnew;
        cnt+= 1;
    } while (delta > epsilon);

    return cnt;
}

// Main routine.
//
proc main() {
    const D = {0..n-1, 0..n-1};   // domain including boundary points
    var a: [D] real = 0.0;	// mesh array
    var b: [D] real = 0.0;	// mesh array
    a[n-1, 0..n-1] = 1.0;         // - setting boundary values
    a[0..n-1, n-1] = 1.0;
    var cnt = jacobi(D, a, epsilon);
    writeln("Mesh size: ", n, " x ", n, ", epsilon=", epsilon, 
            ", total Jacobi iterations: ", cnt);
    writeln(a);
}
