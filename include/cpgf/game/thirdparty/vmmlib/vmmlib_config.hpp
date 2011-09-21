#ifndef __VMML__VMMLIB_CONFIG__HPP__
#define __VMML__VMMLIB_CONFIG__HPP__

// #define VMMLIB_NO_SFINAE 



// enabling this switch will have the following effect:
// operator T* will not be compiled, but for vectors, 
// operator[] will instead be used. This means you can 
// use vec[2] as before, but things like glVertex3fv( vec )
// will not work anymore.
//#define VMMLIB_NO_CONVERSION_OPERATORS

#ifndef VMMLIB_CUSTOM_CONFIG
#  ifndef NDEBUG
#    define VMMLIB_SAFE_ACCESSORS
#  endif
#  define VMMLIB_THROW_EXCEPTIONS
#  ifdef VMMLIB_DONT_FORCE_ALIGNMENT
#    define VMMLIB_ALIGN( var ) var
#  else
#    ifdef __GNUC__
#      define VMMLIB_ALIGN( var ) var __attribute__((aligned(4)))
#    elif defined WIN32
#      define VMMLIB_ALIGN( var ) __declspec (align (4)) var
#    else
#      error "Alignment macro undefined"
#    endif
#  endif
#else   // we define VMMLIB_ALIGN in case it's not defined in the custom config
#   ifndef VMMLIB_ALIGN
#       define VMMLIB_ALIGN( var ) var
#   endif
#endif

#endif

