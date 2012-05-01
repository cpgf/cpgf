var config = {
	projectID : "box2d",

	headerOutput : "../../../include/cpgf/metadata/box2d",
	sourceOutput : "../../../src/metadata/box2d",
	
	metaOutputCallback : processCallback,
	
	cppNamespace : "meta_box2d",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_box2d_",
	mainSourceFile : "register_meta_box2d",

	autoRegisterToGlobal : true,
	
	metaNamespace : "box2d",
	sourceHeaderCode : "#include \"Box2D/Box2D.h\"",
	sourceHeaderReplacer : doHeaderReplace,
	metaHeaderPath : "cpgf/metadata/box2d/",
};

var re_doHeaderReplace = new RegExp(".*Box2D[^/]*/Box2D", "i");

function processCallback(item, data)
{
	if(item.isMethod()) {
		if(item.getName() == "GetMaxMotorForce") {
			data.skipBind = true;
		}
	}
}

function doHeaderReplace(fileName)
{
	return fileName.replace(re_doHeaderReplace, "Box2D");
}
