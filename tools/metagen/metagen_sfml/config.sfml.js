var config = {
	projectID : "sfml",

	headerOutput : "../../../include/cpgf/metadata/sfml",
	sourceOutput : "../../../src/metadata/sfml",
	
	metaItemCallback : processCallback,
	
	cppNamespace : "meta_sfml",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_sfml_",
	mainSourceFile : "register_meta_sfml",

	autoRegisterToGlobal : true,

	wrapBitField : true, // default is true
	wrapOperator : true, // default is true 

	metaNamespace : "sfml",
	sourceHeaderCode : "#include \"SFML/Audio.hpp\"" + "\n#include \"SFML/Config.hpp\"" + "\n#include \"SFML/Graphics.hpp\"" + "\n#include \"SFML/Network.hpp\"" + "\n#include \"SFML/System.hpp\"" + "\n#include \"SFML/Window.hpp\"",
	sourceHeaderReplacer : [ "!.*include/SFML!i", "SFML" ],
	metaHeaderPath : "cpgf/metadata/sfml/",
	
	classTraits : [
		{ pattern : ".*\\b[io]stream$", traits : { copyConstructorHidden : true }  },
		{ pattern : ".*\\bw?string$", traits : { hasTypeConvertConstructor : true }  },
		{ pattern : ".*\\bUTF\\d+String$", traits : { hasTypeConvertConstructor : true }  },
	],

	predefinedTemplateInstances : [
		"Rect<int>", "",
		"Rect<float>", "",
		"Vector2<int>", "",
		"Vector2<float>", "",
		"Vector3<int>", "",
		"Vector3<float>", ""
	]
};

var re_Win32 = new RegExp("(.*)/Win32/(.*)", "i");

function processCallback(item, data)
{
	item.replaceInType("\\bSFML_API\\b", "");

	if(item.isMethod()) {
		var owner = item.getOwner();
		if(owner) {
			if(owner.getPrimaryName() == "Font") {
				// The default parameter is using private member in Font, we can't use it directly.
				if(item.getPrimaryName() == "LoadFromFile" || item.getPrimaryName() == "LoadFromMemory") {
					item.setParameterDefaultValueAt(-1, "cpgf_meta_ourDefaultCharset");
				}
			}
		}
	}
	else if(item.isClass()) {
		var loc = new String(item.getLocation());

		if(loc.match(re_Win32)) {
			loc = loc.replace(re_Win32, "$1/$2");
			item.setLocation(loc);
		}

		if(item.getPrimaryName() == "Font") {
			data.addHeaderCode("extern Uint32 cpgf_meta_ourDefaultCharset[];");
			data.addSourceCode("Uint32 cpgf_meta_ourDefaultCharset[] = {");
			data.addSourceCode("	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,");
			data.addSourceCode("	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,");
			data.addSourceCode("	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,");
			data.addSourceCode("	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,");
			data.addSourceCode("	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,");
			data.addSourceCode("	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,");
			data.addSourceCode("	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0x2A, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,");
			data.addSourceCode("	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,");
			data.addSourceCode("	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,");
			data.addSourceCode("	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,");
			data.addSourceCode("	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,");
			data.addSourceCode("	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE,");
			data.addSourceCode("	0x00");
			data.addSourceCode("};");
		}
	}
	else if(item.isConstructor()) {
//		if(item.getOwner().getPrimaryName() == "Thread" && item.hasParameter()) {
//			item.getParameterAt(0).setCallback("void (void * param)");
//		}
	}
}

