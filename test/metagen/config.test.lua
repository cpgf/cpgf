cpgf.import("cpgf", "builtin.debug")

myProjectID = "metatest"

function cppInclude(inc)
	return " -I \"" .. inc .. "\""
end

function onFileCallback(sourceFile)
--	sourceFile.addInclude("../../../testmetatraits.h");
end

function onMainCallback(item, data)
	local name = nil;
	if item.isNamed() then
		name = item.getName()
	end

	if item.isClass() then
		if name == "SimpleOverride" then
			data.setWrapClass(true);
		end
	end
end

config = {
	projectID = myProjectID,
	sourceRootPath = "./",
	
	cppNamespace = "meta_test",

	targetFilePrefix = "meta_test_",
	
	maxItemCountPerFile = 0,
	
	headerIncludePrefix = "../include/",

	clangOptions = "-x c++"
			.. " -std=c++11"
			.. " -fcxx-exceptions"
			.. " -fms-compatibility -fms-extensions -fdelayed-template-parsing"

			.. cppInclude("D:/Program Files (x86)/Microsoft Visual Studio 11.0/VC/include")
			.. cppInclude("include")
			.. cppInclude("../../include")
	,
	
	files = {
		"include/*.h"
	},
	
	headerOutputPath = "metadata/include",
	sourceOutputPath = "metadata/src",
	
	metaNamespace = "metatest",
	mainRegisterFileName = "register_meta_test",
	
	templateInstantiations = {
		{ "TemplateClassA<int, char, 5>", "" },
		{ "TemplateClassB<TemplateBase>", "" },
	},
	
	force = false,
	
	stopOnCompileError = false,
	
	fileCallback = onFileCallback,
	mainCallback = onMainCallback,
	headerReplaceCallback = function(s) return s end,
}


