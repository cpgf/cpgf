#ifndef __GINTERFACE_H
#define __GINTERFACE_H

#include "cpgf/gcompiler.h"
#include "cpgf/gstdint.h"


namespace cpgf {

typedef int32_t gapi_bool;

struct IObject
{
   virtual uint32_t G_API_CC unused_queryInterface(void *, void *) = 0;
   virtual uint32_t G_API_CC addReference() = 0;
   virtual uint32_t G_API_CC releaseReference() = 0;
};


} // namespace cpgf


#endif

