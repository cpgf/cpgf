package org.cpgf.metagen;

import org.cpgf.metagen.metadata.EnumCategory;
import org.cpgf.metagen.metadata.PredefinedClassTraitsList;
import org.cpgf.metagen.metawriter.callback.IOutputCallback;

public class Config {
	// projectID is used as unique ID for meta build function names.
	// It must be unique among the meta data.
	public String projectID = "";

	// The directory to create .h files in.
	// The tool will create it and its parent directory if it doesn't yet.
	public String headerOutput = "./output";
	
	// The directory to create .cpp files in
	// The tool will create it and its parent directory if it doesn't yet.
	public String sourceOutput = "./output";

	// Specify the visibility of what kind of members to be built meta data.
	// Usually only allowPublic; otherwise; you can"t compile the meta data.
	public boolean allowPublic = true;
	public boolean allowProtected = false;
	public boolean allowPrivate = false;
	
	// Specify the meta data categories that are allowed to generate meta data
	// NOTE: current you can't change it in Javascript config file.
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
	
	// A callback called before the items are outputed.
	// See SFML generator scripts for details.
	public IOutputCallback metaItemCallback = null;

	// Header file extension.
	public String headerExtension = ".h";

	// Source file extension.
	public String sourceExtension = ".cpp";

	// A string of C++ code that will be put in front of all header files.
	// Useful if all header files must include extra headers.
	public String headerHeaderCode = null;
	
	// A string of C++ code that will be put in front of all source files.
	// Useful if all source files must include extra headers.
	public String sourceHeaderCode = null;
	
	// See SFML generator.
	public String[] sourceHeaderReplacer = null;
	
	public String[] parameterTypeReplacer = null;

	// A string of path that will be put in front of auto generated header.
	// See Box2D generator.
	public String metaHeaderPath = "";
	
	// auto register to global
	// If this property is true, .cpp source files will be generated
	// so the meta data will be auto registered after linked to the .cpp files.
	// If this property is false, only header files will be generated
	// so you need to call the reflect function manually.
	public boolean autoRegisterToGlobal = true;
	public boolean generateRegisterHeader = true;
	
	// A string of namespace for meta data.
	// All meta data will be put into the namespace (pseudo class).
	// null for no namespace.
	public String metaNamespace = null;

	public PredefinedClassTraitsList classTraits = new PredefinedClassTraitsList();
	
	public String[] predefinedTemplateInstances = {};
	
	public boolean wrapBitField = true;
	public boolean wrapOperator = true;

	// Not implemented yet. DON'T set it to true.
	public boolean wrapCallback = false;
	
	public String scriptClassWrapperPostfix = "Wrapper";

}
