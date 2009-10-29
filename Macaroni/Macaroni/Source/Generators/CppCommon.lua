print("CppCommon.lua is a jerk.");
require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Node";
require "Macaroni.Model.Context";
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

function Generate(context, path)
    print("I play no part in this cosmic dance. I prance, and prance, and prance...");
end
--[[]]--
print("CppCommon.lua is being loaded now.");