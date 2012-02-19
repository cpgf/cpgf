$config = {
	id => 'box2d',

	allowPublic => 1,
	allowProtected => 0,
	allowPrivate => 0,
	
	metaClassFunctionPrefix => 'buildMetaClass_',
	metaFileFunctionPrefix => 'buildMetaFile_',
	sourceFilePrefix => 'meta_',

	# for source code
	autoRegisterToGlobal => 1,
	namespace => 'box2d',
	hardcodeNamespace => 1,
	headerCode => '#include "Box2D/Box2D.h"',
	headerReplacer => \&doHeaderReplace,
	metaHeaderPath => 'cpgf/metadata/box2d/',
};

sub doHeaderReplace
{
	my ($fileName) = @_;

	$fileName =~ s/.*Box2D\/Box2D/Box2D/i;

	return $fileName;
}
