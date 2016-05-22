//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Jacobi method for solving a Laplace equation.  
// (shared-memory version)
//

config const epsilon = 0.001;	// convergence tolerance
config const n = 8; 	        // mesh size (including boundary)

proc max(x: real, y: real) {
    if (x > y) { return x; }
    return y;
}
// Jacobi iteration -- return the iteration count.
// 
proc jacobi(D: domain(2), x: [D] real, epsilon: real) { 
    var cnt = 0;			// iteration counter
    var xnew: [D] real = x;

    // ... need code here ...

    //   double xnew[n][n];	// buffer for new values    
    var delta: real = 0.0;		// measure of convergence   

    do {	
        delta = 0.0;
        for i  in 1..(n-2) {
            for j  in  1..(n-2) {
                xnew(i,j) = (x(i-1,j) + x(i,j-1) + x(i+1,j) + x(i,j+1)) / 4.0;
                delta = max(delta, abs(xnew(i,j) - x(i,j)));
            }
        }	
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
    // writeln("");
    // b = a;
    // writeln(b);
    // writeln("");
    // b = 0.0;
    // writeln(b);
    // writeln("");
    // writeln(a);
}
