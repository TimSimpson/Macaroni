////#ifndef MACARONI_BUILD_LOCALMANIFESTRESOLVER_H
////#define MACARONI_BUILD_LOCALMANIFESTRESOLVER_H
////
////#include <Macaroni/Build/LibraryId.h>
////#include <boost/none.hpp>
////
////namespace Macaroni { namespace Build {
////
/////** Manifest client code should hand this to a Manifest so it can resolve
//// * the parent and child libraries within it.  You see, everything else uses
//// * LibraryId, but these use relative file paths, which is beyond the scope of
//// * the Manifest's responsibilities.  So this class is used for that.
//// **/
////struct LocalManifestResolver
////{
////	virtual boost::optional<LibraryId> FindParent(
////		const boost::optional<std::string &> & parentPath) = 0;
////	
////	virtual LibraryId FindChild(const std::string & childPath) = 0;
////
////	virtual void SelfLibraryId(const LibraryId & myId) = 0;
////};
////
////} }
////
////#endif