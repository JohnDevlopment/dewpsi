#include "Dewpsi_Except.h"
#include "Dewpsi_String.h"

static constexpr Dewpsi::StaticString _Error = "Dewpsi engine aborted";

namespace Dewpsi {

#define PD_SRC_LENGTH(s) String::Length(src.s) + 1

DewpsiError::DewpsiError() : m_cpWhat(new char[_Error.size()])
{
    Dewpsi::String::Copy(m_cpWhat, _Error.get(), _Error.size() + 1);
}

DewpsiError::DewpsiError(const DewpsiError& src)
    : m_cpWhat(String::New(PD_SRC_LENGTH(m_cpWhat)))
{
    if (! m_cpWhat)
        throw std::bad_alloc();
    
    // copy source string
    String::Copy(m_cpWhat, src.m_cpWhat);
}

DewpsiError::~DewpsiError()
{
    String::Delete(m_cpWhat);
    m_cpWhat = nullptr;
}

DewpsiError::DewpsiError(const std::string& src)
    : m_cpWhat(String::New(PD_SRC_LENGTH(c_str())))
{
    if (! m_cpWhat)
        throw std::bad_alloc();
    
    // copy source string
    String::Copy(m_cpWhat, src.c_str());
}

const char* DewpsiError::what() const noexcept
{
    return m_cpWhat;
}

#undef PD_SRC_LENGTH

}

