//~import Macaroni::Build::Builder;
//~import Macaroni::Environment::Console;
//~import Macaroni::Model::Context;
//~import Macaroni::Model::ContextPtr;
//~import Macaroni::IO::FileSet;
//~import Macaroni::Exception;
//~import Macaroni::Model::Library;
//~import Macaroni::Model::LibraryPtr;
//~import Macaroni::Environment::LuaEnvironment;
//~import Macaroni::Build::Manifest;
//~import Macaroni::Build::MCompiler;
//~import Macaroni::Build::MCompilerOptions;
//~import Macaroni::Environment::Messages;
//~import std::string;
//~import std::stringstream;
//~import std::vector;
//
//namespace Macaroni::CmdLine 
//{ 
//
//class CmdLineOptions
//{
//	vector<CmdLineOption> options;
//	
//	public CmdLineOptions()
//	:	options()
//	{
//		options.push_back(CmdLineOption(
//	}
//	
//	public inline const string & GetDescription() const
//	{
//		return description;
//	}
//
//	public inline const string & GetName() const
//	{
//		return name;
//	}
//
//	public void SetDescription(const string & description)
//	{
//		description = description;
//	}
//
//	public void SetName(const string & name)
//	{
//		name = name;
//	}
//
//}
//
//} //  end ns