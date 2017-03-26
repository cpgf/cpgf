var config = {
	projectID : "tween",

	headerOutput : "../../../include/cpgf/metadata/tween",
	sourceOutput : "../../../src/metadata/tween",
	
	metaItemCallback : processCallback,
	
	cppNamespace : "meta_tween",
	
	metaClassFunctionPrefix : "buildMetaClass_",
	metaClassCreatePrefix : "createMetaClass_",

	metaClassMainRegisterPrefix : "registerMain_",

	sourceFilePrefix : "meta_tween",
	mainSourceFile : "register_meta_tween",

	autoRegisterToGlobal : true,

	wrapBitField : true, // default is true
	wrapOperator : true, // default is true 

	metaNamespace : "tween",
	sourceHeaderCode : '',
	sourceHeaderReplacer : [ '!.*include/cpgf!i', 'cpgf' ],
	metaHeaderPath : "cpgf/metadata/tween/",
	
	classTraits : [
	],

	predefinedTemplateInstances : [
	]
};

var ignoredItemNames = [
];

var wrapperClassNames = {
};

var parameterFromEndIndex = 1000;
var transferOwnerShipParameters = {
};


function normalizeParameterIndex(item, index)
{
	if(index >= parameterFromEndIndex) {
		return item.getParameterCount() - 1 - (index - parameterFromEndIndex);
	}
	else {
		return index;
	}
}

function processCallback(item, data)
{
	if(item.isConstant() || item.isOperator()) {
		data.skipBind = true;
	}
	
	var owner = item.getOwner();

	var itemName = item.getPrimaryName();
	
	if(itemName == 'repeat') {
		data.addAlias('_repeat');
	}
}

