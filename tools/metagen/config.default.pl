$config = {
	# the id must be unique in the meta data
	id => '',

	allowPublic => 1,
	allowProtected => 0,
	allowPrivate => 0,
	
	metaClassFunctionPrefix => 'buildMetaClass_',
	metaClassCreatePrefix => 'createMetaClass_',

	metaClassMainRegisterPrefix => 'registerMain_',

	sourceFilePrefix => 'meta_',
	mainSourceFilePrefix => 'register_main_',

	cppNamespace => undef,

	metaOutputCallback => undef,

	headerExtension => '.h',
	sourceExtension => '.cpp',

	sourceHeaderCode => undef,
	sourceHeaderReplacer => undef,
	metaHeaderPath => '',
	
	# auto register to global?
	autoRegisterToGlobal => 0,
	metaNamespace => undef,
};
