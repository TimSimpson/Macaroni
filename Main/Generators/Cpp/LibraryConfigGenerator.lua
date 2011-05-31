-- Generates the Library Config file.


_BoostConfigIsAvailable = nil

BoostConfigIsAvailable = function(context)
	-- The thinking here is that if anything from boost is defined then the 
	-- headers should be on the path and we can safely assume boost/config is
	-- there.
	if _BoostConfigIsAvailable == nil then
		_BoostConfigIsAvailable = (context.Root:Find("boost") ~= nil);
	end
	return _BoostConfigIsAvailable;
end

LibraryConfigFile = function(library)
	return "Config_" .. library:GetCId() .. ".h"
end

LibraryCreate = function(library)
	local libId = library:GetCId();
	local libCreate = "MACARONI_LIB_CREATE_" .. libId;
	return libCreate;
end

LibraryDecl = function(library)
	-- A helper method which returns the Macro to affix to header declarations.
	if BoostConfigIsAvailable(library.Context) then
		return "MACARONI_LIB_DECL_" .. library:GetCId();
	end 
	return "";
end

LibraryMetaTarget = function(library)
	return "MACARONI_LIB_TARGET_" .. library:GetCId();
end

LibraryDynLink = function(library)
	local libId = library:GetCId();
	local libDynLink = "MACARONI_LIB_DYN_LINK_" .. libId;
	return libDynLink;
end

LibraryConfigGenerator =
{
	new = function(library)
		self = {}
		self.library = library
		self.writeFile = LibraryConfigGenerator.writeFile;
		return self;
	end,
	
	writeFile = function(self, path)
		local libId = self.library:GetCId();
		self.file = path:NewPath("/" .. LibraryConfigFile(self.library));
		self.writer = self.file:CreateFile();
		
		local libDecl = LibraryDecl(self.library);
		local libDynLink = LibraryDynLink(self.library);
		local libCreate = LibraryCreate(self.library);
		
		self.writer:Write(
[[#ifndef MACARONI_COMPILE_GUARD_Config_]] .. libId .. [[_H
#define MACARONI_COMPILE_GUARD_Config_]] .. libId .. [[_H

#include <boost/config.hpp>

// Creates macros needed to create portable library code.
//
// Macros:
//
// ]] .. libDynLink .. [[]] .. "\n" .. [[
//     If defined, this code will be built or used as a library.
//     If not defined, then the code will be compiled into the resulting binary.
//
// ]] .. libCreate .. [[]] .. "\n" .. [[
//     If defined, we are building this library.  If not, we are using it.
//
// ]] .. libDecl .. [[]] .. "\n" .. [[
//     This is defined by this header file by looking at the above values.
//     This may or may not become a compiler attribute affixed to all symbol
//     definitions.  This is necessary for certain compilers (specifically MSVC++).
//

#if defined(BOOST_ALL_DYN_LINK) || defined(]] .. libDynLink .. [[)
# if defined(]] .. libCreate .. [[)
#  define ]] .. libDecl .. [[ BOOST_SYMBOL_EXPORT
# else
#  define ]] .. libDecl .. [[ BOOST_SYMBOL_IMPORT
# endif
#else
# define ]] .. libDecl .. [[ 
#endif

#endif // end of MACARONI_COMPILE_GUARD_Config_]] .. libId .. [[_H
]]);
		self.writer:Close();
	end,
}