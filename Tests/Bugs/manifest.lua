require "os"
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library"
require "Macaroni.IO.Path";

Path = Macaroni.IO.Path;


upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name= upper.Name .. ".Bugs",
    version= upper.Version -- "1.0.0.0"
}

description="Current and former known bugs experienced in Macaroni."

output = "target"

bugs = 
{
	"2",
	"1",
};

function generate()
end

function build()	
	local manifestDir = Path.New(manifestDirectory);	
	print("HI " .. manifestDirectory);
	local mD = manifestDir:NewPathForceSlash("target/mdoc/bugreport.mdoc");
	print("FFSD");
	local writer = manifestDir:NewPathForceSlash("target/mdoc/bugreport.mdoc"):CreateFile();	
	print("BYE");
	writer:WriteLine([[
	<table>
	<tr><td>Name</td><td>Status</td><td>Description</td></tr>
	]]);
	print([[__ BUG REPORT ]]);
	for k,v in pairs(bugResults) do		
		local trClass;
		if (v.success) then 
			trClass = "bug-recreate-success";
		else
			trClass = "bug-recreate-failure";
		end
		local name;
		if (v.success) then
			name = v.library.Name;
		else
			name = tostring(k);
		end
		local success;
		if (v.success == true) then
			success = "success";
		else
			success = "FAILED";
		end			
		local desc;		
		if (v.description == nil) then
			desc = "Unknown.  There was an error parsing the manifest for this recreate. :(";
		else
			desc = v.description;
		end
		writer:WriteLine([[
		<tr class="]] .. trClass .. [["><td>]] .. name .. [[<td>
		    <td>]] .. success .. [[</td>
		    <td>]] .. desc .. [[</td>
		    </tr>
		]]);		
		print([[__ ]] .. name .. [[ .. ]] .. success);
	end
	
	writer:WriteLine([[
	</table>		
	]]);	
	writer:Close();
end

function install()
	run("Site");
end


