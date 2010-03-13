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

BOOST_AUTO_TEST_CASE(when_using_a_GeneratedFileWriter_to_create_files_then_comparing)
{	
	const boost::filesystem::path filePath("TestFile.txt");

	// First check file, it should be different.
	GeneratedFileWriter file(filePath);
	file.WriteLine("GeneratedFileWriter was used for this!");
	file.Write("^_");
	file.Write("_^");
	bool closeThinksFileWasChanged = file.Close();

	std::stringstream ss;	
	ss << "GeneratedFileWriter was used for this!" << std::endl <<
		"^__^";
	bool result = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss);

	BOOST_CHECK_MESSAGE(closeThinksFileWasChanged, "Close should have returned true the first time.");
	BOOST_CHECK_MESSAGE(!result, "The GeneratedFile should compare equally to the stream. (^__^)");

	GeneratedFileWriter file2(filePath);
	file2.WriteLine("GeneratedFileWriter was used for this!");
	file2.Write("*o*");
	bool closeThinksFileWasChanged2 = file2.Close();

	bool result2 = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss);
	BOOST_CHECK_MESSAGE(closeThinksFileWasChanged, "Close should have returned true the second time.");
	BOOST_CHECK_MESSAGE(result2, "The GeneratedFile should have written a file which was different (*o*).");

	GeneratedFileWriter file3(filePath);
	file3.WriteLine("GeneratedFileWriter was used for this!");
	file3.Write("*o*");
	bool closeThinksFileWasChanged3 = file3.Close();

	std::stringstream ss2;	
	ss2 << "GeneratedFileWriter was used for this!" << std::endl <<
		"*o*";

	bool result3 = GeneratedFileWriter::isFileDifferentThanBuffer(filePath, ss2);
	BOOST_CHECK_MESSAGE(!closeThinksFileWasChanged3, "Close should have returned false the third time.");
	BOOST_CHECK_MESSAGE(!result3, "The GeneratedFile should have not written anything the third time.");
}


BOOST_AUTO_TEST_CASE(when_using_a_GeneratedFileWriter_to_write_over_an_empty_file)
{	
	const boost::filesystem::path filePath("TestFile.txt");

	ofstream file(filePath.native_file_string().c_str());
	file.close();

	// This is to catch a case where the GeneratedFileWriter isn't overwritting 
	// an empty file.
	// First check file, it should be different.
	GeneratedFileWriter fileWriter(filePath);
	fileWriter.Write("SOMETHING NEW!!");
	bool closeThinksFileWasChanged = fileWriter.Close();

	BOOST_CHECK_MESSAGE(closeThinksFileWasChanged, "Should overwrite empty file.");
}

BOOST_AUTO_TEST_SUITE_END()
