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
	sourceHeaderCode : "#include \"SFML/Audio.hpp\"" + "\n#include \"SFML/Config.hpp\"" + "\n#include \"SFML/Graphics.hpp\"" + "\n#include \"SFML/Network.hpp\"" + "\n#include \"SFML/System.hpp\"" + "\n#include \"SFML/Window.hpp\"",
	sourceHeaderReplacer : doHeaderReplace,
	metaHeaderPath : "",
	
	excludeCompound : [ "unix" ],
};

var re_doHeaderReplace = new RegExp(".*include/SFML", "i");

function processCallback(item, data)
{
	if(item.isMethod()) {
		var owner = item.getOwner();
		if(owner) {
			if(owner.getPrimaryName() == "Font") {
				if(item.getPrimaryName() == "LoadFromFile" || item.getPrimaryName() == "LoadFromMemory") {
					while(item.removeParameterDefaultValue()) {}
				}
			}
		}
	}
}

function doHeaderReplace(fileName)
{
	return fileName.replace(re_doHeaderReplace, "SFML").replace("Unix", "Win32");
}
