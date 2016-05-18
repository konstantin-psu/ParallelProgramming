//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Locale Example 4
//

var x: int;
on Locales(1) {
  var y: int;
  on Locales(2) {
    var z = x;
    writeln("x's locale: " + x.locale.id);
    writeln("y's locale: " + y.locale.id);
    writeln("z's locale: " + z.locale.id);
  }
} 
