var config = {
	projectID : "wxwidgets",

	headerOutput : "cpp/include",
	sourceOutput : "cpp/src",
	
	metaOutputCallback : processCallback,
	
	cppNamespace : "meta_wxwidgets",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_wxwidgets_",
	mainSourceFile : "register_meta_wxwidgets",

	autoRegisterToGlobal : true,
	
	metaNamespace : "wxwidgets",
	sourceHeaderCode : "#include \"wx/wx.h\"",
	sourceHeaderReplacer : doHeaderReplace,
	metaHeaderPath : "",
};

var re_doHeaderReplace = new RegExp(".*Box2D[^/]*/Box2D", "i");

function processCallback(item, data)
{
	if(item.isMethod()) {
		if(item.getPrimaryName() == "GetMaxMotorForce") {
			data.skipBind = true;
		}
	}
}

function doHeaderReplace(fileName)
{
	return fileName.replace(re_doHeaderReplace, "Box2D");
}
