#ifndef INI_ERROR_H_INCLUDED
#define INI_ERROR_H_INCLUDED

#include <iostream>
#include <exception>
#include <cstring>

#define INI_ERROR_DEFAULT_STRING "Error in INI file"
#define INI_ERROR_STRING_PREFIX "Error in INI file : "

class ini_error : public std::exception {
	char* m_text;

	public:

	ini_error() {
		m_text = new char[std::strlen(INI_ERROR_DEFAULT_STRING)+1];
		std::strcpy(m_text, INI_ERROR_DEFAULT_STRING);
	}

	ini_error(const char* _s) {
		m_text = new char[std::strlen(INI_ERROR_STRING_PREFIX)+std::strlen(_s)+1];
		std::strcpy(m_text, INI_ERROR_STRING_PREFIX);
		std::strcat(m_text, _s);
	}

	ini_error(std::string& _s) {
		ini_error( _s.c_str() );
	}

	ini_error(const ini_error& _src) {
		m_text = new char[std::strlen(_src.m_text)+1];
		std::strcpy(m_text, _src.m_text);
	}

	~ini_error() throw() {
		delete[] m_text;
	}

	const char* what() const noexcept {
		return m_text;
	}
};

#endif /* INI_ERROR_H_INCLUDED */
