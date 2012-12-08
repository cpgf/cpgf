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
	
	wrapBitField : true, // default is true
	wrapOperator : true, // default is true 

	autoRegisterToGlobal : true,

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

		"array<irr::scene::quake3::IEntity, irrAllocator<irr::scene::quake3::IEntity> >", "tQ3EntityList",
		"array<video::S3DVertex, irrAllocator<video::S3DVertex> >", "array_S3DVertex",
		"array<video::S3DVertex2TCoords, irrAllocator<video::S3DVertex2TCoords> >", "array_S3DVertex2TCoords",
		"array<scene::ISceneNode *, irrAllocator<scene::ISceneNode *> >", "array_ISceneNodePointer",
		"array<f32, irrAllocator<f32> >", "array_f32",
		"array<u16, irrAllocator<u16> >", "array_u16",
		"array<scene::ILightSceneNode *, irrAllocator<scene::ILightSceneNode *> >", "array_ILightSceneNodePointer",

//		"list<scene::ISceneNode *>", "list_ISceneNodePointer",

		"CMeshBuffer<video::S3DVertex>", "SMeshBuffer",
		"CMeshBuffer<video::S3DVertex2TCoords>", "SMeshBufferLightMap",
		"CMeshBuffer<video::S3DVertexTangents>", "SMeshBufferTangents",
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
	ILightManager : 1,
};

var skipedNames = {
	SExposedVideoData : 1,
	irrEmptyStringc : 1,
	isdigit : 1,
	isspace : 1,
	isupper : 1,
	AccessClass : 1,
	// Below four read/write functions cause link error. It's fine to ignore them since they are irrlicht "internal" functions.
	createLimitReadFile : 1,
	createMemoryReadFile : 1,
	createReadFile : 1,
	createWriteFile : 1,
	//
	PACK_STRUCT : 1,
};


function processCallback(item, data)
{
	if(item.isConstant()) {
		data.skipBind = true;
	}
	if(item.getLocation().match("irrpack")) {
		data.skipBind = true;
	}
	var name = item.getPrimaryName();
	if(skipedNames[name]) {
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
			else if(owner.getPrimaryName() == "array") {
				if(name == "==" || name == "!=") { // compile error, skip them for now
					data.skipBind = true;
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
		else if(name == "array") {
			data.addHeaderInclude("IQ3Shader.h");
			data.addHeaderInclude("SMeshBuffer.h");
			data.addHeaderInclude("ISceneNode.h");
			data.addHeaderInclude("ILightSceneNode.h");
		}
		else if(name == "list") {
			data.addHeaderInclude("ISceneNode.h");
		}
	}
}

