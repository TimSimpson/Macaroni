--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--------------------------------------------------------------------------------
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Cpp/LibraryConfigGenerator";
require "Macaroni.Model.Library";
require "Log";

CPP_GENERATE_VERBOSE = false

function Generate(library, path)
	log = log.Init("Cpp");
	--log.Write = function(self, msg) 			
	--		print("[CPP]:" .. msg);
	--	end;
	
	if BoostConfigIsAvailable(library.Context) then
		lcg = LibraryConfigGenerator.new(library);
		lcg:writeFile(path);
	end
    log:Write("Generating H Files_.\n");
    local hGen = HFileGenerator.new(library);
    log:Write("Adonde esta? " .. tostring(library) .. " !");
    hGen:iterateNodes(library.Context.Root.Children, path); 
    log:Write("Generating Cpp Files\n");
    local cppGen = CppFileGenerator.new(library);
    cppGen:iterateNodes(library.Context.Root.Children, path); 
    log:Write("End of Cpp.lua\n");
end
