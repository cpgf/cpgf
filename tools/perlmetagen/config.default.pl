$config = {
	# projectID is used as unique ID for meta build function names.
	# It must be unique among the meta data.
	projectID => '',

	# The directory to create .h files in
	headerOutput => './output',
	
	# The directory to create .cpp files in
	sourceOutput => './output',
	
	# Specify the visibility of what kind of members to be built meta data.
	# Usually only allowPublic, otherwise, you can't compile the meta data.
	allowPublic => 1,
	allowProtected => 0,
	allowPrivate => 0,

	# Specify the meta data categories that are allowed to generate meta data
	allowedMetaData => [
		Item::catConstructor,
		Item::catDestructor,
		Item::catClass,
		Item::catDefine,
		Item::catEnum,
		Item::catField,
		Item::catMethod,
		Item::catOperator
	],
	
	# Prefix for auto generated functions.
	# See the meta data for Box2D to see how they are used.
	metaClassFunctionPrefix => 'buildMetaClass_',
	metaClassCreatePrefix => 'createMetaClass_',

	metaClassMainRegisterPrefix => 'registerMain_',

	# Prefix for .h and .cpp files.
	sourceFilePrefix => 'meta_',
	
	# File name for the main entry source file.
	mainSourceFile => 'register_meta',

	# A string for namespace for auto generated C++ code.
	# Choosing it carefully will avoid namespace clash.
	# undef for no namespace.
	cppNamespace => undef,

	# A callback sub routine.
	# See Box2D generator scripts for details.
	metaOutputCallback => undef,

	# Header file extension.
	headerExtension => '.h',
	
	# Source file extension.
	sourceExtension => '.cpp',

	# A string of C++ code that will be put in front of all source files.
	# Useful if all source files must include extra headers.
	sourceHeaderCode => undef,
	
	# See Box2D generator.
	sourceHeaderReplacer => undef,
	
	# A string of path that will be put in front of auto generated header.
	# See Box2D generator.
	metaHeaderPath => '',
	
	# auto register to global?
	autoRegisterToGlobal => 0,
	
	# A string of namespace for meta data.
	# All meta data will be put into the namespace (pseudo class).
	# undef for no namespace.
	metaNamespace => undef,
};
