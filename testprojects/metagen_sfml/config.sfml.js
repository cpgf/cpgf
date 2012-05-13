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
	
	classTraits : [
		{ pattern : ".*\\b[io]stream$", traits : { copyConstructorHidden : true }  },
		{ pattern : ".*\\bw?string$", traits : { hasTypeConvertConstructor : true }  },
		{ pattern : ".*\\bUTF\\d+String$", traits : { hasTypeConvertConstructor : true }  },
	],

	parseFileNameCallback : doParseFileNameCallback
};

var re_doHeaderReplace = new RegExp(".*include/SFML", "i");

function processCallback(item, data)
{
	item.replaceInType("\\bSFML_API\\b", "");

	if(item.isMethod()) {
		var owner = item.getOwner();
		if(owner) {
			if(owner.getPrimaryName() == "Font") {
				// The default parameter is using private member in Font, we can't use it.
				if(item.getPrimaryName() == "LoadFromFile" || item.getPrimaryName() == "LoadFromMemory") {
					while(item.removeParameterDefaultValue()) {}
					item.removeParameter("CharSize");
					item.removeParameter("Charset");
				}
			}
		}
	}
}

function doHeaderReplace(fileName)
{
	return fileName.replace(re_doHeaderReplace, "SFML").replace("Unix", "Win32");
}

function doParseFileNameCallback(fileName)
{
	if(fileName.indexOf("unix") >= 0) {
		fileName = null;
	}
	
	return fileName;
}
