// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IANIMATEDMESHMD3_H
#define __META_IRRLICHT_IANIMATEDMESHMD3_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ianimatedmeshmd3(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<eMD3Models>("eMD3Models")
        ._element("EMD3_HEAD", irr::scene::EMD3_HEAD)
        ._element("EMD3_UPPER", irr::scene::EMD3_UPPER)
        ._element("EMD3_LOWER", irr::scene::EMD3_LOWER)
        ._element("EMD3_WEAPON", irr::scene::EMD3_WEAPON)
        ._element("EMD3_NUMMODELS", irr::scene::EMD3_NUMMODELS)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EMD3_ANIMATION_TYPE>("EMD3_ANIMATION_TYPE")
        ._element("EMD3_BOTH_DEATH_1", irr::scene::EMD3_BOTH_DEATH_1)
        ._element("EMD3_BOTH_DEAD_1", irr::scene::EMD3_BOTH_DEAD_1)
        ._element("EMD3_BOTH_DEATH_2", irr::scene::EMD3_BOTH_DEATH_2)
        ._element("EMD3_BOTH_DEAD_2", irr::scene::EMD3_BOTH_DEAD_2)
        ._element("EMD3_BOTH_DEATH_3", irr::scene::EMD3_BOTH_DEATH_3)
        ._element("EMD3_BOTH_DEAD_3", irr::scene::EMD3_BOTH_DEAD_3)
        ._element("EMD3_TORSO_GESTURE", irr::scene::EMD3_TORSO_GESTURE)
        ._element("EMD3_TORSO_ATTACK_1", irr::scene::EMD3_TORSO_ATTACK_1)
        ._element("EMD3_TORSO_ATTACK_2", irr::scene::EMD3_TORSO_ATTACK_2)
        ._element("EMD3_TORSO_DROP", irr::scene::EMD3_TORSO_DROP)
        ._element("EMD3_TORSO_RAISE", irr::scene::EMD3_TORSO_RAISE)
        ._element("EMD3_TORSO_STAND_1", irr::scene::EMD3_TORSO_STAND_1)
        ._element("EMD3_TORSO_STAND_2", irr::scene::EMD3_TORSO_STAND_2)
        ._element("EMD3_LEGS_WALK_CROUCH", irr::scene::EMD3_LEGS_WALK_CROUCH)
        ._element("EMD3_LEGS_WALK", irr::scene::EMD3_LEGS_WALK)
        ._element("EMD3_LEGS_RUN", irr::scene::EMD3_LEGS_RUN)
        ._element("EMD3_LEGS_BACK", irr::scene::EMD3_LEGS_BACK)
        ._element("EMD3_LEGS_SWIM", irr::scene::EMD3_LEGS_SWIM)
        ._element("EMD3_LEGS_JUMP_1", irr::scene::EMD3_LEGS_JUMP_1)
        ._element("EMD3_LEGS_LAND_1", irr::scene::EMD3_LEGS_LAND_1)
        ._element("EMD3_LEGS_JUMP_2", irr::scene::EMD3_LEGS_JUMP_2)
        ._element("EMD3_LEGS_LAND_2", irr::scene::EMD3_LEGS_LAND_2)
        ._element("EMD3_LEGS_IDLE", irr::scene::EMD3_LEGS_IDLE)
        ._element("EMD3_LEGS_IDLE_CROUCH", irr::scene::EMD3_LEGS_IDLE_CROUCH)
        ._element("EMD3_LEGS_TURN", irr::scene::EMD3_LEGS_TURN)
        ._element("EMD3_ANIMATION_COUNT", irr::scene::EMD3_ANIMATION_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IAnimatedMeshMD3(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setInterpolationShift", &D::ClassType::setInterpolationShift);
    _d.CPGF_MD_TEMPLATE _method("getTagList", &D::ClassType::getTagList);
    _d.CPGF_MD_TEMPLATE _method("getOriginalMesh", &D::ClassType::getOriginalMesh);
}


template <typename D>
void buildMetaClass_SMD3AnimationInfo(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("first", &D::ClassType::first);
    _d.CPGF_MD_TEMPLATE _field("num", &D::ClassType::num);
    _d.CPGF_MD_TEMPLATE _field("looping", &D::ClassType::looping);
    _d.CPGF_MD_TEMPLATE _field("fps", &D::ClassType::fps);
}


template <typename D>
void buildMetaClass_SMD3Face(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("Index", &D::ClassType::Index);
}


template <typename D>
void buildMetaClass_SMD3Header(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("headerID", &D::ClassType::headerID);
    _d.CPGF_MD_TEMPLATE _field("Version", &D::ClassType::Version);
    _d.CPGF_MD_TEMPLATE _field("fileName", &D::ClassType::fileName);
    _d.CPGF_MD_TEMPLATE _field("numFrames", &D::ClassType::numFrames);
    _d.CPGF_MD_TEMPLATE _field("numTags", &D::ClassType::numTags);
    _d.CPGF_MD_TEMPLATE _field("numMeshes", &D::ClassType::numMeshes);
    _d.CPGF_MD_TEMPLATE _field("numMaxSkins", &D::ClassType::numMaxSkins);
    _d.CPGF_MD_TEMPLATE _field("frameStart", &D::ClassType::frameStart);
    _d.CPGF_MD_TEMPLATE _field("tagStart", &D::ClassType::tagStart);
    _d.CPGF_MD_TEMPLATE _field("tagEnd", &D::ClassType::tagEnd);
    _d.CPGF_MD_TEMPLATE _field("fileSize", &D::ClassType::fileSize);
}


template <typename D>
void buildMetaClass_SMD3Mesh(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Name", &D::ClassType::Name);
    _d.CPGF_MD_TEMPLATE _field("Buffer", &D::ClassType::Buffer);
    _d.CPGF_MD_TEMPLATE _field("TagList", &D::ClassType::TagList);
    _d.CPGF_MD_TEMPLATE _field("MD3Header", &D::ClassType::MD3Header);
}


template <typename D>
void buildMetaClass_SMD3MeshBuffer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("MeshHeader", &D::ClassType::MeshHeader);
    _d.CPGF_MD_TEMPLATE _field("Shader", &D::ClassType::Shader);
    _d.CPGF_MD_TEMPLATE _field("Indices", &D::ClassType::Indices);
    _d.CPGF_MD_TEMPLATE _field("Vertices", &D::ClassType::Vertices);
    _d.CPGF_MD_TEMPLATE _field("Tex", &D::ClassType::Tex);
}


