require "Cpp/Common"
require "Cpp/FileGenerator"

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

ClassFileGenerator = {

};

Util.linkToSubClass(FileGenerator, ClassFileGenerator);
