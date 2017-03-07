#ifndef CPGF_GINTERFACE_H
#define CPGF_GINTERFACE_H

#include "cpgf/gcompiler.h"

#include <cstdint>

namespace cpgf {

typedef int32_t gapi_bool;

struct IObject
{
   virtual uint32_t G_API_CC addReference() = 0;
   virtual uint32_t G_API_CC releaseReference() = 0;
};


} // namespace cpgf


#endif

