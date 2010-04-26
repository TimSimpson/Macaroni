require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

print("HEELLEELELEO!");

function Generate(library, path)
    print "Generating build.jam for the Generated source.\n";
    local buildjam = path:NewPath("/build.jam");
    print("Opening " .. buildjam.AbsolutePath .. "\n");
    --local writer, errorMsg, errorNumber = io.open(buildjam.AbsolutePath, 'w+');
    --if (writer == nil) then
    --    error(tostring(errorNumber) .. " " .. errorMsg, 2);
    --end
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
    local wroteFile = writer:Close();
    if (wroteFile) then 
        print("It looks like we wrote the file.\n");
    else 
        print("The file did not get written.\n");
    end
end
