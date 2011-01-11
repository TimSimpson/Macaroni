require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function formatTimeToString(date)
    return (date.year .. "-" .. date.month .. "-" .. date.day .. "-" .. 
            date.hour .. ":" .. date.min .. ":" .. date.sec);
end

function localTime()
    return formatTimeToString( os.date("*t") );
end

function utc()
    return formatTimeToString( os.date("!*t") );
end

function Generate(library, path)    
    local commaVersion = string.gsub(library.Version, "%.", "\,");
    local version = library.Version;    
    local file = path:NewPath("/Macaroni/VersionNo.h");
    local writer = file:CreateFile();

    writer:Write([[
#ifndef MACARONI_VERSION

#define MACARONI_VERSION ]] .. commaVersion .. [[ 
#define MACARONI_VERSION_STRING "]] .. version .. [["
#define MACARONI_FILE_DESCRIPTION "Macaroni for C++"
#define MACARONI_COPYRIGHT "(C) Tim Simpson, 2011"
#define BUILD_TIMESTAMP_LOCAL "]] .. localTime() .. [["
#define BUILD_TIMESTAMP_UTC "]] .. utc() .. [["
#endif
]]);    

end
