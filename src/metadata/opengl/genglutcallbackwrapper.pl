use strict;
use warnings;

my $glutFunc = <<EOM;
GLUTAPI void APIENTRY glutDisplayFunc(void (GLUTCALLBACK *func)(void));
GLUTAPI void APIENTRY glutReshapeFunc(void (GLUTCALLBACK *func)(int width, int height));
GLUTAPI void APIENTRY glutKeyboardFunc(void (GLUTCALLBACK *func)(unsigned char key, int x, int y));
GLUTAPI void APIENTRY glutMouseFunc(void (GLUTCALLBACK *func)(int button, int state, int x, int y));
GLUTAPI void APIENTRY glutMotionFunc(void (GLUTCALLBACK *func)(int x, int y));
GLUTAPI void APIENTRY glutPassiveMotionFunc(void (GLUTCALLBACK *func)(int x, int y));
GLUTAPI void APIENTRY glutEntryFunc(void (GLUTCALLBACK *func)(int state));
GLUTAPI void APIENTRY glutVisibilityFunc(void (GLUTCALLBACK *func)(int state));
GLUTAPI void APIENTRY glutIdleFunc(void (GLUTCALLBACK *func)(void));
GLUTAPI void APIENTRY glutTimerFunc(unsigned int millis, void (GLUTCALLBACK *func)(int value), int value);
GLUTAPI void APIENTRY glutMenuStateFunc(void (GLUTCALLBACK *func)(int state));
#if (GLUT_API_VERSION >= 2)
GLUTAPI void APIENTRY glutSpecialFunc(void (GLUTCALLBACK *func)(int key, int x, int y));
GLUTAPI void APIENTRY glutSpaceballMotionFunc(void (GLUTCALLBACK *func)(int x, int y, int z));
GLUTAPI void APIENTRY glutSpaceballRotateFunc(void (GLUTCALLBACK *func)(int x, int y, int z));
GLUTAPI void APIENTRY glutSpaceballButtonFunc(void (GLUTCALLBACK *func)(int button, int state));
GLUTAPI void APIENTRY glutButtonBoxFunc(void (GLUTCALLBACK *func)(int button, int state));
GLUTAPI void APIENTRY glutDialsFunc(void (GLUTCALLBACK *func)(int dial, int value));
GLUTAPI void APIENTRY glutTabletMotionFunc(void (GLUTCALLBACK *func)(int x, int y));
GLUTAPI void APIENTRY glutTabletButtonFunc(void (GLUTCALLBACK *func)(int button, int state, int x, int y));
#if (GLUT_API_VERSION >= 3)
GLUTAPI void APIENTRY glutMenuStatusFunc(void (GLUTCALLBACK *func)(int status, int x, int y));
GLUTAPI void APIENTRY glutOverlayDisplayFunc(void (GLUTCALLBACK *func)(void));
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
GLUTAPI void APIENTRY glutWindowStatusFunc(void (GLUTCALLBACK *func)(int state));
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
GLUTAPI void APIENTRY glutKeyboardUpFunc(void (GLUTCALLBACK *func)(unsigned char key, int x, int y));
GLUTAPI void APIENTRY glutSpecialUpFunc(void (GLUTCALLBACK *func)(int key, int x, int y));
GLUTAPI void APIENTRY glutJoystickFunc(void (GLUTCALLBACK *func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);
#endif
#endif
#endif
EOM

my $output = '';
my $proto = '';
my $binding = "";

my @lines = split(/\n/, $glutFunc);
foreach(@lines) {
	&processLine($_);
}
print "===proto \n";
print $proto;
print "\n\n===binding \n";
print $binding;
print "\n\n===code \n";
print $output;


sub out
{
	my ($text) = @_;
	
	$output .= $text;
}

sub outLine
{
	my ($line) = @_;
	
	&out($line . "\n");
}

sub removeSpace
{
	my ($text) = @_;
	
	$text =~ s/^\s+//;
	$text =~ s/\s+$//;
	
	return $text;
}

sub processLine
{
	my ($line) = @_;

	if($line =~ /APIENTRY\s+glut(\w+)Func\s*\(([^](]+)\([\w\s\*]+\)\s*(\([\w\s\,]+\))\s*(\,[\w\s\,]+)?\);/) {
		my ($name, $callbackReturn, $callbackParam, $funcParam) = ($1, $2, $3, $4);
		my $callbackParamList = &parseParam($callbackParam);
		my $funcParamList = &parseParam($funcParam);
		my $glutName = 'glut' . $name . 'Func';
		my $dataName = lcfirst($name);
		
		$callbackReturn = &removeSpace($callbackReturn);
		
		&out($callbackReturn . ' callback_' . $glutName . '(');
		
		my $first = 1;
		foreach(@{$callbackParamList}) {
			my $p = $_;
			&out(', ') unless($first);
			&out($p->{type} . ' ' . $p->{name});
			$first = 0;
		}
		
		&outLine(')');
		&outLine('{');
		&out("\tinvokeScriptFunction(getCurrentCallbackData()->" . $dataName . '.get()');
		foreach(@{$callbackParamList}) {
			my $p = $_;
			&out(', ' . $p->{name});
		}
		&outLine(');');
		&outLine('}');
		&outLine('');
		
		$proto .= $callbackReturn . ' helper_' . $glutName . '(IScriptFunction * scriptFunction';
		&out($callbackReturn . ' helper_' . $glutName . '(IScriptFunction * scriptFunction');
		foreach(@{$funcParamList}) {
			my $p = $_;
			&out(', ' . $p->{type} . ' ' . $p->{name});
			$proto .= ', ' . $p->{type} . ' ' . $p->{name};
		}
		$proto .= ");\n";
		
		&outLine(')');
		&outLine('{');
		&outLine("\tgetCurrentCallbackData()->" . $dataName . '.reset(scriptFunction);');
		&out("\t" . $glutName . "(&callback_" . $glutName);
		foreach(@{$funcParamList}) {
			my $p = $_;
			&out(', ' . $p->{name});
		}
		&outLine(');');
		&outLine('}');
		&outLine('');
		
		$binding .= 'define.CPGF_MD_TEMPLATE _method("' . $glutName . '", &helper_' . $glutName . ");\n";
	}
	else {
		&outLine($line);
		$proto .= $line . "\n";
		$binding .= $line . "\n";
	}
}

sub parseParam
{
	my ($line) = @_;
	my $params = [];
	
	return $params unless defined $line;
	
	while($line =~ /((\w+\s+)+)(\w+)\b/g) {
		my ($type, $name) = ($1, $3);
		
		$type = &removeSpace($type);
		$name = &removeSpace($name);
		
		my $p = { type => $type, name => $name };
		push @$params, $p;
	}
	
	return $params;
}