template <typename D>
void buildMetaClass_SMD3MeshHeader(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("meshID", &D::ClassType::meshID);
    _d.CPGF_MD_TEMPLATE _field("meshName", &D::ClassType::meshName);
    _d.CPGF_MD_TEMPLATE _field("numFrames", &D::ClassType::numFrames);
    _d.CPGF_MD_TEMPLATE _field("numShader", &D::ClassType::numShader);
    _d.CPGF_MD_TEMPLATE _field("numVertices", &D::ClassType::numVertices);
    _d.CPGF_MD_TEMPLATE _field("numTriangles", &D::ClassType::numTriangles);
    _d.CPGF_MD_TEMPLATE _field("offset_triangles", &D::ClassType::offset_triangles);
    _d.CPGF_MD_TEMPLATE _field("offset_shaders", &D::ClassType::offset_shaders);
    _d.CPGF_MD_TEMPLATE _field("offset_st", &D::ClassType::offset_st);
    _d.CPGF_MD_TEMPLATE _field("vertexStart", &D::ClassType::vertexStart);
    _d.CPGF_MD_TEMPLATE _field("offset_end", &D::ClassType::offset_end);
}


inline bool opErAToRWrapper_SMD3QuaternionTag__opEqual(const SMD3QuaternionTag * self, const SMD3QuaternionTag & other) {
    return (*self) == other;
}
inline SMD3QuaternionTag & opErAToRWrapper_SMD3QuaternionTag__opAssign(SMD3QuaternionTag * self, const SMD3QuaternionTag & copyMe) {
    return (*self) = copyMe;
}


