var config = {
	projectID : "metatest",

	headerOutput : "metadata/include",
	sourceOutput : "metadata/src",
	
	metaItemCallback : processCallback,
	
	cppNamespace : "meta_test",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_test_",
	mainSourceFile : "register_meta_test",

	autoRegisterToGlobal : true,
	
	metaNamespace : "metatest",
	sourceHeaderCode : "" +
		"#include \"cpgf/metatraits/gmetaconverter_string.h\"\n" +
		"#include \"cpgf/metatraits/gmetaconverter_widestring.h\"\n" +
		"#include \"cpgf/metatraits/gmetaserializer_string.h\"\n"

	,
	sourceHeaderReplacer : [ "!.*test/metagen/include!i", "../../include" ],
	metaHeaderPath : "../include/",

	classTraits : [
		{ pattern : ".*\\b[io]stream$", traits : { copyConstructorHidden : true }  },
		{ pattern : ".*\\bw?string$", traits : { hasTypeConvertConstructor : true }  },
	],

};

function processCallback(item, data)
{
	if(item.isMethod()) {
		if(item.getPrimaryName() == "GetMaxMotorForce") {
			data.skipBind = true;
		}
	}
}

