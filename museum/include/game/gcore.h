#ifndef __GCORE_H
#define __GCORE_H

#include "cpgf/gcompiler.h"


typedef long long			gint64;
typedef unsigned long long	guint64;
typedef int					gint32;
typedef unsigned int		guint32;
typedef short int			gint16;
typedef unsigned short int	guint16;
typedef signed char			gint8;
typedef unsigned char		guint8;
typedef char				gchar;
typedef unsigned char		gbyte;
typedef int					gint;
typedef unsigned int		guint;
typedef float				gfloat;
typedef double				gdouble;
typedef float				greal32;
typedef double				greal64;
typedef float				gcoord; // coordinate
typedef guint32				gcolor;
typedef gfloat				gradian;
typedef guint32				gmillisecond;

const int gIntMin = -2147483647 - 1;
const int gIntMax = 2147483647;
const int gInt32Min = -2147483647 - 1;
const int gInt32Max = 2147483647;


#ifndef NULL
	#define NULL 0
#endif

namespace cpgf {

#ifdef G_DEBUG
	const bool IsDebug = true;
#else
	const bool IsDebug = false;
#endif

struct IGInterface
{
	// force for vtable
	virtual ~IGInterface() {}
};

class GObject
{
public:

	GObject() {
	}

	virtual ~GObject() {
	}
};



template <typename ToPointerType, typename FromPointerType>
inline ToPointerType gSafeCast(FromPointerType p)
{
#ifdef G_DEBUG
	return dynamic_cast<ToPointerType>(p);
#else
	return static_cast<ToPointerType>(p);
#endif
}

#define NonCopyableClass(className) \
	private: \
		className(const className &); \
		className & operator = (const className &);


} // namespace cpgf



#endif

