cpgf.import("cpgf", "builtin.debug");

var myProjectID = "metagen";

function cppInclude(inc)
{
	return " -I \"" + inc + "\"";
}

var config = {
	projectID : myProjectID,
	sourceRootPath : "../../src/",
	
	maxItemCountPerFile : 0,
	
	headerIncludePrefix : "metadata/",

	// clangOptions specifies the command line options to pass to clang compiler.
	// To get all usable options, run "clang -cc1 --help" to see the options from the help message.
	// The options can be put in a single line, with space as delimeter.
	// Here the options are put to several lines to explain how to use them.
	clangOptions : "-x c++" // Compile as C++ language
			+ " -std=c++11" // Which standard to use?
			+ " -fcxx-exceptions" // Enable exceptions
			+ " -fms-compatibility -fms-extensions -fdelayed-template-parsing" // Microsoft VC compatible settings

			// include directories, normally it looks this
			+ " -I \"C:/Program Files/Microsoft Visual Studio 9.0/VC/include\""
			// To avoid the escape of \", we use the auxiliary function cppInclude
			+ cppInclude("C:/source/llvm/build/tools/clang/include")
			+ cppInclude("C:/source/llvm/build/include")
			+ cppInclude("C:/source/llvm/tools/clang/include")
			+ cppInclude("C:/source/llvm/include")
			+ cppInclude("../../../../include")
			+ cppInclude("../../src")
	,
	
	files : [
		"project.h",
		"buildermodel/*.h",
		"model/*.h"
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
