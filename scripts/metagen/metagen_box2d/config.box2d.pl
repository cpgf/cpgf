$config = {
	id => 'box2d',

	metaOutputCallback => \&processCallback,
	
	cppNamespace => "meta_box2d",

	autoRegisterToGlobal => 1,
	metaNamespace => 'box2d',
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
