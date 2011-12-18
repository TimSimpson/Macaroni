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

-- Generates the REPL handler.

require "Macaroni.Model.AnnotationTable";
require "Macaroni.Model.AnnotationValue";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";
require "Macaroni.Model.Source";

FileName = Macaroni.Model.FileName;
Source = Macaroni.Model.Source;


findFunctions = function(node)
    if (node == nil) then
        error("Node cannot be nil.", 2);
    end
    local rtn = {};
    for i = 1, #node.Children do
        local child = node.Children[i];
        if (child.Member ~= nil and
            child.Member.TypeName == "Function") then
            for j = 1, #child.Children do
                local fon = child.Children[j]; -- FunctionOverload node
                if (fon.Member ~= nil
                    and fon.Member.TypeName == 'FunctionOverload') then
                    local attr = fon.Annotations[
                        "Macaroni::Environment::ReplCommand"];
                    if attr ~= nil then
                        rtn[#rtn + 1] = {
                            fon=fon,
                            name=attr.ValueAsTable["Name"].ValueAsString,
                            summary=attr.ValueAsTable["Summary"].ValueAsString,
                        }
                    end
                end
            end
        end
    end
    return rtn;
end


function Generate(library, path)
    local root = library.Context.Root;
    local node = root:Find("Macaroni::Environment::Environment");
    local fns = findFunctions(node)
    print("Repl commands GO!")

    -- local newFuncNode= node:FindOrCreate("replCommand");
    -- local axiom = Axiom.LuaCreate("Macaroni::Environment::ReplCommand")
    local source = Source.Create(FileName.Create("ReplCommand.lua"), 0, 0);
    -- local reason = Reason.Create(axiom, source)
    -- local func = Function.Create(newFuncNode, reason);

    -- local fo1 = FunctionOverload.Create(func, false, Access.Private,
    --                                     true, false, rtnType,
    --                                     false, false, self.reason);
    local fNode = node:Find("replCommand");
    local foNode = fNode.Children[1];
    local fo1 = foNode.Member;

    local fo2 = node:Find("showHelp").Children[1].Member;
    local doElse;
    helpDoc = {};
    helpDoc[#helpDoc + 1] = "Commands:";
    methodBody = {};

    -- Sort the table (this is probably overkill...)
    local names = {}
    for i, v in ipairs(fns) do
        table.insert(names, v.name)
    end
    table.sort(names)

    local sorted = {}
    for i, v in ipairs(names) do
        for j, v2 in ipairs(fns) do
            if v2.name == v then
                table.insert(sorted, v2)
            end
        end
    end

    for i, v in ipairs(sorted) do
        if doElse then
            methodBody[#methodBody + 1] = "else ";
        else
            doElse = true
        end
        methodBody[#methodBody + 1] = [[
        if (line == "]] .. v.name .. [[" )
        {
            return ]] .. v.fon.Node.Name .. [[(line);
        }
        ]];
        helpDoc[#helpDoc + 1] = " * " .. v.name .. " - " .. v.summary
    end
    methodBody[#methodBody + 1] = "return false;"
    fo1:SetCodeBlock(table.concat(methodBody, "\n\t"), source, false);

    fo2:SetCodeBlock([[
        output->WriteLine("]] .. table.concat(helpDoc, [[");
        output->WriteLine("]]) .. [[");
        return true;
        ]],
        source, false);
end
