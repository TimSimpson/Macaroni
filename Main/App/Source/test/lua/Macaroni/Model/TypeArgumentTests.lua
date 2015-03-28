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
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local NodeList = require "Macaroni.Model.NodeList";
local Type = require "Macaroni.Model.Type";
local TypeList = require "Macaroni.Model.TypeList";

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    self.nodeA = self.context.Root:FindOrCreate("TypeA");
    self.nodeB = self.context.Root:FindOrCreate("TypeB");
    self.nodeC = self.context.Root:FindOrCreate("TypeC");

    self.rootType = self.context:CreateType()
    self.typeList = self.rootType:AddArgument(1)

    self.typeA = self.typeList:CreateType()
    self.typeA.Node = self.nodeA;
    self.typeB = self.typeList:CreateType()
    self.typeB.Node = self.nodeB;
    self.typeC = self.typeList:CreateType()
    self.typeC.Node = self.nodeC;

end
