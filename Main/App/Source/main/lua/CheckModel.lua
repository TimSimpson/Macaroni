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

--TODO: Move to "test" sources once that works.

require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.IO.Path";

function Generate(library, path)
	local root = library.Context.Root;
	local cpp = root:FindOrCreate("Macaroni::Model::Cpp::ClassParentPtr")
	if cpp.Member == nil or cpp.Member.TypeName ~= "Typedef" then
		error("NO!  Was " .. cpp.Member.TypeName);
	end
	if cpp.HFilePath == nil then
		error("NO!GG");
	end


	local cmdline = root:FindOrCreate("Macaroni::Build::CmdLine")
	local ctor = cmdline:Find("$ctor")
	assert(ctor.TypeName == "Constructor")
	assert(#ctor.Children == 1)
	local ol = ctor.Children[1].Member
	assert(ol.TypeName == "ConstructorOverload")
	local args = ol.Arguments;
	local arg1 = args[1]


end
