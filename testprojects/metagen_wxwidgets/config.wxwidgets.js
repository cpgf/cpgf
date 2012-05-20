var config = {
	projectID : "wxwidgets",

	headerOutput : "cpp/include",
	sourceOutput : "cpp/src",
	
	metaItemCallback : processCallback,
	
	cppNamespace : "meta_wxwidgets",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_wxwidgets_",
	mainSourceFile : "register_meta_wxwidgets",

	autoRegisterToGlobal : true,
	
	metaNamespace : "wxwidgets",
	sourceHeaderCode : "#include \"wx/wx.h\"",
	sourceHeaderReplacer : [ "!.*include/SFML!i", "SFML" ],
	metaHeaderPath : "",
};

function processCallback(item, data)
{
	if(item.isMethod()) {
		if(item.getPrimaryName() == "GetMaxMotorForce") {
			data.skipBind = true;
		}
	}
}

