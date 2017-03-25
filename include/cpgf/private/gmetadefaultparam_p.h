#ifndef CPGF_GMETADEFAULTPARAM_P_H
#define CPGF_GMETADEFAULTPARAM_P_H

#include "cpgf/gscopedptr.h"

#include <vector>

namespace cpgf {

namespace meta_internal {


class GMetaDefaultParamList
{
private:
	typedef std::vector<GVariant> DefaultValueList;

public:
	~GMetaDefaultParamList();

	void addDefault(const GVariant & v);
	const GVariant & getDefault(size_t index);
	size_t getDefaultCount() const;

	size_t loadDefaultParams(
		const GVariant ** paramBuffer,
		size_t passedParamCount,
		size_t prototypeParamCount
	);

	size_t loadDefaultParamsByData(
		const GVariantData ** paramDataBuffer,
		size_t passedParamCount,
		size_t prototypeParamCount
	);

private:
	DefaultValueList defaultValueList;
};


} // namespace meta_internal

} // namespace cpgf


#endif
