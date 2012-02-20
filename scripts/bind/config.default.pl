$config = {
	# the id must be unique in the meta data
	id => '',

	allowPublic => 1,
	allowProtected => 0,
	allowPrivate => 0,
	
	metaClassFunctionPrefix => 'buildMetaClass_',
	metaFileFunctionPrefix => 'buildMetaFile_',
	sourceFilePrefix => 'meta_',

	callback => undef,

	headerExtension => '.h',
	sourceExtension => '.cpp',

	# for source code
	autoRegisterToGlobal => 0,
	namespace => undef,
	sourceHeaderCode => undef,
	sourceHeaderReplacer => undef,
	metaHeaderPath => '',
};
