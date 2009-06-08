require "Macaroni.Model.Reason";

local Axiom = Macaroni.Model.Axiom;
local FileName = Macaroni.Model.FileName;
local Reason = Macaroni.Model.Reason;
local Source = Macaroni.Model.Source;

Test.register(
{	
    name = "Reason tests.",    
    tests = {    
        {
            name = "Creating a Reason.",
            init = function(this)
                this.file = FileName.Create("Mino.mcpp");
                this.source = Source.Create(this.file, 10);
                this.axiom = Axiom.LuaCreate("Class keyword discovered at start of line.");
                this.reason = Reason.Create(this.axiom, this.source);
            end,
            tests = {            
                ["Axiom is what was passed in."] = function(this)
                    local a = this.reason.Axiom;
                    Test.assert(this.axiom, a);
                end,                
                ["Source is what was passed in."] = function(this)
                    local src = this.reason.Source;
                    Test.assert(this.source, src);
                end,
                ["Tostring concats both relevant parts."] = function(this)
                    Test.assert("Mino.mcpp, line 10: Class keyword discovered at start of line.", 
                                tostring(this.reason));
                end,                
            }
        }
    }  -- end of all Reason Tests.  
}); -- End of register call
