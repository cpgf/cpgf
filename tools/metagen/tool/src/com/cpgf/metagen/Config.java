package com.cpgf.metagen;

import com.cpgf.metagen.metadata.EnumCategory;
import com.cpgf.metagen.metawriter.callback.IOutputCallback;
import com.cpgf.metagen.metawriter.callback.ISourceHeaderReplacer;

public class Config {
	// projectID is used as unique ID for meta build function names.
	// It must be unique among the meta data.
	public String projectID = "";

	// The directory to create .h files in
	public String headerOutput = "./output";
	
	// The directory to create .cpp files in
	public String sourceOutput = "./output";

	// Specify the visibility of what kind of members to be built meta data.
	// Usually only allowPublic; otherwise; you can"t compile the meta data.
	public boolean allowPublic = true;
	public boolean allowProtected = false;
	public boolean allowPrivate = false;

	// Specify the meta data categories that are allowed to generate meta data
	public EnumCategory[] allowedMetaData = {
			EnumCategory.Constructor,
			EnumCategory.Destructor,
			EnumCategory.Class,
			EnumCategory.Constant,
			EnumCategory.Enum,
			EnumCategory.Field,
			EnumCategory.Method,
			EnumCategory.Operator
	};
	
	// Prefix for auto generated functions.
	// See the meta data for Box2D to see how they are used.
	public String metaClassFunctionPrefix = "buildMetaClass_";
	public String metaClassCreatePrefix = "createMetaClass_";

	public String metaClassMainRegisterPrefix = "registerMain_";

	// Prefix for .h and .cpp files.
	public String sourceFilePrefix = "meta_";
	
	// File name for the main entry source file.
	public String mainSourceFile = "register_meta";

	// A string for namespace for auto generated C++ code.
	// Choosing it carefully will avoid namespace clash.
	// null for no namespace.
	public String cppNamespace = null;

	// A callback sub routine.
	// See Box2D generator scripts for details.
	public IOutputCallback metaOutputCallback = null;

	// Header file extension.
	public String headerExtension = ".h";
	
	// Source file extension.
	public String sourceExtension = ".cpp";

	// A string of C++ code that will be put in front of all source files.
	// Useful if all source files must include extra headers.
	public String sourceHeaderCode = null;
	
	// See Box2D generator.
	public ISourceHeaderReplacer sourceHeaderReplacer = null;
	
	// A string of path that will be put in front of auto generated header.
	// See Box2D generator.
	public String metaHeaderPath = "";
	
	// auto register to global?
	public boolean autoRegisterToGlobal = true;
	
	// A string of namespace for meta data.
	// All meta data will be put into the namespace (pseudo class).
	// null for no namespace.
	public String metaNamespace = null;

}
