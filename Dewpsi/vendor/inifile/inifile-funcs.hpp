#ifndef INIFILE_FUNCS_HPP
#define INIFILE_FUNCS_HPP

#include <ini_error.h>
#include <cassert>

#define SYNTAX_ERROR(s) "Syntax error: " s

IniFile::IniFile() : m_sections(), m_cursection(nullptr), m_file(nullptr), m_curline(0)
{
	m_sections.insert(std::make_pair("", KeyValMap()));
	SetToSection("");
}

int IniFile::ReadFile(const std::string& file) {
	char* sNewFile = (char*) std::realloc(m_file, file.length());
	if (! sNewFile)
		throw std::bad_alloc();

	std::strcpy(sNewFile, file.c_str());
	m_file = sNewFile;
	sNewFile = nullptr;

	// clear old data
	m_cursection = nullptr;
	m_sections.clear();

	// open file
	std::ifstream ifs(file);
	if (! ifs.is_open()) {
		std::cerr << "Failed to open " << file << '\n';
		return -1;
	}

	// collect data
	StringMapPair curSection;
	std::string line;
	size_t szStart, szEnd;

	for (; ! ifs.eof(); ++m_curline)
	{
		if (! std::getline(ifs, line))
			break;

		szStart = 0;
		szEnd = line.length();

		RemoveLeadingSpaces(line, szStart, szEnd);

		// skip empty lines
		if (szStart >= szEnd || line[szStart] == ';')
			continue;
		// line defines a section
		else if (line[szStart] == '[')
			ParseSection(curSection, line, szStart, szEnd);
		// line defines a key-value pair
		else
			ParseKeyValuePair(curSection, line, szStart, szEnd);
	}

	// add last section
	m_sections.insert(curSection);
	ifs.close();
	SetToSection("");

	return 0;
}

int IniFile::SetToSection(const std::string& sec) {
	SectionMapItr found = m_sections.find(sec);

	if (found == m_sections.end()) {
	  std::cerr << "Could not find section '" << sec << "'\n";
	  return -1;
	}

	m_cursection = &(found->second);
	return 0;
}

//////////////////////////////////////////private functions/////////////////////////////////////////

void IniFile::ParseSection(StringMapPair& curSection, std::string& line, size_t& beg, size_t& end)
{
	m_sections.insert(curSection); // add previous section
	GetSectionName(line, beg, end);
	curSection.first = line;
	curSection.second.clear();
}

void IniFile::ParseKeyValuePair(StringMapPair& curSection, std::string& line, size_t& beg, size_t& end)
{
	StringPair pair;
	SeparateValueLine(pair, line, beg, end);
	curSection.second.insert(pair);
}

void IniFile::SeparateValueLine(StringPair& pair, const std::string& line, size_t& beg, size_t& end)
{
	size_t szKeyEnd, szValBeg, szCmtIdx;

	szKeyEnd = szValBeg = line.find_first_of('=', beg);
	assert(szKeyEnd != std::string::npos);

#ifdef NDEBUG
	if (szKeyEnd == std::string::npos)
	{
		std::stringstream ss;
		ss << SYNTAX_ERROR("no = found on ") << m_file << ':' << m_curline;
		throw ini_error(ss.str());
	}
#endif

	// remove spaces between key and =
	RemoveTrailingSpaces(line, beg, --szKeyEnd);
	// remove spaces in front of value
	RemoveLeadingSpaces(line, ++szValBeg, end);

	// locate comment
	szCmtIdx = line.find_first_of(';', szValBeg);
	if (szCmtIdx != std::string::npos) {
	  end = szCmtIdx;
	}

	// remove spaces from value to ';' or end
	RemoveTrailingSpaces(line, szValBeg, --end);

	// separate string into key-value pair
	pair.first = line.substr(beg, szKeyEnd - beg + 1);
	pair.second = line.substr(szValBeg, end - szValBeg + 1);
}

void IniFile::GetSectionName(std::string& line, size_t& beg, size_t& end)
{
	// go into the function with beg pointing to the first non-whitespace character
	++beg;

	// remove leading spaces from string
	RemoveLeadingSpaces(line, beg, end);
	end = line.find_first_of(']');
	assert(beg != end && static_cast<size_t>(end) != std::string::npos);

#ifdef NDEBUG
	if (beg == end || end == std::string::npos)
	{
		std::string sError = SYNTAX_ERROR("invalid section name, ") + line.substr(beg);
  	  	throw ini_error(sError);
	}
#endif

	// remove trailing spaces from string
	RemoveTrailingSpaces(line, beg, --end);

	// extract substring from line
	line = line.substr(beg, end - beg + 1);
}

void IniFile::RemoveLeadingSpaces(const std::string& line, size_t& beg, size_t end)
{
	// zero-length string
	if (beg >= end)
	  return;

	// increase index number until a non-whitespace character is found
	while (line[beg] == ' ' || line[beg] == '\t') {
	  if (beg == std::string::npos)
	  	break;

	  ++beg;
	}
}

void IniFile::RemoveTrailingSpaces(const std::string& line, size_t beg, size_t& end)
{
	if (beg >= end)
	  return;

	while (line[end] == ' ' || line[end] == '\t')
	  --end;
}


#endif /* INIFILE_FUNCS_HPP */
