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
		"#include \"../../../testmetatraits.h\"\n"
	,
	sourceHeaderReplacer : [ "!.*test/metagen/include!i", "../../include" ],
	metaHeaderPath : "../include/",

	classTraits : [
		{ pattern : ".*\\b[io]stream$", traits : { copyConstructorHidden : true }  },
		{ pattern : ".*\\bw?string$", traits : { hasTypeConvertConstructor : true }  },
	],

	predefinedTemplateInstances : [
		"TemplateClassA<int, char, 5>", "",
		"TemplateClassB<TemplateBase>", "",
	],
};

function processCallback(item, data)
{
	var name = item.getPrimaryName();

	if(item.isMethod()) {
		if(name == "GetMaxMotorForce") {
			data.skipBind = true;
		}
	}

	if (("" + item.getQualifiedName()) === "SimpleOverride::createHelperData") {
		item.setDiscardResultOwnership(true);
	}

	if(item.isClass()) {
		if(name == "SimpleOverride") {
			data.getWrapperConfig().setWrapClass(true);
		}
		if(name == "SimpleOverridableIncrementer") {
			data.getWrapperConfig().setWrapClass(true);
		}
	}
}

