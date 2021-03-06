PROJECT(Macaroni CXX)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(Source/main/mcpp
                    GeneratedSource
                    ${CONAN_INCLUDE_DIRS})

ADD_LIBRARY(LibMacaroni STATIC
    GeneratedSource/Macaroni/AppPathsPtr.cpp
    GeneratedSource/Macaroni/AppPaths.cpp
    GeneratedSource/Macaroni/Build/BadArgException.cpp
    GeneratedSource/Macaroni/Build/CmdLine.cpp
    GeneratedSource/Macaroni/Build/LibraryId.cpp
    GeneratedSource/Macaroni/Build/NewProjectWizard.cpp
    GeneratedSource/Macaroni/Core/Hash.cpp
    GeneratedSource/Macaroni/Core/BaseVisitor.cpp
    GeneratedSource/Macaroni/Core/Visitor.cpp
    GeneratedSource/Macaroni/Doc/MDocParser.cpp
    GeneratedSource/Macaroni/Environment/ClassicPlugin.cpp
    GeneratedSource/Macaroni/Environment/Console.cpp
    GeneratedSource/Macaroni/Environment/Environment.cpp
    GeneratedSource/Macaroni/Environment/Input.cpp
    GeneratedSource/Macaroni/Environment/InputPtr.cpp
    GeneratedSource/Macaroni/Environment/LuaEnvironment.cpp
    GeneratedSource/Macaroni/Environment/Messages.cpp
    Source/main/mcpp/Macaroni/Environment/MessagesLua.cpp
    GeneratedSource/Macaroni/Environment/Output.cpp
    GeneratedSource/Macaroni/Environment/OutputPtr.cpp
    GeneratedSource/Macaroni/Environment/PlatformConsole.cpp
    GeneratedSource/Macaroni/Environment/PluginPtr.cpp
    GeneratedSource/Macaroni/Environment/Plugin.cpp
    GeneratedSource/Macaroni/Environment/PluginFactoryPtr.cpp
    GeneratedSource/Macaroni/Environment/PluginFactory.cpp
    GeneratedSource/Macaroni/Environment/ProjectEnvironment.cpp
    GeneratedSource/Macaroni/Environment/ProjectEnvironmentPtr.cpp
    GeneratedSource/Macaroni/Environment/ProjectLoader.cpp
    GeneratedSource/Macaroni/Environment/ProjectLoaderMarker.cpp
    GeneratedSource/Macaroni/Environment/ProjectLoaderPtr.cpp
    Source/main/mcpp/Macaroni/Environment/RegisterInternalModules.cpp
    GeneratedSource/Macaroni/Environment/Timer.cpp
    GeneratedSource/Macaroni/Environment/Values/Basic.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_Model_Context.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_Model_Project_Files.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_IO_FileSet.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_Environment_Output.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_IO_Path.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_IO_PathList.cpp
    GeneratedSource/Macaroni/Environment/Values
    GeneratedSource/Macaroni/Environment/Values/Macaroni_Model_Project_Target.cpp
    GeneratedSource/Macaroni/Environment/Values/Macaroni_Model_Project_ProjectVersion.cpp
    GeneratedSource/Macaroni/Environment/Values/UserData.cpp
    GeneratedSource/Macaroni/Exceptions.cpp
    GeneratedSource/Macaroni/Exception.cpp
    GeneratedSource/Macaroni/Generators/CMake.cpp
    GeneratedSource/Macaroni/Generators/CMake/CMakeFileWriterLuaMetaData.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeGraph.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeText.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeTextProducer.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeTextSorter.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CppFileWriter.cpp
    GeneratedSource/Macaroni/Generators/Cpp/HollowNodeException.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CyclicNodeDetected.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CyclicNodeError.cpp
    GeneratedSource/Macaroni/Generators/Cpp/EmptyImportListError.cpp
    GeneratedSource/Macaroni/Generators/Cpp/MissingCodeBlockError.cpp
    GeneratedSource/Macaroni/Generators/Cpp/UnitTargetNotSetError.cpp
    GeneratedSource/Macaroni/Generators/Cpp/GeneratorSettings.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Library/LibraryConfigGenerator.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Project/MhFileGenerator.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Project/ProjectGenerator.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Project/ProjectGeneratorPtr.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Project/ProjectGeneratorLuaMetaData.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Unit/Sorter.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Unit/UnitFileGenerator.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Unit/UnitFileGeneratorPtr.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Unit/UnitText.cpp
    GeneratedSource/Macaroni/InternalSource.cpp
    GeneratedSource/Macaroni/IO/FileSetPtr.cpp
    GeneratedSource/Macaroni/IO/FileSet.cpp
    GeneratedSource/Macaroni/IO/ostreamptr.cpp
    GeneratedSource/Macaroni/IO/GeneratedFileWriterPtr.cpp
    GeneratedSource/Macaroni/IO/GeneratedFileWriter.cpp
    Source/main/mcpp/Macaroni/IO/Path.cpp
    Source/main/mcpp/Macaroni/IO/PathLua.cpp
    GeneratedSource/Macaroni/IO/Paths.cpp
    GeneratedSource/Macaroni/IO/RegexFileSetPtr.cpp
    GeneratedSource/Macaroni/IO/RegexFileSet.cpp
    GeneratedSource/Macaroni/IO/RelativeFileSet.cpp
    GeneratedSource/Macaroni/IO/RegexFileSetIterator.cpp
    GeneratedSource/Macaroni/IO/UniquePathList.cpp
    Source/main/mcpp/Macaroni/Model/AnnotationDefinition.cpp
    Source/main/mcpp/Macaroni/Model/AnnotationTable.cpp
    Source/main/mcpp/Macaroni/Model/AnnotationTableLua.cpp
    Source/main/mcpp/Macaroni/Model/AnnotationValue.cpp
    Source/main/mcpp/Macaroni/Model/AnnotationValueLua.cpp
    GeneratedSource/Macaroni/Model/Axiom.cpp
    GeneratedSource/Macaroni/Model/AxiomPtr.cpp
    Source/main/mcpp/Macaroni/Model/Block.cpp
    Source/main/mcpp/Macaroni/Model/BlockLua.cpp
    GeneratedSource/Macaroni/Model/Context.cpp
    Source/main/mcpp/Macaroni/Model/ContextLua.cpp
    GeneratedSource/Macaroni/Model/ContextPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Access.cpp
    GeneratedSource/Macaroni/Model/Cpp/AccessPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Class.cpp
    GeneratedSource/Macaroni/Model/Cpp/ClassPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/ClassLua.cpp
    GeneratedSource/Macaroni/Model/Cpp/ClassParent.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/ClassParentListLuaMetaData.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeGraphPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Constructor.cpp
    GeneratedSource/Macaroni/Model/Cpp/ConstructorPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/ConstructorLua.cpp
    GeneratedSource/Macaroni/Model/Cpp/ConstructorOverload.cpp
    GeneratedSource/Macaroni/Model/Cpp/ConstructorOverloadPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/ConstructorOverloadLua.cpp
    GeneratedSource/Macaroni/Model/Cpp/CppContext.cpp
    GeneratedSource/Macaroni/Model/Cpp/Destructor.cpp
    GeneratedSource/Macaroni/Model/Cpp/DestructorPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Enum.cpp
    GeneratedSource/Macaroni/Model/Cpp/EnumPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/ExceptionSpecifier.cpp
    GeneratedSource/Macaroni/Model/Cpp/Extern.cpp
    GeneratedSource/Macaroni/Model/Cpp/ExternPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Function.cpp
    GeneratedSource/Macaroni/Model/Cpp/FunctionPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/FunctionLua.cpp
    GeneratedSource/Macaroni/Model/Cpp/FunctionOverload.cpp
    GeneratedSource/Macaroni/Model/Cpp/FunctionOverloadPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/FunctionOverloadLua.cpp
    GeneratedSource/Macaroni/Model/Cpp/Namespace.cpp
    GeneratedSource/Macaroni/Model/Cpp/NamespacePtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Primitive.cpp
    GeneratedSource/Macaroni/Model/Cpp/PrimitivePtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Scope.cpp
    GeneratedSource/Macaroni/Model/Cpp/ScopePtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/ScopeMember.cpp
    GeneratedSource/Macaroni/Model/Cpp/ScopeMemberPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/TemplateParameterList.cpp
    GeneratedSource/Macaroni/Model/Cpp/TemplateParameterListPtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/TemplateTypename.cpp
    GeneratedSource/Macaroni/Model/Cpp/TemplateTypenamePtr.cpp
    GeneratedSource/Macaroni/Model/Cpp/Typedef.cpp
    GeneratedSource/Macaroni/Model/Cpp/TypedefPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/TypedefLua.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/TypeInfo.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/TypeInfoLua.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/Variable.cpp
    GeneratedSource/Macaroni/Model/Cpp/VariableAssignment.cpp
    GeneratedSource/Macaroni/Model/Cpp/VariableAssignmentPtr.cpp
    Source/main/mcpp/Macaroni/Model/Cpp/VariableLua.cpp
    GeneratedSource/Macaroni/Model/Element.cpp
    GeneratedSource/Macaroni/Model/ElementPtr.cpp
    Source/main/mcpp/Macaroni/Model/ElementLua.cpp
    GeneratedSource/Macaroni/Model/FileName.cpp
    GeneratedSource/Macaroni/Model/Import.cpp
    GeneratedSource/Macaroni/Model/ImportList.cpp
    Source/main/mcpp/Macaroni/Model/ModelInconsistencyException.cpp
    Source/main/mcpp/Macaroni/Model/Node.cpp
    Source/main/mcpp/Macaroni/Model/NodeListLua.cpp
    Source/main/mcpp/Macaroni/Model/NodeLua.cpp
    GeneratedSource/Macaroni/Model/Project/Artifact.cpp
    GeneratedSource/Macaroni/Model/Project/ArtifactPtr.cpp
    GeneratedSource/Macaroni/Model/Project/ArtifactList.cpp
    GeneratedSource/Macaroni/Model/Project/ArtifactListPtr.cpp
    GeneratedSource/Macaroni/Model/Project/Config.cpp
    GeneratedSource/Macaroni/Model/Project/ConfigPtr.cpp
    GeneratedSource/Macaroni/Model/Project/ConfigLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ExeTarget.cpp
    GeneratedSource/Macaroni/Model/Project/ExeTargetPtr.cpp
    GeneratedSource/Macaroni/Model/Project/FileList.cpp
    GeneratedSource/Macaroni/Model/Project/FileListPtr.cpp
    GeneratedSource/Macaroni/Model/Project/Files.cpp
    GeneratedSource/Macaroni/Model/Project/FilesPtr.cpp
    GeneratedSource/Macaroni/Model/Project/Group.cpp
    GeneratedSource/Macaroni/Model/Project/GroupPtr.cpp
    GeneratedSource/Macaroni/Model/Project/GroupList.cpp
    GeneratedSource/Macaroni/Model/Project/GroupListPtr.cpp
    GeneratedSource/Macaroni/Model/Project/LibraryTarget.cpp
    GeneratedSource/Macaroni/Model/Project/LibraryTargetPtr.cpp
    GeneratedSource/Macaroni/Model/Project/PlatformName.cpp
    GeneratedSource/Macaroni/Model/Project/PlatformList.cpp
    GeneratedSource/Macaroni/Model/Project/PlatformListPtr.cpp
    GeneratedSource/Macaroni/Model/Project/Project.cpp
    GeneratedSource/Macaroni/Model/Project/ProjectPtr.cpp
    GeneratedSource/Macaroni/Model/Project/ProjectVersionId.cpp
    GeneratedSource/Macaroni/Model/Project/ProjectVersion.cpp
    GeneratedSource/Macaroni/Model/Project/ProjectVersionPtr.cpp
    GeneratedSource/Macaroni/Model/Project/Resource.cpp
    GeneratedSource/Macaroni/Model/Project/ResourcePtr.cpp
    GeneratedSource/Macaroni/Model/Project/TargetPtr.cpp
    GeneratedSource/Macaroni/Model/Project/Target.cpp
    GeneratedSource/Macaroni/Model/Project/TargetFileSweeper.cpp
    GeneratedSource/Macaroni/Model/Project/TargetListPtr.cpp
    GeneratedSource/Macaroni/Model/Project/TargetList.cpp
    GeneratedSource/Macaroni/Model/Project/TargetLuaExtra.cpp
    GeneratedSource/Macaroni/Model/Project/UnitTarget.cpp
    GeneratedSource/Macaroni/Model/Project/Utils.cpp
    GeneratedSource/Macaroni/Model/Reason.cpp
    GeneratedSource/Macaroni/Model/SimpleTypeModifiers.cpp
    GeneratedSource/Macaroni/Model/Source.cpp
    GeneratedSource/Macaroni/Model/Type.cpp
    GeneratedSource/Macaroni/Model/TypeList.cpp
    GeneratedSource/Macaroni/Model/TypePtr.cpp
    GeneratedSource/Macaroni/Model/TypeArgumentList.cpp
    GeneratedSource/Macaroni/Model/TypeArgumentListPtr.cpp
    GeneratedSource/Macaroni/Model/TypeModifiers.cpp
    GeneratedSource/Macaroni/Parser/Cpp/CppAxioms.cpp
    GeneratedSource/Macaroni/Parser/MacaroniAxioms.cpp
    GeneratedSource/Macaroni/Parser/Parser.cpp
    GeneratedSource/Macaroni/Parser/ParserException.cpp
    Source/main/mcpp/Macaroni/Parser/ParserExceptionLua.cpp
    Source/main/mcpp/Macaroni/Parser/ParserLua.cpp
    Source/main/mcpp/Macaroni/Parser/Pippy/PippyParser.cpp
    Source/main/mcpp/Macaroni/Parser/Pippy/PippyParserLua.cpp
    GeneratedSource/Macaroni/Parser/StringItr.cpp
    GeneratedSource/Macaroni/Parser/TextIterator.cpp
    GeneratedSource/Macaroni/Platform/EnvironmentVariable.cpp
    GeneratedSource/Macaroni/Platform/EnvironmentVariables.cpp
    Source/main/mcpp/Macaroni/Platform/FileTime.cpp
    Source/main/mcpp/Macaroni/Platform/Windows/Strings.cpp
    GeneratedSource/Macaroni/StringException.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeType.cpp
    GeneratedSource/Macaroni/Generators/Cpp/ElementCodeSection.cpp
    GeneratedSource/Macaroni/Model/CvQualifier.cpp
    GeneratedSource/Macaroni/Model/ImportType.cpp
    GeneratedSource/Macaroni/IO/FileSetLuaMetaData.cpp
    GeneratedSource/Macaroni/IO/RegexFileSetLuaMetaData.cpp
    GeneratedSource/Macaroni/IO/GeneratedFileWriterLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ProjectVersionLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/TargetLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ResourceLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/FilesLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/LibraryTargetLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/UnitTargetLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ExeTargetLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/TargetListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/GroupListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/GroupLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ProjectLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/FileListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ArtifactLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/ArtifactListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Project/PlatformListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Cpp/ClassParentLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Cpp/EnumLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Cpp/AccessLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Cpp/TemplateTypenameLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Cpp/VariableAssignmentLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/Cpp/TemplateParameterListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/FileNameLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/SourceLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/ReasonLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/TypeLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/AxiomLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/TypeModifiersLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/SimpleTypeModifiersLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/TypeArgumentListLuaMetaData.cpp
    GeneratedSource/Macaroni/Model/TypeListLuaMetaData.cpp
    GeneratedSource/Macaroni/Environment/OutputLuaMetaData.cpp
    GeneratedSource/Macaroni/Environment/PluginLuaMetaData.cpp
    GeneratedSource/Macaroni/Environment/PluginFactoryLuaMetaData.cpp
    GeneratedSource/Macaroni/Environment/ProjectEnvironmentLuaMetaData.cpp
    GeneratedSource/Macaroni/Core/HashLuaMetaData.cpp
    GeneratedSource/Macaroni/Generators/Cpp/CodeGraphLuaMetaData.cpp
    GeneratedSource/Macaroni/Generators/Cpp/Project
    GeneratedSource/Macaroni/Generators/Cpp/Unit/UnitFileGeneratorLuaMetaData.cpp
)

target_link_libraries(LibMacaroni ${CONAN_LIBS})


ADD_EXECUTABLE(macaroni GeneratedSource/Macaroni.cpp)
TARGET_LINK_LIBRARIES(macaroni LibMacaroni)

IF (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    TARGET_LINK_LIBRARIES(macaroni stdc++)
ENDIF()
