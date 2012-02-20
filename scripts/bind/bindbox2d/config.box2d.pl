$config = {
	id => 'box2d',

	callback => \&processCallback,
	
	# for source code
	autoRegisterToGlobal => 1,
	namespace => 'box2d',
	sourceHeaderCode => '#include "Box2D/Box2D.h"',
	sourceHeaderReplacer => \&doHeaderReplace,
	metaHeaderPath => 'cpgf/metadata/box2d/',
};

sub processCallback
{
	my ($item, $callbackParam) = @_;

	if($item->isMethod) {
		if($item->getName eq 'GetMaxMotorForce') {
			$callbackParam->{skipBind} = 1;
		}
	}
}

sub doHeaderReplace
{
	my ($fileName) = @_;

	$fileName =~ s!.*Box2D[^/]*/Box2D!Box2D!i;

	return $fileName;
}
