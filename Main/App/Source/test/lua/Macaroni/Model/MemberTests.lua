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
require "Macaroni.Model.Axiom";
require "Macaroni.Model.Member";

local Axiom = Macaroni.Model.Axiom;
local Member = Macaroni.Model.Member;

Test.register(
{	
    name = "Member Tests",    
    tests = {    
        {
            name = "Subclasses of Member must initialize specific vars properly.",
            init = function(this)                
                this.context = Context.New("{~ROOT~}");
                this.root = this.context.Root;                
            end,
            tests = {
                ["Failure to set the Node before exiting the method causes failure."] = function(this)                    
                    local newNode = this.root:FindOrCreate("Bad");
                    local status, err = pcall(function() 
                        local member = Member.New(newNode, {
                            ctor = function(super, node)
                                super.setNode(node);
                            end,
                        });     
                    end);
                    Test.assertEquals(false, status);
                    
                    local member = Member.New(newNode, {
                            ctor = function(super, node)
                                -- do not set Node.
                            end,
                        }); 
                    -- test ends without failure.
                end,
                ["Example of successful arbitrary Member construction."] = function(this)                    
                    local newNode = this.root:FindOrCreate("Bad");                    
                    local member = Member.New(newNode, 
                        { -- subclass specification:
                            ctor = function(super, node)
                                super.setNode(node);
                            end,
                        }
                    ); 
                    -- test ends without failure.
                end,
            }
        }
    }    
}); -- End of register call
