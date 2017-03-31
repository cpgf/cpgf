#ifndef GSCRIPTCOMMON_H
#define GSCRIPTCOMMON_H

namespace cpgf {

namespace bind_internal {

enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile,

	opcvCount
};


} //namespace bind_internal

} //namespace cpgf


#endif
