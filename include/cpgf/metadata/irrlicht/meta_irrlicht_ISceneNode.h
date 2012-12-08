// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENENODE_H
#define __META_IRRLICHT_ISCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ISceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnRegisterSceneNode", &D::ClassType::OnRegisterSceneNode);
    _d.CPGF_MD_TEMPLATE _method("OnAnimate", &D::ClassType::OnAnimate);
    _d.CPGF_MD_TEMPLATE _method("render", &D::ClassType::render);
    _d.CPGF_MD_TEMPLATE _method("getName", &D::ClassType::getName);
    _d.CPGF_MD_TEMPLATE _method("setName", (void (D::ClassType::*) (const c8 *))&D::ClassType::setName);
    _d.CPGF_MD_TEMPLATE _method("setName", (void (D::ClassType::*) (const core::stringc &))&D::ClassType::setName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getTransformedBoundingBox", &D::ClassType::getTransformedBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getAbsoluteTransformation", &D::ClassType::getAbsoluteTransformation);
    _d.CPGF_MD_TEMPLATE _method("getRelativeTransformation", &D::ClassType::getRelativeTransformation);
    _d.CPGF_MD_TEMPLATE _method("isVisible", &D::ClassType::isVisible);
    _d.CPGF_MD_TEMPLATE _method("isTrulyVisible", &D::ClassType::isTrulyVisible);
    _d.CPGF_MD_TEMPLATE _method("setVisible", &D::ClassType::setVisible);
    _d.CPGF_MD_TEMPLATE _method("getID", &D::ClassType::getID);
    _d.CPGF_MD_TEMPLATE _method("setID", &D::ClassType::setID);
    _d.CPGF_MD_TEMPLATE _method("addChild", &D::ClassType::addChild);
    _d.CPGF_MD_TEMPLATE _method("removeChild", &D::ClassType::removeChild);
    _d.CPGF_MD_TEMPLATE _method("removeAll", &D::ClassType::removeAll);
    _d.CPGF_MD_TEMPLATE _method("remove", &D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("addAnimator", &D::ClassType::addAnimator);
    _d.CPGF_MD_TEMPLATE _method("getAnimators", &D::ClassType::getAnimators);
    _d.CPGF_MD_TEMPLATE _method("removeAnimator", &D::ClassType::removeAnimator);
    _d.CPGF_MD_TEMPLATE _method("removeAnimators", &D::ClassType::removeAnimators);
    _d.CPGF_MD_TEMPLATE _method("getMaterial", &D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getMaterialCount", &D::ClassType::getMaterialCount);
    _d.CPGF_MD_TEMPLATE _method("setMaterialFlag", &D::ClassType::setMaterialFlag);
    _d.CPGF_MD_TEMPLATE _method("setMaterialTexture", &D::ClassType::setMaterialTexture);
    _d.CPGF_MD_TEMPLATE _method("setMaterialType", &D::ClassType::setMaterialType);
    _d.CPGF_MD_TEMPLATE _method("getScale", &D::ClassType::getScale);
    _d.CPGF_MD_TEMPLATE _method("setScale", &D::ClassType::setScale);
    _d.CPGF_MD_TEMPLATE _method("getRotation", &D::ClassType::getRotation);
    _d.CPGF_MD_TEMPLATE _method("setRotation", &D::ClassType::setRotation);
    _d.CPGF_MD_TEMPLATE _method("getPosition", &D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", &D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("getAbsolutePosition", &D::ClassType::getAbsolutePosition);
    _d.CPGF_MD_TEMPLATE _method("setAutomaticCulling", &D::ClassType::setAutomaticCulling);
    _d.CPGF_MD_TEMPLATE _method("getAutomaticCulling", &D::ClassType::getAutomaticCulling);
    _d.CPGF_MD_TEMPLATE _method("setDebugDataVisible", &D::ClassType::setDebugDataVisible);
    _d.CPGF_MD_TEMPLATE _method("isDebugDataVisible", &D::ClassType::isDebugDataVisible);
    _d.CPGF_MD_TEMPLATE _method("setIsDebugObject", &D::ClassType::setIsDebugObject);
    _d.CPGF_MD_TEMPLATE _method("isDebugObject", &D::ClassType::isDebugObject);
    _d.CPGF_MD_TEMPLATE _method("getChildren", &D::ClassType::getChildren);
    _d.CPGF_MD_TEMPLATE _method("setParent", &D::ClassType::setParent);
    _d.CPGF_MD_TEMPLATE _method("getTriangleSelector", &D::ClassType::getTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("setTriangleSelector", &D::ClassType::setTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("updateAbsolutePosition", &D::ClassType::updateAbsolutePosition);
    _d.CPGF_MD_TEMPLATE _method("getParent", &D::ClassType::getParent);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("serializeAttributes", &D::ClassType::serializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("deserializeAttributes", &D::ClassType::deserializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("clone", &D::ClassType::clone)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneManager", &D::ClassType::getSceneManager);
}


class ISceneNodeWrapper : public irr::scene::ISceneNode, public cpgf::GScriptWrapper {
public:
    
    ISceneNodeWrapper(ISceneNode * parent, ISceneManager * mgr, s32 id = -1, const core::vector3df & position = core::vector3df(0, 0, 0), const core::vector3df & rotation = core::vector3df(0, 0, 0), const core::vector3df & scale = core::vector3df(1.0f, 1.0f, 1.0f))
        : irr::scene::ISceneNode(parent, mgr, id, position, rotation, scale) {}
    
    void OnRegisterSceneNode()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnRegisterSceneNode"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        irr::scene::ISceneNode::OnRegisterSceneNode();
    }
    void super_OnRegisterSceneNode()
    {
        irr::scene::ISceneNode::OnRegisterSceneNode();
    }
    
    void OnAnimate(u32 timeMs)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnAnimate"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, timeMs);
            return;
        }
        irr::scene::ISceneNode::OnAnimate(timeMs);
    }
    void super_OnAnimate(u32 timeMs)
    {
        irr::scene::ISceneNode::OnAnimate(timeMs);
    }
    
    void render()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("render"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        throw "Abstract method";
    }
    void super_render()
    {
        throw "Abstract method";
    }
    
    const c8 * getName() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getName"));
        if(func)
        {
            return cpgf::fromVariant<const c8 * >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getName();
    }
    const c8 * super_getName() const
    {
        return irr::scene::ISceneNode::getName();
    }
    
    void setName(const c8 * name)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setName"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, name);
            return;
        }
        irr::scene::ISceneNode::setName(name);
    }
    void super_setName(const c8 * name)
    {
        irr::scene::ISceneNode::setName(name);
    }
    
    void setName(const core::stringc & name)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setName"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, name);
            return;
        }
        irr::scene::ISceneNode::setName(name);
    }
    void super_setName(const core::stringc & name)
    {
        irr::scene::ISceneNode::setName(name);
    }
    
    const core::aabbox3d< f32 > & getBoundingBox() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getBoundingBox"));
        if(func)
        {
            return cpgf::fromVariant<const core::aabbox3d< f32 > & >(cpgf::invokeScriptFunction(func.get(), this));
        }
        throw "Abstract method";
    }
    const core::aabbox3d< f32 > & super_getBoundingBox() const
    {
        throw "Abstract method";
    }
    
    const core::aabbox3d< f32 > getTransformedBoundingBox() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getTransformedBoundingBox"));
        if(func)
        {
            return cpgf::fromVariant<const core::aabbox3d< f32 > >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getTransformedBoundingBox();
    }
    const core::aabbox3d< f32 > super_getTransformedBoundingBox() const
    {
        return irr::scene::ISceneNode::getTransformedBoundingBox();
    }
    
    const core::matrix4 & getAbsoluteTransformation() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getAbsoluteTransformation"));
        if(func)
        {
            return cpgf::fromVariant<const core::matrix4 & >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getAbsoluteTransformation();
    }
    const core::matrix4 & super_getAbsoluteTransformation() const
    {
        return irr::scene::ISceneNode::getAbsoluteTransformation();
    }
    
    core::matrix4 getRelativeTransformation() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getRelativeTransformation"));
        if(func)
        {
            return cpgf::fromVariant<core::matrix4 >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getRelativeTransformation();
    }
    core::matrix4 super_getRelativeTransformation() const
    {
        return irr::scene::ISceneNode::getRelativeTransformation();
    }
    
    bool isVisible() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("isVisible"));
        if(func)
        {
            return cpgf::fromVariant<bool >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::isVisible();
    }
    bool super_isVisible() const
    {
        return irr::scene::ISceneNode::isVisible();
    }
    
    bool isTrulyVisible() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("isTrulyVisible"));
        if(func)
        {
            return cpgf::fromVariant<bool >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::isTrulyVisible();
    }
    bool super_isTrulyVisible() const
    {
        return irr::scene::ISceneNode::isTrulyVisible();
    }
    
    void setVisible(bool isVisible)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setVisible"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, isVisible);
            return;
        }
        irr::scene::ISceneNode::setVisible(isVisible);
    }
    void super_setVisible(bool isVisible)
    {
        irr::scene::ISceneNode::setVisible(isVisible);
    }
    
    s32 getID() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getID"));
        if(func)
        {
            return cpgf::fromVariant<s32 >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getID();
    }
    s32 super_getID() const
    {
        return irr::scene::ISceneNode::getID();
    }
    
    void setID(s32 id)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setID"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, id);
            return;
        }
        irr::scene::ISceneNode::setID(id);
    }
    void super_setID(s32 id)
    {
        irr::scene::ISceneNode::setID(id);
    }
    
    void addChild(ISceneNode * child)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("addChild"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, child);
            return;
        }
        irr::scene::ISceneNode::addChild(child);
    }
    void super_addChild(ISceneNode * child)
    {
        irr::scene::ISceneNode::addChild(child);
    }
    
    bool removeChild(ISceneNode * child)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("removeChild"));
        if(func)
        {
            return cpgf::fromVariant<bool >(cpgf::invokeScriptFunction(func.get(), this, child));
        }
        return irr::scene::ISceneNode::removeChild(child);
    }
    bool super_removeChild(ISceneNode * child)
    {
        return irr::scene::ISceneNode::removeChild(child);
    }
    
    void removeAll()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("removeAll"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        irr::scene::ISceneNode::removeAll();
    }
    void super_removeAll()
    {
        irr::scene::ISceneNode::removeAll();
    }
    
    void remove()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("remove"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        irr::scene::ISceneNode::remove();
    }
    void super_remove()
    {
        irr::scene::ISceneNode::remove();
    }
    
    void addAnimator(ISceneNodeAnimator * animator)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("addAnimator"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, animator);
            return;
        }
        irr::scene::ISceneNode::addAnimator(animator);
    }
    void super_addAnimator(ISceneNodeAnimator * animator)
    {
        irr::scene::ISceneNode::addAnimator(animator);
    }
    
    void removeAnimator(ISceneNodeAnimator * animator)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("removeAnimator"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, animator);
            return;
        }
        irr::scene::ISceneNode::removeAnimator(animator);
    }
    void super_removeAnimator(ISceneNodeAnimator * animator)
    {
        irr::scene::ISceneNode::removeAnimator(animator);
    }
    
    void removeAnimators()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("removeAnimators"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        irr::scene::ISceneNode::removeAnimators();
    }
    void super_removeAnimators()
    {
        irr::scene::ISceneNode::removeAnimators();
    }
    
    video::SMaterial & getMaterial(u32 num)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getMaterial"));
        if(func)
        {
            return cpgf::fromVariant<video::SMaterial & >(cpgf::invokeScriptFunction(func.get(), this, num));
        }
        return irr::scene::ISceneNode::getMaterial(num);
    }
    video::SMaterial & super_getMaterial(u32 num)
    {
        return irr::scene::ISceneNode::getMaterial(num);
    }
    
    u32 getMaterialCount() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getMaterialCount"));
        if(func)
        {
            return cpgf::fromVariant<u32 >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getMaterialCount();
    }
    u32 super_getMaterialCount() const
    {
        return irr::scene::ISceneNode::getMaterialCount();
    }
    
    const core::vector3df & getScale() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getScale"));
        if(func)
        {
            return cpgf::fromVariant<const core::vector3df & >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getScale();
    }
    const core::vector3df & super_getScale() const
    {
        return irr::scene::ISceneNode::getScale();
    }
    
    void setScale(const core::vector3df & scale)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setScale"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, scale);
            return;
        }
        irr::scene::ISceneNode::setScale(scale);
    }
    void super_setScale(const core::vector3df & scale)
    {
        irr::scene::ISceneNode::setScale(scale);
    }
    
    const core::vector3df & getRotation() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getRotation"));
        if(func)
        {
            return cpgf::fromVariant<const core::vector3df & >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getRotation();
    }
    const core::vector3df & super_getRotation() const
    {
        return irr::scene::ISceneNode::getRotation();
    }
    
    void setRotation(const core::vector3df & rotation)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setRotation"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, rotation);
            return;
        }
        irr::scene::ISceneNode::setRotation(rotation);
    }
    void super_setRotation(const core::vector3df & rotation)
    {
        irr::scene::ISceneNode::setRotation(rotation);
    }
    
    const core::vector3df & getPosition() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getPosition"));
        if(func)
        {
            return cpgf::fromVariant<const core::vector3df & >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getPosition();
    }
    const core::vector3df & super_getPosition() const
    {
        return irr::scene::ISceneNode::getPosition();
    }
    
    void setPosition(const core::vector3df & newpos)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setPosition"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, newpos);
            return;
        }
        irr::scene::ISceneNode::setPosition(newpos);
    }
    void super_setPosition(const core::vector3df & newpos)
    {
        irr::scene::ISceneNode::setPosition(newpos);
    }
    
    core::vector3df getAbsolutePosition() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getAbsolutePosition"));
        if(func)
        {
            return cpgf::fromVariant<core::vector3df >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getAbsolutePosition();
    }
    core::vector3df super_getAbsolutePosition() const
    {
        return irr::scene::ISceneNode::getAbsolutePosition();
    }
    
    void setDebugDataVisible(u32 state)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setDebugDataVisible"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, state);
            return;
        }
        irr::scene::ISceneNode::setDebugDataVisible(state);
    }
    void super_setDebugDataVisible(u32 state)
    {
        irr::scene::ISceneNode::setDebugDataVisible(state);
    }
    
    void setParent(ISceneNode * newParent)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setParent"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, newParent);
            return;
        }
        irr::scene::ISceneNode::setParent(newParent);
    }
    void super_setParent(ISceneNode * newParent)
    {
        irr::scene::ISceneNode::setParent(newParent);
    }
    
    ITriangleSelector * getTriangleSelector() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getTriangleSelector"));
        if(func)
        {
            return cpgf::fromVariant<ITriangleSelector * >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getTriangleSelector();
    }
    ITriangleSelector * super_getTriangleSelector() const
    {
        return irr::scene::ISceneNode::getTriangleSelector();
    }
    
    void setTriangleSelector(ITriangleSelector * selector)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("setTriangleSelector"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, selector);
            return;
        }
        irr::scene::ISceneNode::setTriangleSelector(selector);
    }
    void super_setTriangleSelector(ITriangleSelector * selector)
    {
        irr::scene::ISceneNode::setTriangleSelector(selector);
    }
    
    void updateAbsolutePosition()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("updateAbsolutePosition"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        irr::scene::ISceneNode::updateAbsolutePosition();
    }
    void super_updateAbsolutePosition()
    {
        irr::scene::ISceneNode::updateAbsolutePosition();
    }
    
    ESCENE_NODE_TYPE getType() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getType"));
        if(func)
        {
            return cpgf::fromVariant<ESCENE_NODE_TYPE >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getType();
    }
    ESCENE_NODE_TYPE super_getType() const
    {
        return irr::scene::ISceneNode::getType();
    }
    
    void serializeAttributes(io::IAttributes * out, io::SAttributeReadWriteOptions * options = 0) const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("serializeAttributes"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, out, options);
            return;
        }
        irr::scene::ISceneNode::serializeAttributes(out, options);
    }
    void super_serializeAttributes(io::IAttributes * out, io::SAttributeReadWriteOptions * options = 0) const
    {
        irr::scene::ISceneNode::serializeAttributes(out, options);
    }
    
    void deserializeAttributes(io::IAttributes * in, io::SAttributeReadWriteOptions * options = 0)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("deserializeAttributes"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, in, options);
            return;
        }
        irr::scene::ISceneNode::deserializeAttributes(in, options);
    }
    void super_deserializeAttributes(io::IAttributes * in, io::SAttributeReadWriteOptions * options = 0)
    {
        irr::scene::ISceneNode::deserializeAttributes(in, options);
    }
    
    ISceneNode * clone(ISceneNode * newParent = 0, ISceneManager * newManager = 0)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("clone"));
        if(func)
        {
            return cpgf::fromVariant<ISceneNode * >(cpgf::invokeScriptFunction(func.get(), this, newParent, newManager));
        }
        return irr::scene::ISceneNode::clone(newParent, newManager);
    }
    ISceneNode * super_clone(ISceneNode * newParent = 0, ISceneManager * newManager = 0)
    {
        return irr::scene::ISceneNode::clone(newParent, newManager);
    }
    
    ISceneManager * getSceneManager() const
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getSceneManager"));
        if(func)
        {
            return cpgf::fromVariant<ISceneManager * >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return irr::scene::ISceneNode::getSceneManager();
    }
    ISceneManager * super_getSceneManager() const
    {
        return irr::scene::ISceneNode::getSceneManager();
    }
};


template <typename D>
void buildMetaClass_ISceneNodeWrapper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (ISceneNode *, ISceneManager *, s32, const core::vector3df &, const core::vector3df &, const core::vector3df &)>()
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
    ;
    
    _d.CPGF_MD_TEMPLATE _method("super_OnRegisterSceneNode", (void (D::ClassType::*) ())&D::ClassType::super_OnRegisterSceneNode);
    _d.CPGF_MD_TEMPLATE _method("super_OnAnimate", (void (D::ClassType::*) (u32))&D::ClassType::super_OnAnimate);
    _d.CPGF_MD_TEMPLATE _method("super_render", (void (D::ClassType::*) ())&D::ClassType::super_render);
    _d.CPGF_MD_TEMPLATE _method("super_getName", (const c8 * (D::ClassType::*) () const)&D::ClassType::super_getName);
    _d.CPGF_MD_TEMPLATE _method("super_setName", (void (D::ClassType::*) (const c8 *))&D::ClassType::super_setName);
    _d.CPGF_MD_TEMPLATE _method("super_setName", (void (D::ClassType::*) (const core::stringc &))&D::ClassType::super_setName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("super_getBoundingBox", (const core::aabbox3d< f32 > & (D::ClassType::*) () const)&D::ClassType::super_getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("super_getTransformedBoundingBox", (const core::aabbox3d< f32 > (D::ClassType::*) () const)&D::ClassType::super_getTransformedBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("super_getAbsoluteTransformation", (const core::matrix4 & (D::ClassType::*) () const)&D::ClassType::super_getAbsoluteTransformation);
    _d.CPGF_MD_TEMPLATE _method("super_getRelativeTransformation", (core::matrix4 (D::ClassType::*) () const)&D::ClassType::super_getRelativeTransformation);
    _d.CPGF_MD_TEMPLATE _method("super_isVisible", (bool (D::ClassType::*) () const)&D::ClassType::super_isVisible);
    _d.CPGF_MD_TEMPLATE _method("super_isTrulyVisible", (bool (D::ClassType::*) () const)&D::ClassType::super_isTrulyVisible);
    _d.CPGF_MD_TEMPLATE _method("super_setVisible", (void (D::ClassType::*) (bool))&D::ClassType::super_setVisible);
    _d.CPGF_MD_TEMPLATE _method("super_getID", (s32 (D::ClassType::*) () const)&D::ClassType::super_getID);
    _d.CPGF_MD_TEMPLATE _method("super_setID", (void (D::ClassType::*) (s32))&D::ClassType::super_setID);
    _d.CPGF_MD_TEMPLATE _method("super_addChild", (void (D::ClassType::*) (ISceneNode *))&D::ClassType::super_addChild);
    _d.CPGF_MD_TEMPLATE _method("super_removeChild", (bool (D::ClassType::*) (ISceneNode *))&D::ClassType::super_removeChild);
    _d.CPGF_MD_TEMPLATE _method("super_removeAll", (void (D::ClassType::*) ())&D::ClassType::super_removeAll);
    _d.CPGF_MD_TEMPLATE _method("super_remove", (void (D::ClassType::*) ())&D::ClassType::super_remove);
    _d.CPGF_MD_TEMPLATE _method("super_addAnimator", (void (D::ClassType::*) (ISceneNodeAnimator *))&D::ClassType::super_addAnimator);
    _d.CPGF_MD_TEMPLATE _method("super_removeAnimator", (void (D::ClassType::*) (ISceneNodeAnimator *))&D::ClassType::super_removeAnimator);
    _d.CPGF_MD_TEMPLATE _method("super_removeAnimators", (void (D::ClassType::*) ())&D::ClassType::super_removeAnimators);
    _d.CPGF_MD_TEMPLATE _method("super_getMaterial", (video::SMaterial & (D::ClassType::*) (u32))&D::ClassType::super_getMaterial);
    _d.CPGF_MD_TEMPLATE _method("super_getMaterialCount", (u32 (D::ClassType::*) () const)&D::ClassType::super_getMaterialCount);
    _d.CPGF_MD_TEMPLATE _method("super_getScale", (const core::vector3df & (D::ClassType::*) () const)&D::ClassType::super_getScale);
    _d.CPGF_MD_TEMPLATE _method("super_setScale", (void (D::ClassType::*) (const core::vector3df &))&D::ClassType::super_setScale);
    _d.CPGF_MD_TEMPLATE _method("super_getRotation", (const core::vector3df & (D::ClassType::*) () const)&D::ClassType::super_getRotation);
    _d.CPGF_MD_TEMPLATE _method("super_setRotation", (void (D::ClassType::*) (const core::vector3df &))&D::ClassType::super_setRotation);
    _d.CPGF_MD_TEMPLATE _method("super_getPosition", (const core::vector3df & (D::ClassType::*) () const)&D::ClassType::super_getPosition);
    _d.CPGF_MD_TEMPLATE _method("super_setPosition", (void (D::ClassType::*) (const core::vector3df &))&D::ClassType::super_setPosition);
    _d.CPGF_MD_TEMPLATE _method("super_getAbsolutePosition", (core::vector3df (D::ClassType::*) () const)&D::ClassType::super_getAbsolutePosition);
    _d.CPGF_MD_TEMPLATE _method("super_setDebugDataVisible", (void (D::ClassType::*) (u32))&D::ClassType::super_setDebugDataVisible);
    _d.CPGF_MD_TEMPLATE _method("super_setParent", (void (D::ClassType::*) (ISceneNode *))&D::ClassType::super_setParent);
    _d.CPGF_MD_TEMPLATE _method("super_getTriangleSelector", (ITriangleSelector * (D::ClassType::*) () const)&D::ClassType::super_getTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("super_setTriangleSelector", (void (D::ClassType::*) (ITriangleSelector *))&D::ClassType::super_setTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("super_updateAbsolutePosition", (void (D::ClassType::*) ())&D::ClassType::super_updateAbsolutePosition);
    _d.CPGF_MD_TEMPLATE _method("super_getType", (ESCENE_NODE_TYPE (D::ClassType::*) () const)&D::ClassType::super_getType);
    _d.CPGF_MD_TEMPLATE _method("super_serializeAttributes", (void (D::ClassType::*) (io::IAttributes *, io::SAttributeReadWriteOptions *) const)&D::ClassType::super_serializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("super_deserializeAttributes", (void (D::ClassType::*) (io::IAttributes *, io::SAttributeReadWriteOptions *))&D::ClassType::super_deserializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("super_clone", (ISceneNode * (D::ClassType::*) (ISceneNode *, ISceneManager *))&D::ClassType::super_clone)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("super_getSceneManager", (ISceneManager * (D::ClassType::*) () const)&D::ClassType::super_getSceneManager);
    
    buildMetaClass_ISceneNode<D>(config, _d);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
