require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

print("HEELLEELELEO!");

--[[function iteratePath(self, path) 
	local children = path.Paths
	for i = 1, #children do
		local c = children[i]
		if (!c.IsDirectory) then
			
		else
			iteratePath(c)
		end				
	end	
end]]--

function Generate(library, path)	
    print "Generating build.jam for the Generated source.\n";
    local buildjam = path:NewPath("/jamroot.txt");
    print("Opening " .. buildjam.AbsolutePath .. "\n");
    --local writer, errorMsg, errorNumber = io.open(buildjam.AbsolutePath, 'w+');
    --if (writer == nil) then
    --    error(tostring(errorNumber) .. " " .. errorMsg, 2);
    --end
    local writer = buildjam:CreateFile();
    writer:Write([[
# Generated by Jam Generator.

import boost ;
import path ;

boost.use-project 1.42 ;

import site-config ;


project
	:	usage-requirements
		<include>./		
		<include>../Source
	;
	
alias generated
	:	[ glob-tree *.c	: .svn ]
		[ glob-tree *.cpp	: .svn ]
	:	<include>./
		<include>../Source
	;
    
alias source
	:	[ path.glob-tree ../Source/*.c	: .svn ]
		[ path.glob-tree ../Source/*.cpp	: .svn *Test.cpp Main.cpp ]
	:	<include>./
		<include>../Source
	;
	
alias sources
	:	source
		generated
		/boost//regex # TODO: I'm including this because I don't know how to 
					  # reference the smart_ptr project... 
	;
	
exe binary
	:	/lua-5.1.4//sources
		../Source/Main.cpp	
		sources					
	;
    ]]);
    local wroteFile = writer:Close();
    if (wroteFile) then 
        print("It looks like we wrote the file.\n");
    else 
        print("The file did not get written.\n");
    end
end

function Build(library, sources, outputPath, installPath, extraArgs)
	local buildjam = outputPath:NewPath("/jamroot.jam");
	print("Creating Boost.Build file at " .. buildjam.AbsolutePath .. ".");
	local writer = buildjam:CreateFile();		
	
	local forAllSourcesWrite = function(text) 
		writer:Write(text(outputPath.AbsolutePath));
		for i = 1, #sources do
			local source = sources[i];
			writer:Write(text(source.AbsolutePath));		
		end	
	end;
	
	writer:Write([[
# Generated by Macaroni.

import boost ;
import path ;

boost.use-project 1.42 ;

import site-config ;


project
	:	usage-requirements
]]);
	forAllSourcesWrite(function(src) return " <include>" .. src .. " "; end);
	writer:Write([[
	;
	
alias localSources
	:	]]);
	forAllSourcesWrite(function(src) return [[
		[ path.glob-tree ]] .. src .. [[/ : *.c : .svn ]
		[ path.glob-tree ]] .. src .. [[/ : *.cpp : .svn ]
		]]; end);
	writer:Write(" : ");
	forAllSourcesWrite(function(src) return [[
		<include>]] .. src .. [[		
		]]; end);
	writer:Write([[
	;
	
alias sources
	:	localSources
		/boost//regex # TODO: I'm including this because I don't know how to 
					  # reference the smart_ptr project... 
	;
	
exe binary
	:	../Source/Main.cpp	
		sources					
	;
    ]]);
    local wroteFile = writer:Close();
    if (wroteFile) then 
        print("It looks like we wrote the file.\n");
    else 
        print("The file did not get written.\n");
    end
end

function Install(library, sourcePaths, outputPath, installPath)
	-- Create a Jam file which simply points to the source files.
	-- Copy all C++ source to the folder.
end

