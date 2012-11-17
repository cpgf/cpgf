var config = {
	projectID : "box2d",

	headerOutput : "../../../include/cpgf/metadata/box2d",
	sourceOutput : "../../../src/metadata/box2d",
	
	metaItemCallback : processCallback,
	
	cppNamespace : "meta_box2d",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_box2d_",
	mainSourceFile : "register_meta_box2d",

	wrapBitField : true, // default is true
	wrapOperator : true, // default is true 

	autoRegisterToGlobal : true,
	
	metaNamespace : "box2d",
	sourceHeaderCode : "#include \"Box2D/Box2D.h\"",
	sourceHeaderReplacer : [ "!.*Box2D[^/]*/Box2D!i", "Box2D" ],
	metaHeaderPath : "cpgf/metadata/box2d/",
};

function processCallback(item, data)
{
	if(item.isMethod()) {
		if(item.getPrimaryName() == "GetMaxMotorForce") {
			data.skipBind = true;
		}
	}
}

