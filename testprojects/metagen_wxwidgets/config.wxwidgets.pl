$config = {
	projectID => 'wxwidgets',

	headerOutput => 'cpp/include',
	sourceOutput => 'cpp/src',
	
	metaOutputCallback => \&processCallback,
	
	cppNamespace => "meta_wxwidgets",
	
	metaClassFunctionPrefix => 'buildMetaClass_',
	metaClassCreatePrefix => 'createMetaClass_',

	metaClassMainRegisterPrefix => 'registerMain_',

	sourceFilePrefix => 'meta_wxwidgets_',
	mainSourceFile => 'register_meta_wxwidgets',

	autoRegisterToGlobal => 1,
	metaNamespace => 'wxwidgets',
	sourceHeaderCode => '#include "wx/wx.h"',
	sourceHeaderReplacer => \&doHeaderReplace,
	metaHeaderPath => '',
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

	$fileName =~ s!/interface/!/include/!i;

	return $fileName;
}
