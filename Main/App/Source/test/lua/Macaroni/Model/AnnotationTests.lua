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
local Access = require "Macaroni.Model.Cpp.Access";
local AnnotationTable = require "Macaroni.Model.AnnotationTable";
local AnnotationValue = require "Macaroni.Model.AnnotationValue";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";

--local Access = Macaroni.Model.Cpp.Access;

local Context = require "Macaroni.Model.Context";



Test.register(
{
name = "Annotation Tests",
tests = {
    {   name = "Creating a node with some attributes.",
        init = function(this)
            this.context = Context.New("{r}");
            this.root = this.context.Root;
            this.newNode = this.root:FindOrCreate("Dog");
            this.attr = this.newNode.Annotations;
        end,
        tests = {
            ["Find returns the same Node that was created."] = function(this)
                local foundNode = this.root:Find("Dog");
                Test.assertEquals(this.newNode, foundNode);
            end,
        }
    },
} -- end of tests
}); -- End of register call
