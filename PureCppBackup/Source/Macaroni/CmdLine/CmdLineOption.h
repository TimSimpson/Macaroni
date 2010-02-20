#ifndef MACARONI_COMPILE_GUARD_Macaroni_CmdLine_CmdLineOption_H
#define MACARONI_COMPILE_GUARD_Macaroni_CmdLine_CmdLineOption_H

// This class was originally defined in Source/Macaroni/CmdLine/CmdLineOption.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace CmdLine { 
class CmdLineOption;
} } // End namespace 

#include <string>

namespace Macaroni { namespace CmdLine { 


/* Public Global Methods */

class CmdLineOption
{
public:
	CmdLineOption(const std::string  name, const std::string  description);
	inline const std::string &  GetDescription() const	{
		
        return description;
    
	}
	inline const std::string &  GetName() const	{
		
        return name;
    
	}
	void  SetDescription(const std::string &  value);
	void  SetName(const std::string &  value);
protected:
private:
	std::string  description;
	std::string  name;
}; // End of class CmdLineOption

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_CmdLine_CmdLineOption_H