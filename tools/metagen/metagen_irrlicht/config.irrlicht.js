var config = {
	projectID : "irrlicht",

	headerOutput : "../../../include/cpgf/metadata/irrlicht",
	sourceOutput : "../../../src/metadata/irrlicht",
	
	metaItemCallback : processCallback,
	
	cppNamespace : "meta_irrlicht",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_irrlicht_",
	mainSourceFile : "register_meta_irrlicht",

	autoRegisterToGlobal : true,

	scriptable : false, // not implemented yet.
	
	metaNamespace : "irrlicht",
	headerHeaderCode : "#include \"gmetaobjectlifemanager_irrlicht_ireferencecounted.h\"",
	sourceHeaderCode : "#include \"irrlicht.h\"",
	sourceHeaderReplacer : [ "!.*include/!i", "" ],
	metaHeaderPath : "cpgf/metadata/irrlicht/",
	
	classTraits : [
		{ pattern : ".*\\b[io]stream$", traits : { copyConstructorHidden : true }  },
		{ pattern : ".*\\bw?string$", traits : { hasTypeConvertConstructor : true }  },
		{ pattern : ".*\\bUTF\\d+String$", traits : { hasTypeConvertConstructor : true }  },
		{ pattern : ".*\\bpath$", traits : { hasTypeConvertConstructor : true }  },
		{ pattern : ".*\\bstringc$", traits : { hasTypeConvertConstructor : true }  },
		{ pattern : ".*\\bstringw$", traits : { hasTypeConvertConstructor : true }  },
	],

	predefinedTemplateInstances : [
		"dimension2d<u32>", "",
		"dimension2d<f32>", "",
		"dimension2d<u32>", "dimension2du",
		"dimension2d<f32>", "dimension2df",
		"dimension2d<s32>", "dimension2di",
		"rect<s32>", "",
		"vector3d<f32>", "vector3df",
		"vector3d<s32>", "vector3di",
		"line3d<f32>", "",
		"triangle3d<f32>", "triangle3df",
		"triangle3d<s32>", "triangle3di",
		"CMatrix4<f32>", "matrix4",
		"aabbox3d<f32>", "",
		"vector2d<s32>", "position2d_s32",
		"vector2d<s32>", "position2di",
		"vector2d<f32>", "position2d_f32",
		"vector2d<f32>", "position2df",
		"plane3d<f32>", "plane3df",
		"plane3d<s32>", "plane3di",
		"IIrrXMLReader<wchar_t, IReferenceCounted>", "IXMLReader",
		"IIrrXMLReader<c8, IReferenceCounted>", "IXMLReaderUTF8",
	],
	
	parameterTypeReplacer : [
		"IRRLICHT_API", "",
		"IRRCALLCONV", "",
		"REALINLINE", "",
	]
};

var wrapperClassNames = {
	ISceneNode : 1,
	IEventReceiver : 1,
	IShaderConstantSetCallBack : 1,
};


function processCallback(item, data)
{
	if(item.isConstant()) {
		data.skipBind = true;
	}
	var name = item.getPrimaryName();
	if(name == "SExposedVideoData"
		|| name == "irrEmptyStringc"
		|| name == "isdigit" || name == "isspace" || name == "isupper"
		|| name == "AccessClass")
	{
		data.skipBind = true;
	}
	
	if(item.isOperator()) {
		var owner = item.getOwner();
		if(owner) {
			if(owner.getPrimaryName().match("vector3d")) {
				if(item.getResultType().getLiteralType().match("\\bs32\\b")) {
					// special for specialization for s32 in vector3d
					item.setSelf("vector3d<s32>");
				}
			}
		}
	}
	if(item.isClass()) {
		if(wrapperClassNames[name]) {
			data.getWrapperConfig().setWrapClass(true);
		}
		if(name == "IVideoDriver") {
			data.putInSeparatedFile();
		}
	}
}

