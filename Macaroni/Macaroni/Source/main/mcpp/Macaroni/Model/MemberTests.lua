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
                    Test.assert(false, status);
                    
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
