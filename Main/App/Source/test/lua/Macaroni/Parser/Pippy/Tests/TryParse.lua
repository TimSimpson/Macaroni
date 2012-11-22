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
require "Macaroni.IO.Path";
local Path = Macaroni.IO.Path;


-- Helper to write parser tests.
ParserTest = {
	tryParse = function(text)
		local parser = PippyParser.Create();
		local context = Context.New("{ROOT}");
		local file = FileName.Create("Blah1.mcpp");
		local src = Source.Create(file, 1, 1);

		local result = nil;
		local status, err = pcall(function()
				result = parser:Read(context, src, text);
			end
		);
		return status, err;
	end,

	init = function(self)
		self.parser = Macaroni.Parser.Pippy.PippyParser.Create();
        self.context = Macaroni.Model.Context.New("{ROOT}");
        self.library = self.context:FindOrCreateLibrary("Tests",
                              "TestGeneratedLib", "1.0");
        self.file = Macaroni.Model.FileName.Create(Path.New("", "Blah1.mcpp"));
        self.root = self.context.Root;
        self.src = Macaroni.Model.Source.Create(self.file, 1, 1);
	end,
};


