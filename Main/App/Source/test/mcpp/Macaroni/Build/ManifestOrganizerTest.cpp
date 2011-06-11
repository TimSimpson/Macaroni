#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Macaroni::Build::ManifestOrganizerTest
#include <boost/test/unit_test.hpp>

#include <Macaroni/Build/LibraryId.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/ManifestOrganizer.h>

using Macaroni::Build::LibraryId;
using Macaroni::Build::Manifest;
using Macaroni::Build::ManifestPtr;
using Macaroni::Build::ManifestOrganizer;
using std::vector;

BOOST_AUTO_TEST_SUITE(AnnotationValueSuite)

BOOST_AUTO_TEST_CASE(A)
{	
	LibraryId gfx("Macaroni", "Gfx", "1.0.0.0");
	LibraryId sfx("Macaroni", "Sfx", "1.0.0.0");
	LibraryId mem("Macaroni", "Memory", "1.0.0.0");
	LibraryId platform("Macaroni", "Platform", "1.0.0.0");

	vector<LibraryId> deps;	
	ManifestPtr platformM(new Manifest(platform, deps));
	deps.push_back(platform);
	ManifestPtr memM(new Manifest(mem, deps)); // "Memory" depends on "Platform"
	deps.push_back(mem);
	ManifestPtr gfxM(new Manifest(gfx, deps)); // "Gfx" depends on "Memory"
	ManifestPtr sfxM(new Manifest(sfx, deps)); // So does Sound.

	// So, if we organize this, we see the build order should be
	// Platform, Memory, (Gfx or Sound)

	vector<ManifestPtr> manifests;
	manifests.push_back(gfxM);
	manifests.push_back(memM);
	manifests.push_back(sfxM);	
	manifests.push_back(platformM);

	ManifestOrganizer orgList(manifests);	
	orgList.Sort();

	BOOST_CHECK_EQUAL(4, manifests.size());	
	BOOST_CHECK_EQUAL("Platform", manifests[0]->GetName());
	BOOST_CHECK_EQUAL("Memory", manifests[1]->GetName());	
	BOOST_CHECK_EQUAL("Sfx", manifests[2]->GetName()); // These last two could
	BOOST_CHECK_EQUAL("Gfx", manifests[3]->GetName()); // be swapped. Doesn't matter.

	// BUG
	// g, m, s, p 
	// m, g, s, p  g depends on m, so m is swapped
	// p, g, s, m  m depends on p, so m is swapped
	// p, g, s, m  g depends on m, BUT, m has already been at index #2! So no dice.
	// 
}


BOOST_AUTO_TEST_SUITE_END()
