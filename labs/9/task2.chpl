use BlockDist;

const D = {0..3};
const MD = D dmapped Block(D);
var a: [D] int = 0;
var b: [MD] int = 0;
writeln("Case 1:");
forall i in D {
    writeln(i, ": ", here.id, ", ", a[i].locale.id, ", ", b[i].locale.id);
}
writeln("Case 2:");
forall i in MD {
    writeln(i, ": ", here.id, ", ", a[i].locale.id, ", ", b[i].locale.id);
}
