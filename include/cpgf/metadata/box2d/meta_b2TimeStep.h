//auto generated file, don't modify.
#ifndef __META_B2TIMESTEP_H
#define __META_B2TIMESTEP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_B2TimeStep(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("dt", _r), &D::ClassType::dt, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("inv_dt", _r), &D::ClassType::inv_dt, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("dtRatio", _r), &D::ClassType::dtRatio, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("velocityIterations", _r), &D::ClassType::velocityIterations, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("positionIterations", _r), &D::ClassType::positionIterations, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("warmStarting", _r), &D::ClassType::warmStarting, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2TimeStep(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_B2TimeStep(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif