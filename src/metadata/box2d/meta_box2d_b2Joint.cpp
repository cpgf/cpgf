// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2Joint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2Joint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2joint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2joint(0, _d, NULL);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Jacobian()
{
    GDefineMetaClass<b2Jacobian> _d = GDefineMetaClass<b2Jacobian>::declare("b2Jacobian");
    buildMetaClass_B2Jacobian(0, _d, NULL);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Joint()
{
    GDefineMetaClass<b2Joint> _d = GDefineMetaClass<b2Joint>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2Joint");
    buildMetaClass_B2Joint(0, _d, NULL);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2JointDef()
{
    GDefineMetaClass<b2JointDef> _d = GDefineMetaClass<b2JointDef>::declare("b2JointDef");
    buildMetaClass_B2JointDef(0, _d, NULL);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2JointEdge()
{
    GDefineMetaClass<b2JointEdge> _d = GDefineMetaClass<b2JointEdge>::declare("b2JointEdge");
    buildMetaClass_B2JointEdge(0, _d, NULL);
    return _d.getMetaInfo();
}


} // namespace meta_box2d


