require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function Generate(library, path)
    print "Generating build.jam for the Generated source directory.\n";
    local buildjam = path:NewPath("/build.jam");
    print("Opening " .. buildjam.AbsolutePath .. "\n");   
    local writer = buildjam:CreateFile();
    writer:Write([[
# Generated by Jam Generator.
alias generated
	:	[ glob-tree *.c	: .svn ]
		[ glob-tree *.cpp	: .svn ]
	:	<include>./
		<include>../Source
	;
    ]]);    
end