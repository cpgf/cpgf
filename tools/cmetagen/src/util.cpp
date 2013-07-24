#include "util.h"
#include "project.h"

#include "Poco/String.h"
#include "Poco/RegularExpression.h"
#include "Poco/Path.h"
#include "Poco/File.h"


#include <algorithm>

using namespace std;
using namespace cpgf;

namespace metagen {


std::string normalizeSymbolName(const std::string & name)
{
	string result = name;
	
	Poco::replaceInPlace(result, "::", "_");
	Poco::replaceInPlace(result, "__", "_");
	
	return result;
}

std::string normalizePath(const std::string & path)
{
	std::string result(path);
	std::replace(result.begin(), result.end(), '\\', '/');
	return result;
}

std::string makeRelativePath(const std::string & sbase, const std::string & spath)
{
	Poco::Path base(sbase);
	Poco::Path path(spath);

	if(base.getNode() != path.getNode()) {
		return spath;
	}
	if(base.getDevice() != path.getDevice()) {
		return spath;
	}

	int baseDepth = base.depth();
	int pathDepth = path.depth();
	if(baseDepth == 0 || pathDepth == 0) {
		return spath;
	}
	
	int i = 0;
	while(base[i] == path[i]) {
		++i;
		if(i == baseDepth || i == pathDepth) {
			break;
		}
	}
	
	if(i == baseDepth && i == pathDepth) {
		return "";
	}

	Poco::Path result;
	for(int k = i; k < baseDepth; ++k) {
		result.pushDirectory("..");
	}
	for(int k = i; k < pathDepth; ++k) {
		result.pushDirectory(path[k]);
	}

	return result.toString();
}

bool readStringFromFile(const std::string & fileName, std::string * outContent)
{
	FILE * file = fopen(fileName.c_str(), "rb");
	if(file == NULL) {
		return false;
	}
	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);
	outContent->resize(length);
	fread(&(*outContent)[0], length, 1, file);
	fclose(file);
	return true;
}

bool writeStringToFile(const std::string & fileName, const std::string & content)
{
	FILE * file = fopen(fileName.c_str(), "wb");
	if(file == NULL) {
		return false;
	}
	fwrite(&content[0], content.size(), 1, file);
	fclose(file);
	return true;
}

bool shouldTargetFileBeUpdated(const std::string & sourceFileName, const std::string & targetFileName)
{
	Poco::File targetFile(targetFileName);

	if(! targetFile.exists()) {
		return true;
	}

	Poco::File sourceFile(sourceFileName);
	
	return sourceFile.getLastModified() >= targetFile.getLastModified();
}

bool isFileContentSameToString(const std::string & fileName, const std::string & s)
{
	string content;
	readStringFromFile(fileName, &content);
	return s == content;
}



} // namespace metagen
