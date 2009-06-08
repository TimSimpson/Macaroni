require "Macaroni.Model.FileName";

local FileName = Macaroni.Model.FileName;

Test.register(
{	
    name = "FileName Tests",    
    tests = {    
        ["Creating a file."] = function(this)
            local file = FileName.Create("blah");
            Test.assert("blah", tostring(file));
        end,
        ["Reference counting"] = function(this)
            local file = FileName.Create("blah");            
            Test.assert(1, file.ReferenceCount);
            local file2 = file; --lua makes no copy.
            Test.assert(1, file.ReferenceCount);
        end,
    }    
}); -- End of register call
