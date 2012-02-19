$config = {
	allowPublic => 1,
	allowProtected => 0,
	allowPrivate => 0,
	
	metaClassFunctionPrefix => 'buildMetaClass_',
	metaFileFunctionPrefix => 'buildMetaFile_',
	sourceFilePrefix => 'meta_',

	# for source code
	autoRegisterToGlobal => 0,
	namespace => undef,
	headerCode => undef,
	headerReplacer => undef,
	metaHeaderPath => '',
};
