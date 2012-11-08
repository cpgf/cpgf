// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SVERTEXMANIPULATOR_H
#define __META_IRRLICHT_SVERTEXMANIPULATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IVertexManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <typename D>
void buildMetaClass_SVertexColorBrightnessManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (s32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorContrastBrightnessManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, s32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorContrastManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorDesaturateToAverageManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorDesaturateToLightnessManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorDesaturateToLuminanceManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorGammaManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorInterpolateLinearManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::SColor, f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorInterpolateQuadraticManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::SColor, video::SColor, f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorInvertManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorScaleManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorSetAlphaManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (u32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorSetManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::SColor)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexColorThresholdManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (u8, video::SColor, video::SColor)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
}


template <typename D>
void buildMetaClass_SVertexPositionScaleAlongNormalsManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &)>();
}


template <typename D>
void buildMetaClass_SVertexPositionScaleManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &)>();
}


template <typename D>
void buildMetaClass_SVertexPositionTransformManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::matrix4 &)>();
}


template <typename D>
void buildMetaClass_SVertexTCoordsScaleManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector2df &, u32)>()
        ._default(copyVariantFromCopyable(1))
    ;
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex2TCoords &)>(mopHolder(mopHolder));
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
