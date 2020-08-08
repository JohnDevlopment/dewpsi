#ifndef DEWPSI_EXCEPT_H
#define DEWPSI_EXCEPT_H

/**
*   @file       Dewpsi_Except.h
*   @brief      @doxfb
*   
*   @defgroup   exceptions Exceptions
*   Dewpsi exception classes.
*   @ingroup core
*/

#include <Dewpsi_Core.h>
#include <exception>

namespace Dewpsi {
    /** A generic "Dewpsi error" exception class.
    *   @ingroup exceptions
    */
    class DewpsiError : public std::exception {
    public:
        /// Initializes DewpsiError to a default string.
        DewpsiError();
        
        /// Copy constructor
        DewpsiError(const DewpsiError& src);
        
        /// Initialize DewpsiError with the string pointed to by @a src.
        explicit DewpsiError(const std::string& src);
        
        /// Destructor
        ~DewpsiError();
        
        /// Returns the string containing the exception.
        const char* what() const noexcept;
    
    private:
        char* m_cpWhat;
    };
}

#endif /* DEWPSI_EXCEPT_H */
