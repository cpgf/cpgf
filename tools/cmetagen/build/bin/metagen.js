cpgf.import("cpgf", "builtin.debug");

var myProjectID = "metagen";

var config = {
	projectID : myProjectID,
	sourceRootPath : "../../src/",
	
	maxItemCountPerFile : 0,
	
	headerIncludePrefix : "metadata/",
	
	files : [
		"project.h",
		"buildermodel/*.h",
		"model/*.h"
	],
	
	includeDirectories : [
		"C:/Program Files/Microsoft Visual Studio 9.0/VC/include",
		"C:/source/llvm/build/tools/clang/include",
		"C:/source/llvm/build/include",
		"C:/source/llvm/tools/clang/include",
		"C:/source/llvm/include",
		"../../../../include",
		"../../src",
	],
	
	headerOutputPath : "../../src/metadata",
	sourceOutputPath : "../../src/metadata",
	
	mainRegisterFileName : "registerMain_" + myProjectID,
	
	templateInstantiations : [
		[ "ns1::TemplateA<int, 18>", "TemplateA_int", "TemplateA_wrapper_int" ]
	],
	
	force : false,
	
	stopOnCompileError : false,
	
	fileCallback : onFileCallback,
	mainCallback : onMainCallback,
	headerReplaceCallback : function(s) { return s.replace(/.*\/src\//, ""); },
}

var skippedItem = [
	"builderfilewriter",
	"buildersection",
	"buildertemplateinstantiation",
	"builderutil",
	"builderwriter",

	"cppclasstraits",
	"cppcontext",
	"cpppolicy",
	"cpputil"
];

var includeFiles = {
	"builderclass" : [
		"model/cppclass.h"
	],
};

function onFileCallback(sourceFile)
{
	var baseName = sourceFile.getBaseFileName();
//cpgf.traceLine("" + baseName);
	var len = skippedItem.length;
	for(var i = 0; i < len; ++i) {
		if(baseName == skippedItem[i]) {
			sourceFile.setSkipBind(true);
			return;
		}
	}

	if(includeFiles[baseName] != null) {
		len = includeFiles[baseName].length;
		for(var i = 0; i < len; ++i) {
			sourceFile.addInclude(includeFiles[baseName][i]);
		}
	}
}

function onMainCallback(item, data)
{
}
