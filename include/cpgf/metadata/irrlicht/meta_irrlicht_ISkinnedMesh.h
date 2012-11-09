// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISKINNEDMESH_H
#define __META_IRRLICHT_ISKINNEDMESH_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iskinnedmesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_INTERPOLATION_MODE>("E_INTERPOLATION_MODE")
        ._element("EIM_CONSTANT", irr::scene::EIM_CONSTANT)
        ._element("EIM_LINEAR", irr::scene::EIM_LINEAR)
        ._element("EIM_COUNT", irr::scene::EIM_COUNT)
    ;
}


template <typename D>
void buildMetaClass_ISkinnedMesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getJointCount", &D::ClassType::getJointCount);
    _d.CPGF_MD_TEMPLATE _method("getJointName", &D::ClassType::getJointName);
    _d.CPGF_MD_TEMPLATE _method("getJointNumber", &D::ClassType::getJointNumber);
    _d.CPGF_MD_TEMPLATE _method("useAnimationFrom", &D::ClassType::useAnimationFrom);
    _d.CPGF_MD_TEMPLATE _method("updateNormalsWhenAnimating", &D::ClassType::updateNormalsWhenAnimating);
    _d.CPGF_MD_TEMPLATE _method("setInterpolationMode", &D::ClassType::setInterpolationMode);
    _d.CPGF_MD_TEMPLATE _method("animateMesh", &D::ClassType::animateMesh);
    _d.CPGF_MD_TEMPLATE _method("skinMesh", &D::ClassType::skinMesh);
    _d.CPGF_MD_TEMPLATE _method("convertMeshToTangents", &D::ClassType::convertMeshToTangents);
    _d.CPGF_MD_TEMPLATE _method("setHardwareSkinning", &D::ClassType::setHardwareSkinning);
    _d.CPGF_MD_TEMPLATE _method("getMeshBuffers", &D::ClassType::getMeshBuffers);
    _d.CPGF_MD_TEMPLATE _method("getAllJoints", (core::array<ISkinnedMesh::SJoint*>& (D::ClassType::*) ())&D::ClassType::getAllJoints);
    _d.CPGF_MD_TEMPLATE _method("getAllJoints", (const core::array<ISkinnedMesh::SJoint*>& (D::ClassType::*) () const)&D::ClassType::getAllJoints);
    _d.CPGF_MD_TEMPLATE _method("finalize", &D::ClassType::finalize);
    _d.CPGF_MD_TEMPLATE _method("addMeshBuffer", &D::ClassType::addMeshBuffer);
    _d.CPGF_MD_TEMPLATE _method("addJoint", &D::ClassType::addJoint)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addWeight", &D::ClassType::addWeight);
    _d.CPGF_MD_TEMPLATE _method("addPositionKey", &D::ClassType::addPositionKey);
    _d.CPGF_MD_TEMPLATE _method("addScaleKey", &D::ClassType::addScaleKey);
    _d.CPGF_MD_TEMPLATE _method("addRotationKey", &D::ClassType::addRotationKey);
    _d.CPGF_MD_TEMPLATE _method("isStatic", &D::ClassType::isStatic);
    {
        GDefineMetaClass<ISkinnedMesh::SJoint> _nd = GDefineMetaClass<ISkinnedMesh::SJoint>::declare("SJoint");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _field("Name", &irr::scene::ISkinnedMesh::SJoint::Name);
        _nd.CPGF_MD_TEMPLATE _field("LocalMatrix", &irr::scene::ISkinnedMesh::SJoint::LocalMatrix);
        _nd.CPGF_MD_TEMPLATE _field("Children", &irr::scene::ISkinnedMesh::SJoint::Children);
        _nd.CPGF_MD_TEMPLATE _field("AttachedMeshes", &irr::scene::ISkinnedMesh::SJoint::AttachedMeshes);
        _nd.CPGF_MD_TEMPLATE _field("PositionKeys", &irr::scene::ISkinnedMesh::SJoint::PositionKeys);
        _nd.CPGF_MD_TEMPLATE _field("ScaleKeys", &irr::scene::ISkinnedMesh::SJoint::ScaleKeys);
        _nd.CPGF_MD_TEMPLATE _field("RotationKeys", &irr::scene::ISkinnedMesh::SJoint::RotationKeys);
        _nd.CPGF_MD_TEMPLATE _field("Weights", &irr::scene::ISkinnedMesh::SJoint::Weights);
        _nd.CPGF_MD_TEMPLATE _field("GlobalMatrix", &irr::scene::ISkinnedMesh::SJoint::GlobalMatrix);
        _nd.CPGF_MD_TEMPLATE _field("GlobalAnimatedMatrix", &irr::scene::ISkinnedMesh::SJoint::GlobalAnimatedMatrix);
        _nd.CPGF_MD_TEMPLATE _field("LocalAnimatedMatrix", &irr::scene::ISkinnedMesh::SJoint::LocalAnimatedMatrix);
        _nd.CPGF_MD_TEMPLATE _field("Animatedposition", &irr::scene::ISkinnedMesh::SJoint::Animatedposition);
        _nd.CPGF_MD_TEMPLATE _field("Animatedscale", &irr::scene::ISkinnedMesh::SJoint::Animatedscale);
        _nd.CPGF_MD_TEMPLATE _field("Animatedrotation", &irr::scene::ISkinnedMesh::SJoint::Animatedrotation);
        _nd.CPGF_MD_TEMPLATE _field("GlobalInversedMatrix", &irr::scene::ISkinnedMesh::SJoint::GlobalInversedMatrix);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<ISkinnedMesh::SPositionKey> _nd = GDefineMetaClass<ISkinnedMesh::SPositionKey>::declare("SPositionKey");
        _nd.CPGF_MD_TEMPLATE _field("frame", &irr::scene::ISkinnedMesh::SPositionKey::frame);
        _nd.CPGF_MD_TEMPLATE _field("position", &irr::scene::ISkinnedMesh::SPositionKey::position);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<ISkinnedMesh::SRotationKey> _nd = GDefineMetaClass<ISkinnedMesh::SRotationKey>::declare("SRotationKey");
        _nd.CPGF_MD_TEMPLATE _field("frame", &irr::scene::ISkinnedMesh::SRotationKey::frame);
        _nd.CPGF_MD_TEMPLATE _field("rotation", &irr::scene::ISkinnedMesh::SRotationKey::rotation);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<ISkinnedMesh::SScaleKey> _nd = GDefineMetaClass<ISkinnedMesh::SScaleKey>::declare("SScaleKey");
        _nd.CPGF_MD_TEMPLATE _field("frame", &irr::scene::ISkinnedMesh::SScaleKey::frame);
        _nd.CPGF_MD_TEMPLATE _field("scale", &irr::scene::ISkinnedMesh::SScaleKey::scale);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<ISkinnedMesh::SWeight> _nd = GDefineMetaClass<ISkinnedMesh::SWeight>::declare("SWeight");
        _nd.CPGF_MD_TEMPLATE _field("buffer_id", &irr::scene::ISkinnedMesh::SWeight::buffer_id);
        _nd.CPGF_MD_TEMPLATE _field("vertex_id", &irr::scene::ISkinnedMesh::SWeight::vertex_id);
        _nd.CPGF_MD_TEMPLATE _field("strength", &irr::scene::ISkinnedMesh::SWeight::strength);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
