#ifndef INIFILE_HPP
#define INIFILE_HPP

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ini_error.h>

class IniFile {
public:
	IniFile();
	~IniFile()
	{
		delete[] m_file;
		m_file = nullptr;
	}

	// read an INI file and parse its data
	int ReadFile(const std::string& file);

	// set INI file to the selected section
	int SetToSection(const std::string& sec);

	// retrieve a value from the current section
	template<typename T>
	int GetValue(const std::string& key, T& value);

private:
	// key-value map
	typedef std::map<std::string, std::string> KeyValMap;

	// map of sections linked by string names
	typedef std::map<std::string, KeyValMap> SectionMap;

	// pair of string and a map
	typedef std::pair<std::string, KeyValMap> StringMapPair;

	// string pair
	typedef std::pair<std::string, std::string> StringPair;

	// constant iterator to a map of string and string
	typedef KeyValMap::const_iterator KeyValMapCItr;

	// iterator to the map of sections
	typedef SectionMap::iterator SectionMapItr;

	SectionMap m_sections;
	KeyValMap* m_cursection;
	char* m_file;
	int m_curline;

	// parse sections of the file
	void ParseSection(StringMapPair& curSection, std::string& line, size_t& beg, size_t& end);
	void ParseKeyValuePair(StringMapPair& curSection, std::string& line, size_t& beg, size_t& end);

	// separate string into key-value pair
	void SeparateValueLine(StringPair& pair, const std::string& line, size_t& beg, size_t& end);

	// extract a substring from between "[]" and set the two endpoints of it
	void GetSectionName(std::string& line, size_t& beg, size_t& end);

	// set endpoints of a string where the whitespaces are cut out
	void RemoveLeadingSpaces(const std::string& line, size_t& beg, size_t end);
	void RemoveTrailingSpaces(const std::string& line, size_t beg, size_t& end);
};

template<typename T>
int IniFile::GetValue(const std::string& key, T& value) {
	KeyValMapCItr citr = m_cursection->find(key);
	if (citr == m_cursection->cend()) {
	  std::cerr << "Failed to locate key\n";
	  return -1;
	}

	std::stringstream ss(citr->second);
	ss >> value;

	return 0;
}

template<>
inline int IniFile::GetValue(const std::string& key, std::string& value)
{
	KeyValMapCItr citr = m_cursection->find(key);
	if (citr == m_cursection->cend()) {
	  std::cerr << "Failed to locate key\n";
	  return -1;
	}

	value = citr->second;

	return 0;
}

#ifdef INIFILE_FUNCTIONS
#undef INIFILE_FUNCTIONS
#include <inifile-funcs.hpp>
#endif

#endif /* INIFILE_HPP */
