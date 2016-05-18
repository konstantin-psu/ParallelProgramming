//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Locale Example 2
//

writeln("start executing on " + here.id 
        + " (" + here.name + " with " + here.numCores + " cores)");
  on Locales[1] do
    writeln("now we are on locale " + here.id 
            + " (" + here.name + " with " + here.numCores + " cores)");
writeln("back on locale " + here.id + " again");

