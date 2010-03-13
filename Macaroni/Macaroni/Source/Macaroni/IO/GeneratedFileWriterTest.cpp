#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE GeneratedFileWriterTests
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/convenience.hpp> 
#include <Macaroni/IO/GeneratedFileWriter.h>
#include <boost/filesystem/convenience.hpp>
#include <Macaroni/Exception.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <sstream>

using boost::filesystem::create_directories;
using Macaroni::Exception;
using std::ofstream;
using std::ifstream;
using std::ostream;
using boost::filesystem::path;
using boost::shared_ptr;
using std::string;
using std::stringstream;

using Macaroni::IO::GeneratedFileWriter;

BOOST_AUTO_TEST_SUITE(GeneratedFileWriterSuite)

static void createFile(path filePath, bool happiness)
{
	ofstream file(filePath.native_file_string().c_str());
	file << "Hello" << std::endl <<
		(happiness ? ":)" : ":(") << std::endl <<
		"Do not delete or modify this file; it is used to as part of a unit test.";
	file.close();

}
BOOST_AUTO_TEST_CASE(when_comparing_buffer_to_identical_file)
{	
	createFile("TestFile.txt", true);
	std::stringstream ss;
	boost::filesystem::path filePath("TestFile.txt");
	ss << "Hello" << std::endl <<
		":)" << std::endl <<
		"Do not delete or modify this file; it is used to as part of a unit test.";

	bool result = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss);
	BOOST_CHECK_MESSAGE(!result, "The files should be identical! How dare it!");
}

BOOST_AUTO_TEST_CASE(when_comparing_buffer_to_different_file)
{	
	createFile("TestFile.txt", true);
	std::stringstream ss;
	boost::filesystem::path filePath("TestFile.txt");
	ss << "Hello" << std::endl <<
		":(" << std::endl <<
		"Do not delete or modify this file; it is used to as part of a unit test.";

	bool result = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss);
	BOOST_CHECK_MESSAGE(result, "The files should not be identical! Unlike the "
		"file, the buffer is unhappy.");
}

BOOST_AUTO_TEST_CASE(when_overwriting_file_then_comparing)
{	
	// First check file, it should be different.
	createFile("TestFile.txt", true);

	std::stringstream ss;
	boost::filesystem::path filePath("TestFile.txt");
	ss << "Hello" << std::endl <<
		":(" << std::endl <<
		"Do not delete or modify this file; it is used to as part of a unit test.";

	bool result = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss);
	BOOST_CHECK_MESSAGE(result, "The files should not be identical! Unlike the "
		"file, the buffer is unhappy.");

	// Next overwrite file.	
	GeneratedFileWriter::overwriteFileWithBufferIfDifferent(filePath, ss);
	
	// Now the file should be the same.
	bool result2 = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss);
	BOOST_CHECK_MESSAGE(!result2, "The files should now be identical! We just changed them.");
}

BOOST_AUTO_TEST_SUITE_END()
