require "Cpp/Common";
require "Cpp/FileGenerator";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

NamespaceHFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,
    
    new = function(args)
        if (args == nil or args.node == nil or (args.path == nil and args.writer == nil)) then
            error('Expected a table to create new instance with fields "node" and "path" or "writer".', 2);
        end        
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
            if (writer == nil) then
                error(tostring(errorNumber) .. " " .. errorMsg, 2);                
            end
            args.writer = writer;
        end
        
        setmetatable(args, NamespaceHFileGenerator);                
        return args;
    end,
    parse = function(self)
        
    end,
};

Util.linkToSubClass(FileGenerator, NamespaceHFileGenerator);