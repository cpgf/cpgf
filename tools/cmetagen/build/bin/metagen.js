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

function suppressClangWarnings()
{
	var s = "@";
	s += "#if defined(_MSC_VER)\n";
	s += "#pragma warning(push, 0)\n";
	s += "#endif\n";

	for(var i = 0; i < arguments.length; i++)
	{
		s += "#include \"" + arguments[i] + "\"\n";
	}
  		
	s += "#if defined(_MSC_VER)\n";
	s += "#pragma warning(pop)\n";
	s += "#endif\n";
	
	return s;
}

var includeFiles = {
	"cppcontext" : [
		"model/cppsourcefile.h",
		"project.h",
	],
	"cppitem" : [
		"project.h",
		suppressClangWarnings("clang/AST/Decl.h", "clang/Basic/Specifiers.h")
	],
	"cppclass" : [
		"model/cppcontainer.h",
		"model/cpppolicy.h",
		suppressClangWarnings("clang/AST/DeclTemplate.h", "clang/AST/DeclCXX.h")
	],
	"cppinvokable" : [
		"model/cpppolicy.h",
	],
	"cppmethod" : [
		"model/cppcontainer.h",
	],
	"cppfield" : [
		"model/cppcontainer.h",
	],
	"cppnamespace" : [
		"model/cppcontainer.h",
	],
	"cpptype" : [
		suppressClangWarnings("clang/AST/DeclTemplate.h"),
	],
	"buildercontext" : [
		"model/cppsourcefile.h",
		"project.h",
	],
	"builderfile" : [
		"project.h",
	],
	"builderitem" : [
		"project.h",
		"codewriter/codeblock.h",
	],
	"buildermethod" : [
		"model/cppcontainer.h",
	],
};

var metaIncludeFiles = {
	"cppitem" : [
		"model/cppcontainer.h",
	],
	"cppclass" : [
		"model/cppconstructor.h",
		"model/cppdestructor.h",
		"model/cppcontext.h",
	],
	"cppcontainer" : [
		"model/cppnamespace.h",
		"model/cppclass.h",
		"model/cppfield.h",
		"model/cppmethod.h",
		"model/cppenum.h",
		"model/cppoperator.h",
		"model/cppcontext.h",
	],
	"builderitem" : [
		"buildermodel/buildercontainer.h",
	],
};

var builderForwardIncludePattern = /^builder(\w+)/;
var cppForwardIncludePattern = /^cpp(\w+)/;

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

	if(baseName.match(builderForwardIncludePattern)) {
		// for file builderXXX.h, we include model/cppXXX.h	
		var s = baseName.replace(builderForwardIncludePattern, "model/cpp$1.h");
		sourceFile.addInclude(s);

		// Also include builderwriter since every one depends on it.	
		sourceFile.addInclude("buildermodel/builderwriter.h");
	}
	
	if(baseName.match(cppForwardIncludePattern)) {
		// Also include Decl since every one depends on it.	
		sourceFile.addInclude(suppressClangWarnings("clang/AST/Decl.h"));
	}

	if(includeFiles[baseName] != null) {
		len = includeFiles[baseName].length;
		for(var i = 0; i < len; ++i) {
			sourceFile.addInclude(includeFiles[baseName][i]);
		}
	}

	if(metaIncludeFiles[baseName] != null) {
		len = metaIncludeFiles[baseName].length;
		for(var i = 0; i < len; ++i) {
			sourceFile.addMetaInclude(metaIncludeFiles[baseName][i]);
		}
	}
}

function onMainCallback(item, data)
{
}
