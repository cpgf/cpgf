var config = {
	projectID : "sfml",

	headerOutput : "cpp/include",
	sourceOutput : "cpp/src",
	
	metaOutputCallback : processCallback,
	
	cppNamespace : "meta_sfml",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_sfml_",
	mainSourceFile : "register_meta_sfml",

	autoRegisterToGlobal : true,
	
	metaNamespace : "sfml",
	sourceHeaderCode : "#include \"sfml/Audio.hpp\"" + "\n#include \"sfml/Config.hpp\"" + "\n#include \"sfml/Graphics.hpp\"" + "\n#include \"sfml/Network.hpp\"" + "\n#include \"sfml/System.hpp\"" + "\n#include \"sfml/Window.hpp\"",
	sourceHeaderReplacer : doHeaderReplace,
	metaHeaderPath : "",
	
	excludeCompound : [ "unix" ],
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
	return fileName.replace(re_doHeaderReplace, "Box2D").replace("Unix", "Win32");
}
