require "Macaroni.Model.Axiom";

local Axiom = Macaroni.Model.Axiom;

Test.register(
{	
    name = "Axiom Tests",    
    tests = {    
        {
            name = "Creating an Axiom in Lua.",
            init = function(this)
                this.axiom = Axiom.LuaCreate("barf");
            end,
            tests = {
                ["Attempting tostring() on Axiom retrieves words."] = function(this)
                    Test.assert("barf", tostring(this.axiom));           
                end,
            }
        }
    }    
}); -- End of register call
