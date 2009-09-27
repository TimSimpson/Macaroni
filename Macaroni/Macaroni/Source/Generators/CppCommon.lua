
require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Context";
require "Macaroni.Model.Cpp.Function";
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.Cpp.VariableAssignment";
require "debug";
require "io";

-- Eventually put this next part in C++.
Macaroni.Model.TypeNames = 
{ 
    Class = "Class", 
    Constructor="Constructor", 
    Function="Function", 
    Namespace = "Namespace", 
    Primitive="Primitive",
    Variable="Variable"
};



Common = {

};