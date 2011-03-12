require "Macaroni.Model.AttributeTable"
require "Macaroni.Model.AttributeValue";
require "Macaroni.Model.Axiom";
require "Macaroni.Model.Block";
require "Macaroni.Model.Cpp.Class";
require "Cpp/Common";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Cpp.Function";
require "Macaroni.IO.GeneratedFileWriter";
require "LuaGlue/LuaGlueCppFile";
require "LuaGlue/LuaGlueHFile";
require "Macaroni.Model.Library";
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.IO.Path";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.Cpp.Variable";
require "Cpp/NodeInfo";
require "Log";

Axiom = Macaroni.Model.Axiom;
Block = Macaroni.Model.Axiom;
Class = Macaroni.Model.Cpp.Class;
Context = Macaroni.Model.Context;
FileName = Macaroni.Model.FileName;
Function = Macaroni.Model.Cpp.Function;
Member = Macaroni.Model.Member;
NodeList = Macaroni.Model.NodeList;
Reason = Macaroni.Model.Reason;
Source = Macaroni.Model.Source;
Type = Macaroni.Model.Type;
Variable = Macaroni.Model.Cpp.Variable;

local check = function(con, msg)
	if (not con) then
		error(msg, 3);
	end
end

local notNull = function(name, value)
	if (value == nil) then
		error('Bad argument:"' .. name .. '" cannot be null.', 3);
	end
end


HtmlViewGenerator = 
{
	new = function(rootNode, library, writer)    
		check(rootNode ~= nil, 'Argument 1, "rootNode", must be specified.');
        local self = {}
        setmetatable(self, {["__index"] = HtmlViewGenerator});                                 
        self.library = library;
        self.rootNode = rootNode;
        self.writer = writer;
        return self;
    end,    
    
    getNodeId = function(self, node)
		notNull("self", self);
		notNull("node", node);
		local id = node:GetPrettyFullName('-');
		if (id == "") then
			return node.Name;
		else
			return id;
		end		
    end,
    
    rootNode = nil,
    	
    run = function(self)
		self:write([[
<html>
<head>
<title>]] .. self.library.Name .. [[</title>
<style type="text/css">
div.show { }
div.hide { display:none; }
</style>
</head>
<body>
]]);
	self:writeNode(self.rootNode);
	self:write([[
<script type="text/javascript">
function change(id) 
{
	var e = document.getElementById(id);
	var c = e.getAttribute("class");
	if (c == "hide") 
	{
		e.setAttribute("class", "show");
	}
	else
	{
	e.setAttribute("class", "hide");
	}
}
change('%ROOT%');
</script>
</body>
</html>		
		]]);
    end,
    
    write = function(self, text)
        if (type(text) ~= "string") then
            error("String was expected in call to write, but got " .. type(text) .. " instead.", 2);
        end
        --log:Write("DEBUG:" .. debug.traceback());       
        self.writer:Write(text);
    end,         
    
    writeDivBegin = function(self, id)
		self:write([[<div id="]] .. id .. [[" class="hide" >]]);	
    end,
    
    writeDivEnd = function(self)
		self:write([[</div>]]);
    end,
    
    writeEntryStart = function(self, id, name)
		self:write('<a name="' .. id 
				.. [[" href="javascript:change(']] .. id .. [[');">]] 
				.. name .. [[</a>]]);
    end,
    
    writeMemberDetails = function(self, node, member)
		if (member == nil) then
			self:write("<i>hollow</i>");
		else 
			local typeName = member.TypeName;
			self:write(typeName);			
			if (typeName == "Class") then
				self:write(" ");
				local id = self:getNodeId(node);					
				self:writeEntryStart(id .. "-class-extras", "Additional Info");
				self:writeDivBegin(id .. "-class-extras");
				local writeNL = function(id, name, list)
					self:writeEntryStart(id, name);
					self:write("<br/>");
					self:writeDivBegin(id);
					self:writeNodeList(list, false);
					self:writeDivEnd();
				end							
				writeNL(id .. "-imports", "imports", member.ImportedNodes);
				writeNL(id .. "-friend-nodes", "friends", member.FriendNodes);
				writeNL(id .. "-globals", "globals", member.GlobalNodes);				
				self:writeDivEnd();
			end
			local reason = member.ReasonCreated;
			local srcFile = tostring(reason.Source.FileName);        
			local src = reason.Source;
			self:write([[<br/>
			]] .. tostring(reason.Axiom) .. [[<br/>
			<i>]] .. tostring(src.FileName) .. [[, line ]] 
			.. tostring(src.Line) .. [[, column ]] 
			.. tostring(src.Column) .. [[</i><br/>
			]]);
		end
    end,
    
    writeNode = function(self, node)
		local id = self:getNodeId(node); 
		local name = node.Name;		
		self:writeEntryStart(id, name);		
		self:writeDivBegin(id);
		self:writeMemberDetails(node, node.Member);
		if node.HFilePath ~= nil then
			self:write("<br/>~hfile=" .. tostring(node.HFilePath) .. "<br/>");
		else
			self:write("<br/>~hfile=null<br/>");
		end
		self:write([[ <br/>]]);		
		self:writeNodeList(node.Children, true);		
		self:writeDivEnd();
    end,
        
    writeNodeList = function(self, nodeList, recurse)
		self:write([[<ul>]]);
		for i = 1, #nodeList do
			local element = nodeList[i]
			self:write([[<li>]]);
			if (recurse) then
				self:writeNode(element);
			else
				self:write('<a href="#' .. self:getNodeId(element) .. '">' .. element.FullName .. '</a>');
			end
			self:write([[</li>]]);
		end
		self:write([[</ul>]]);
    end,      
    
    
    
    writer = nil
     
}; -- end HtmlViewGenerator class



function Generate(library, path, arguments)	
	log.Init("HtmlView");
	arguments = arguments or {}	
    CurrentLibrary = library;
    local writer = path:NewPath("/macaroni-model.html"):CreateFile();
    local generator = HtmlViewGenerator.new(library.Context.Root, library, writer);    
    generator:run();
end


