config const n = 6,
        epsilon = 1.0e-5;
const BigD = {0..n+1, 0..n+1} dmapped Block({1..n, 1..n}),
       D = BigD[1..n, 1..n],
        LastRow = D.exterior(1,0);
var A, Temp : [BigD] real;
A[LastRow] = 1.0;
do {
     forall (i,j) in D do
          Temp[i,j] = (A[i-1,j] + A[i+1,j] + A[i,j-1] + A[i,j+1]) / 4;
      const delta = max reduce abs(A[D] - Temp[D]);
       A[D] = Temp[D];
} while (delta > epsilon);
//writeln(A);
use BlockDist;
