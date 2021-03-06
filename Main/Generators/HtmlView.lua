require "Macaroni.Model.AnnotationTable"
require "Macaroni.Model.AnnotationValue";
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
if MACARONI_VERSION == "0.1.0.22" then
	require "Macaroni.Model.Member";
else
	require "Macaroni.Model.Element";
end
require "Cpp/NodeInfo";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.IO.Path";

require "Cpp/NodeInfo";
require "Log";
require "Plugin"

local Axiom = require "Macaroni.Model.Axiom";
local Block = require "Macaroni.Model.Axiom";
local Class = require "Macaroni.Model.Cpp.Class";
local Context = require "Macaroni.Model.Context";
local FileName = require "Macaroni.Model.FileName";
local Function = require "Macaroni.Model.Cpp.Function";
local Member = require "Macaroni.Model.Element";
local NodeList = require "Macaroni.Model.NodeList";
local Reason = require "Macaroni.Model.Reason";
local Source = require "Macaroni.Model.Source";
local Type = require "Macaroni.Model.Type";
local Variable = require "Macaroni.Model.Cpp.Variable";

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

badHtmlChars =
{
	{ '&', '&amp;'},
	{ '<', '&lt;'},
	{ '>', '&gt;'},
	{ '"', '&quot;'},
	{ ' ', '&nbsp;'},
};

