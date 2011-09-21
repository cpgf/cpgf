#ifndef __VMMLIB__EXCEPTION__HPP__
#define __VMMLIB__EXCEPTION__HPP__

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cassert>

#include <vmmlib/vmmlib_config.hpp>


#define VMMLIB_HERE ( except_here( __FILE__, __LINE__ ) )

#ifdef VMMLIB_THROW_EXCEPTIONS
#define VMMLIB_ERROR( desc, here ) throw( exception( desc, here ) )
#else
#define VMMLIB_ERROR( desc, here ) error_noexcept( desc, here )
#endif


namespace vmml
{

struct except_here
{
    except_here( const char* file_, int line_ ) : file( file_ ), line( line_ ) {}
    const char*     file;
    int             line;  
}; // struct except_here


inline void
error_noexcept( const std::string& desc, const except_here& here )
{
    std::cerr 
        << "vmmlib error at " << here.file << ":" << here.line << "\n"
        << desc 
        << std::endl;
    assert( 0 );
}



class exception : public std::exception
{
public:
    exception( const std::string& desc, const except_here& here )
    : _description( desc )
    , _here( here )
    {}
    
    virtual ~exception() throw() {}
    
    virtual const char* what() const throw()
    {
        std::stringstream ss;
        ss
            << _here.file << "(" << _here.line << "): - " 
            << _description 
            << std::endl;
        return ss.str().c_str();
    }
    
protected:
    std::string         _description;
    const except_here&  _here;

private:
    // disallow std ctor
    exception() : _here( *new except_here( "", 0 ) ){};
    // disallow assignment operator
    virtual const exception& operator=( const exception& ){ return *this; }


};


} // namespace stream_process

#endif
