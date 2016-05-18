//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Locale Example 3
//

writeln("start executing on locale 0 - " + Locales[0].name);
cobegin {
  on Locales[1] do
    writeln("this task runs on locale 1 - " + Locales[1].name);
  on Locales[2] do
    writeln("while this one runs on locale 2 - " + Locales[2].name);
}   
writeln("back on locale 0 again");
