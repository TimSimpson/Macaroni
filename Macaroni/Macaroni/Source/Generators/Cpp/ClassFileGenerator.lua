require "Cpp/Common"
require "Cpp/FileGenerator"

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

ClassFileGenerator = {
    
};

Util.linkToSubClass(FileGenerator, ClassFileGenerator);  