codeToHtml = function(text)
	for i = 1, #badHtmlChars do
		text = string.gsub(text, badHtmlChars[i][1], badHtmlChars[i][2])
	end
	return text
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

    getEntryText = function(self, node)
		if node.Member == nil then
			return "<i>" .. node.Name .. "</i>";
		end
		local typeName = node.Member.TypeName;
		if typeName == 'Class' then
			return [[<font color="#000077" face="courier"><b>]]
				   .. node.Name .. "</b></font>";
		elseif typeName == 'Namespace' then
			return [[<font color="#0000CC" face="courier">]]
				   .. node.Name .. "</font>";
		elseif typeName == 'Function' or typeName =="FunctionOverload"
		       or typeName == 'Constructor' or typeName == "ConstructorOverload"
		       or typeName == 'Destructor' or typeName == "DestructorOverload"
		       then
			return [[<font color="#770000" face="courier">]]
				   .. node.Name .. "</font>";
		elseif typeName == 'Typedef' then
			return [[<font color="#380077" face="courier"><b>]]
				   .. node.Name .. "</b></font>";
		elseif typeName == 'Variable' then
			return [[<font color="#FF0077" face="courier">]]
				   .. node.Name .. "</font>";
		elseif typeName == 'AnnotationDefinition' then
			return [[<font color="#777700" face="courier"><b>]]
				   .. node.Name .. "</b></font>";
		elseif typeName == 'Block' then
			return [[<font color="#005500" face="courier">]]
				   .. node.Name .. "</font>";
		else
			return [[??! ]]
				   .. node.Name .. "";
		end
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
div.show { font-family: Courier; }
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
	var c = e.className;
	if (c == "hide")
	{
		e.className = "show"; //Annotation("class", "show");
	}
	else
	{
		e.className = "hide";
		//e.setAnnotation("class", "hide");
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
		check(name ~= nil, 'Argument #3 name cannot be nil.')
		self:write('<a name="' .. id
				.. [[" href="javascript:change(']] .. id .. [[');">]]
				.. name .. [[</a>]]);
    end,

	writeBlockDetails = function(self, node, member)
		local id = self:getNodeId(node);
		self:write("<br/>");
		self:writeEntryStart(id .. "-block-extras", node.Member.Id);
		self:writeDivBegin(id .. "-block-extras");
		self:write("<br/>");
		self:write(codeToHtml(node.Member.Code));
		self:write("<br/>");
		self:writeDivEnd();
	end,

    writeClassDetails = function(self, node, member)
		local id = self:getNodeId(node);
		self:write("<li>");
		self:writeEntryStart(id .. "-class-extras", "Additional Info");
		self:writeDivBegin(id .. "-class-extras");
		local writeNL = function(id, name, list)
			self:writeEntryStart(id, name);
			self:write("<br/>");
			self:writeDivBegin(id);
			self:write([[<ul>]]);
			self:writeNodeList(list, false);
			self:write([[<ul>]]);
			self:writeDivEnd();
		end
		writeNL(id .. "-imports", "imports", member.ImportedNodes);
		writeNL(id .. "-friend-nodes", "friends", member.FriendNodes);
		writeNL(id .. "-globals", "globals", member.GlobalNodes);
		self:writeDivEnd();
		self:write("</li>");
    end,

    writeCppNodeInfo = function(self, node, member)
		if node.IsRoot then
			return
		end
		local id = self:getNodeId(node);
		self:write("<li>");
		self:writeEntryStart(id .. "-nodecppinfo", "C++ Node Info");
		self:writeDivBegin(id .. "-nodecppinfo");
		local info = NodeInfoList[node];
		self:write([[<ul>]]);
		self:write([[<li>Header File: ]] .. codeToHtml(info.headerFile)
				   .. [[</li>]]);
		self:write([[<li>Light Def: ]] .. codeToHtml(info._lightDef)
		            .. [[</li>]]);
		self:write([[<li>Heavy Def: ]] .. codeToHtml(info._heavyDef)
		           .. [[</li>]]);
		self:write([[<li>Using Statement: ]] .. codeToHtml(info.using)
		            .. [[</li>]]);
		self:write([[<li>Dependencies: ]]);
			self:write([[<ul>]]);
				self:write([[<li>Light]]);
					self:write([[<ul>]]);
						for n in info.dependencies:iterateLightDependencies() do
							self:write([[<li>]] .. n.FullName .. [[</li>]]);
						end
					self:write([[</ul>]]);
				self:write([[</li>]]);
				self:write([[<li>Heavy]]);
					self:write([[<ul>]]);
						for n in info.dependencies:iterateHeavyDependencies() do
							self:write([[<li>]] .. n.FullName .. [[</li>]]);
						end
					self:write([[</ul>]]);
				self:write([[</li>]]);
			self:write([[</ul>]]);
		self:write([[</li>]]);
		self:write([[</ul>]]);
		local writeNL = function(id, name, list)
			self:writeEntryStart(id, name);
			self:write("<br/>");
			self:writeDivBegin(id);
			self:write([[<ul>]]);
			self:writeNodeList(list, false);
			self:write([[<ul>]]);
			self:writeDivEnd();
		end
		self:writeDivEnd();
		self:write("</li>");
    end,

    writeLocationInfo = function(self, node, member)
		local id = self:getNodeId(node);
		self:write("<li>");
		self:writeEntryStart(id .. "-location-info", "Location Info");
		self:writeDivBegin(id .. "-location-info");
			local reason = member.ReasonCreated;
			local srcFile = tostring(reason.Source.FileName);
			local src = reason.Source;
			self:write([[<br/>
			<i>]] .. tostring(reason.Axiom) .. [[</i><br/>
			<i>]] .. tostring(src.FileName) .. [[, line ]]
			.. tostring(src.Line) .. [[, column ]]
			.. tostring(src.Column) .. [[</i><br/>
			]]);
			if node.HFilePath ~= nil then
				self:write("<br/>~hfile=" .. tostring(node.HFilePath) .. "<br/>");
			else
				self:write("<br/>~hfile=null<br/>");
			end
		self:writeDivEnd();
		self:write("</li>");
    end,

    writeMemberDetails = function(self, node, member)
		if (member == nil) then
			self:write("<i>hollow</i>");
		else
			local id = self:getNodeId(node);
			local typeName = member.TypeName;
			self:write("<table border='1'><tr><td>");
			self:writeEntryStart(id .. "-node-info",
				[[<font color="#000000" face="courier">]]
				.. typeName .. ' ' .. node.FullName .. [[</font>]]);
			self:write([[<ul>]]);
			self:writeDivBegin(id .. "-node-info");

			self:write(typeName);
			if (typeName == "Class") then
				self:writeClassDetails(node, member);
			elseif (typeName == "Block") then
				self:writeBlockDetails(node, member);
			end

			self:writeLocationInfo(node, member);

			self:writeCppNodeInfo(node, member);

			self:writeDivEnd();
			self:write([[</ul>]]);
			self:write("</td></tr></table>");
		end
    end,

    writeNode = function(self, node)
		local id = self:getNodeId(node);
		local name = node.Name;
		local entryText = self:getEntryText(node);
		self:writeEntryStart(id, entryText);
		self:writeDivBegin(id);
		self:write([[<ul>]]);
		self:writeMemberDetails(node, node.Member);
		self:writeNodeList(node.Children, true);
		self:write([[</ul>]]);
		self:writeDivEnd();
    end,

    writeNodeList = function(self, nodeList, recurse)
		-- Alphabetize so its easier to see.
		local t = {}
		for i = 1, #nodeList do
			t[#t + 1] = nodeList[i]
		end
		table.sort(t, function(a, b)
			return string.lower(a.FullName) < string.lower(b.FullName)
		end)
		for i = 1, #t do
			local element = t[i]
			self:write([[<li>]]);
			if (recurse) then
				self:writeNode(element);
			else
				self:write('<a href="#' .. self:getNodeId(element) .. '">' .. element.FullName .. '</a>');
			end
			self:write([[</li>]]);
		end
    end,



    writer = nil

}; -- end HtmlViewGenerator class



function Generate(library, path, arguments)
	log.Init("HtmlView");
	--log.Write = function(self, msg)
	--	print("[HTML]:" .. msg);
	--end;

	arguments = arguments or {}
    CurrentLibrary = library;
    local writer = path:NewPath("/macaroni-model.html"):CreateFile();
    local generator = HtmlViewGenerator.new(library.Context.Root, library, writer);
    generator:run();
end


function validateArgs(self)
	Plugin.Check(self.target ~= nil, "Missing argument 'target'.")
    Plugin.Check(self.path ~= nil, "Missing argument 'path'.")
end


function GetMethod(name)
    if name == "Generate" then
        return
        {
            Describe = function(args)
                validateArgs(args);
                args.output.WriteLine("Write HTML describing the target "
                                      .. tostring(args.target)
                                      .. " to the directory "
                                      .. tostring(args.path) .. ".");
            end,
            Run = function(args)
                validateArgs(args);
                Generate(args.target, args.path, args)
            end
        }
    end
    return nil;
end