template <typename D>
void buildMetaClass_SMD3QuaternionTag(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SMD3QuaternionTag &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::stringc &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &, const core::vector3df &)>();
    _d.CPGF_MD_TEMPLATE _field("Name", &D::ClassType::Name);
    _d.CPGF_MD_TEMPLATE _field("position", &D::ClassType::position);
    _d.CPGF_MD_TEMPLATE _field("rotation", &D::ClassType::rotation);
    _d.CPGF_MD_TEMPLATE _method("setto", &D::ClassType::setto);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const SMD3QuaternionTag &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const SMD3QuaternionTag *, const SMD3QuaternionTag &))&opErAToRWrapper_SMD3QuaternionTag__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<SMD3QuaternionTag & (*)(cpgf::GMetaSelf, const SMD3QuaternionTag &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (SMD3QuaternionTag & (*) (SMD3QuaternionTag *, const SMD3QuaternionTag &))&opErAToRWrapper_SMD3QuaternionTag__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


inline const SMD3QuaternionTag & opErAToRWrapper_SMD3QuaternionTagList__opArrayGet(const SMD3QuaternionTagList * self, u32 index) {
    return (*self)[index];
}
inline SMD3QuaternionTag & opErAToRWrapper_SMD3QuaternionTagList__opArrayGet(SMD3QuaternionTagList * self, u32 index) {
    return (*self)[index];
}
inline void opErAToRWrapper_SMD3QuaternionTagList__opArraySet(SMD3QuaternionTagList * self, u32 index, const cpgf::RemoveReference<SMD3QuaternionTag & >::Result & OpsEt_vALue) {
    (*self)[index] = OpsEt_vALue;
}
inline SMD3QuaternionTagList & opErAToRWrapper_SMD3QuaternionTagList__opAssign(SMD3QuaternionTagList * self, const SMD3QuaternionTagList & copyMe) {
    return (*self) = copyMe;
}


template <typename D>
void buildMetaClass_SMD3QuaternionTagList(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SMD3QuaternionTagList &)>();
    _d.CPGF_MD_TEMPLATE _method("get", &D::ClassType::get, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("set_used", &D::ClassType::set_used);
    _d.CPGF_MD_TEMPLATE _method("push_back", &D::ClassType::push_back, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _operator<const SMD3QuaternionTag & (*)(const cpgf::GMetaSelf &, u32)>(mopHolder[0], cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (const SMD3QuaternionTag & (*) (const SMD3QuaternionTagList *, u32))&opErAToRWrapper_SMD3QuaternionTagList__opArrayGet, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<SMD3QuaternionTag & (*)(cpgf::GMetaSelf, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (SMD3QuaternionTag & (*) (SMD3QuaternionTagList *, u32))&opErAToRWrapper_SMD3QuaternionTagList__opArrayGet, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _method("_opArraySet", (void (*) (SMD3QuaternionTagList *, u32, const cpgf::RemoveReference<SMD3QuaternionTag & >::Result &))&opErAToRWrapper_SMD3QuaternionTagList__opArraySet, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<SMD3QuaternionTagList & (*)(cpgf::GMetaSelf, const SMD3QuaternionTagList &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (SMD3QuaternionTagList & (*) (SMD3QuaternionTagList *, const SMD3QuaternionTagList &))&opErAToRWrapper_SMD3QuaternionTagList__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


template <typename D>
void buildMetaClass_SMD3TexCoord(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("u", &D::ClassType::u);
    _d.CPGF_MD_TEMPLATE _field("v", &D::ClassType::v);
}


template <typename D>
void buildMetaClass_SMD3Vertex(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("position", &D::ClassType::position);
    _d.CPGF_MD_TEMPLATE _field("normal", &D::ClassType::normal);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
