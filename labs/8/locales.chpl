//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 
//
//


coforall loc in Locales do
    writeln("Locale " + loc.id + ": " + loc.name + " (with " + loc.numCores + " cores)");
