// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2ContactSolver.h"

#include "cpgf/metadata/box2d/meta_box2d_b2ContactSolver.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2ContactConstraint()
{
    GDefineMetaClass<b2ContactConstraint> _d = GDefineMetaClass<b2ContactConstraint>::declare("b2ContactConstraint");
    buildMetaClass_B2ContactConstraint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2ContactConstraintPoint()
{
    GDefineMetaClass<b2ContactConstraintPoint> _d = GDefineMetaClass<b2ContactConstraintPoint>::declare("b2ContactConstraintPoint");
    buildMetaClass_B2ContactConstraintPoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2ContactSolver()
{
    GDefineMetaClass<b2ContactSolver> _d = GDefineMetaClass<b2ContactSolver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2ContactSolver");
    buildMetaClass_B2ContactSolver(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


