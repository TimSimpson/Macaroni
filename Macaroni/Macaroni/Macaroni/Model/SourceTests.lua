require "Macaroni.Model.Source";

local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;

Test.register(
{	
    name = "Source Tests",    
    tests = {    
        {
            name = "Creating a source object.",           
            init = function(this)
                this.file = FileName.Create("blah");
                Test.assert(1, this.file.ReferenceCount);
                this.src = Source.Create(this.file, 27, 1);
            end,            
            tests = {
                ["Column should return the given column."] = function(this)
                    Test.assert(1, this.src.Column);          
                end,
                ["FileName should return the given file."] = function(this)
                    Test.assert(this.file, this.src.FileName);          
                end,
                ["Line should return the given line."] = function(this)
                    Test.assert(27, this.src.Line);          
                end,
                ["ReferenceCount on the File should have been incremented."] = function(this)
                    Test.assert(2, this.file.ReferenceCount);
                end,
                ["ReferenceCount on the Source starts as one."] = function(this)
                    Test.assert(1, this.src.ReferenceCount);
                end,
                ["ToString lookz like dis:"] = function(this)
                    Test.assert("blah, line 27", tostring(this.src));
                end,
            }
        }             
    }    
}); -- End of register call
