use strict;
use warnings;
use Switch;

my %typeProto = (
	generator => \&generateNumeric,
	name => 'unsigned int',
	min => 0,
	max => 0xffffff,
);

my %paramProto = (
	name => 'p0',
	type => \%typeProto,
	pointers => 0,
	reference => 0,
	const => 0,
	volatile => 0,
	constPointer => 0,
	volatilePointer => 0,
	constReference => 0,
	volatileReference => 0,
	valueList => [ 0, 1, 2 ], # expected value to verify, for primary type and one ~ two levels pointer
);

my %functionProto = (
	name => 'TestFunction1',
	params => [
		\%paramProto,
	],
	result => \%paramProto, # undef for void
	resultValue => '0', # used to verify function return
	virtual => 0,
	static => 0,
	class => undef, # should be classProto
);

my %classProto = (
	name => 'TestObject1', # undef for global, non-class
	functions => [
		\%functionProto
	],
	enumName => 'EType',
	super => undef, # should be classProto
	namespace => undef, # should be namespaceProto
);

my %namespaceProto = (
	name => 'TestNameSpace', #undef for global space
	classes => [
		\%classProto,
	],
);

my %paramIndexProto = (
	mainIndex => 0,
	subIndex => 0,
);

my @basicTypes = (
#	{
#		generator => \&generateNumeric,
#		name => 'char',
#		min => -0x7f,
#		max => 0x7f,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'signed char',
#		min => -0x7f,
#		max => 0x7f,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'unsigned char',
#		min => 0,
#		max => 0xff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'signed short',
#		min => -0x7fff,
#		max => 0x7fff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'unsigned short',
#		min => 0,
#		max => 0xffff,
#	},

	{
		generator => \&generateNumeric,
		name => 'signed int',
		min => -0x7fffff,
		max => 0x7fffff,
	},

#	{
#		generator => \&generateNumeric,
#		name => 'unsigned int',
#		min => 0,
#		max => 0xffffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'signed long',
#		min => -0x7fffff,
#		max => 0x7fffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'unsigned long',
#		min => 0,
#		max => 0xffffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'signed long long',
#		min => -0x7fffff,
#		max => 0x7fffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'unsigned long long',
#		min => 0,
#		max => 0xffffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'float',
#		min => -0x7fffff,
#		max => 0x7fffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'double',
#		min => 0,
#		max => 0xffffff,
#	},

#	{
#		generator => \&generateNumeric,
#		name => 'long double',
#		min => -0x7fffff,
#		max => 0x7fffff,
#	},

);

my $configMaxParamCount = 3;
my $configMaxValues = 3;
my $configAssertEqual = 'TS_ASSERT_EQUALS(x1, x2)';
my $configCoveredParamCount = ($#basicTypes + 1) * 36;

my $indent = 0;

&doMain;

sub doMain
{
	srand;
	
	my $mainText = &generateMain;
	
	print $mainText;
}

sub generateMain
{
	my $result = '';
	
	my $ns = &generateNamespace(-1);
	
	$result .= &textNamespace($ns);
	
	$result .= &textReflectGlobal([$ns]);

	$result .= &textTest([$ns]);
	
	return $result;
}

sub getNextParam
{
	my ($index, $order) = @_;
	my $mainIndex = $index->{mainIndex};
	my $subIndex = $index->{subIndex};
	my $param = {
		name => 'p' . $order,
		type => $basicTypes[$mainIndex],
		pointers => 0,
		reference => 0,
		const => 0,
		volatile => 0,
	};
	
	switch($subIndex) {
		case 0			{}
		case [ 1, 2 ]	{ $param->{pointers} = $subIndex; }
		case [ 3, 4 ]	{ $param->{pointers} = $subIndex; $param->{const} = 1; }
		case [ 5, 6 ]	{ $param->{pointers} = $subIndex; $param->{volatile} = 1; }
		case [ 7, 8 ]	{ $param->{pointers} = $subIndex; $param->{const} = 1; $param->{volatile} = 1; }
		
		case [ 9, 10 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; }
		case [ 11, 12 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{const} = 1; }
		case [ 13, 14 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{volatile} = 1; }
		case [ 15, 16 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{const} = 1; $param->{volatile} = 1; }
		
		case [ 17, 18 ]	{ $param->{pointers} = $subIndex; $param->{volatilePointer} = 1; }
		case [ 19, 20 ]	{ $param->{pointers} = $subIndex; $param->{volatilePointer} = 1; $param->{const} = 1; }
		case [ 21, 22 ]	{ $param->{pointers} = $subIndex; $param->{volatilePointer} = 1; $param->{volatile} = 1; }
		case [ 23, 24 ]	{ $param->{pointers} = $subIndex; $param->{volatilePointer} = 1; $param->{const} = 1; $param->{volatile} = 1; }
		
		case [ 25, 26 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{volatilePointer} = 1; }
		case [ 27, 28 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{volatilePointer} = 1; $param->{const} = 1; }
		case [ 29, 30 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{volatilePointer} = 1; $param->{volatile} = 1; }
		case [ 31, 32 ]	{ $param->{pointers} = $subIndex; $param->{constPointer} = 1; $param->{volatilePointer} = 1; $param->{const} = 1; $param->{volatile} = 1; }
		
		case 33			{ $param->{reference} = 1; $param->{constReference} = 1; }
		case 34			{ $param->{reference} = 1; $param->{volatileReference} = 1; }
		case 35			{ $param->{reference} = 1; $param->{constReference} = 1; $param->{volatileReference} = 1; }
		else			{ $subIndex = -1; ++$mainIndex; }
	}
$param->{reference} = 0;	
	$param->{pointers} -= 2 while($param->{pointers} > 2);
	
	++$subIndex;
	$mainIndex = $mainIndex % ($#basicTypes + 1);
	
	$index->{mainIndex} = $mainIndex;
	$index->{subIndex} = $subIndex;
	
	foreach(1 .. $configMaxValues) {
		my $value = &generateParamValue($param);
		push @{$param->{valueList}}, $value;
	}

	return $param;
};

sub textReflectClass
{
	my ($class) = @_;
	my $result = '';
	
	if(defined($class->{name})) {
		$result .= &textIndent . 'gDefineMetaClass(' . $class->{name} . ", GMetaRoot) {\n";
		&enterIndent;
	}
	
	foreach(@{$class->{functions}}) {
		$result .= &textReflectFunction($_, $class) . "\n";
	}
	
	if(defined($class->{name})) {
		&leaveIndent;
		$result .= &textIndent . "}\n";
	}
	
	return $result;
}

sub textReflectFunction
{
	my ($func, $class) = @_;
	my $result = '';
	my $prefix = '';
	
	$prefix = '' if(defined($class) && defined($class->{name}));
	my $funcName = $func->{name};
	$funcName = $func->{class}->{name} . '::' . $funcName if(defined $func->{class}->{name});
	$result .= &textIndent . 'reflectMethod("' . $func->{name} . '", &' . $funcName . ');';
	
	return $result;
};

sub textTestFunction
{
	my ($func) = @_;
	my $result = '';
	
	my $global = ! defined $func->{class}->{name};
	
	$result .= &textIndent . "{\n";
	&enterIndent;

	$result .= &textIndent . "const GMetaClass * metaClass = ";
	if($global) {
		$result .= "getGlobalMetaClass()";
	}
	else {
		$result .= "findMetaClass<" . $func->{class}->{name} . ">()";
	}
	$result .= ";\n";
	$result .= &textIndent . 'const GMetaMethod * method = metaClass->getMethod("' . $func->{name} . '");' . "\n";
	
	my $objPtr;
	
	if($global) {
		$objPtr = 'NULL';
	}
	else {
		$result .= &textIndent . $func->{class}->{name} . " obj;\n";
		$objPtr = '&obj';
	}
	
	my $callParams = $objPtr;
	for(my $i = 0; $i <= $#{@{$func->{params}}}; ++$i) {
		my $param = $func->{params}->[$i];
		my $localName = 't' . $i;
		
		$callParams .= ', ';
		
		if($param->{pointers} == 2) {
			$param->{pointers} = 1;
			$result .= &textIndent . &textParamType($param, 0) . ' ' . $localName . ' [] = { ';
			for(my $k = 0; $k <= $#{@{$param->{valueList}}}; ++$k) {
				$result .= ', ' if($k);
				$result .= &textCastParam($param, $k);
			}
			$param->{pointers} = 2;
			$result .= " }; \n";
			
#			$result .= &textIndent . &textParamType($param, 0) . $localName . ' = ' . $localName . "_a;\n";
			$callParams .= $localName;
		}
		else {
			if($param->{pointers} || $param->{reference}) {
				my $oldPointers = $param->{pointers};
				my $oldReference = $param->{reference};
				$param->{pointers} = 0;
				$param->{reference} = 0;
				
				$result .= &textIndent . &textParamType($param, 0) . ' ' . $localName . ' = ' . &textCastParam($param, 0) . ";\n";
				
				$param->{pointers} = $oldPointers;
				$param->{reference} = $oldReference;
				
				$callParams .= '&' if($param->{pointers});
				$callParams .= $localName;
			}
			else {
				$callParams .= &textCastParam($param, 0);
			}
		}
	}
	
	$result .= &textIndent . 'method->invoke(' . $callParams . ");\n";
	
	&leaveIndent;
	$result .= &textIndent . "}\n";
	
	return $result;
};

sub textCastParam
{
	my ($param, $index) = @_;
	
	return '(' . &textParamType($param, 0) . ')(' . $param->{valueList}->[$index] . ')';
}

sub textTestClass
{
	my ($class) = @_;
	my $result = '';

	foreach(@{$class->{functions}}) {
		$result .= &textTestFunction($_) . "\n";
	}
	
	return $result;
};

sub textTestNamespace
{
	my ($ns) = @_;
	my $result = '';

	foreach(@{$ns->{classes}}) {
		$result .= &textTestClass($_) . "\n";
	}
	
	return $result;
};

sub textNamespace
{
	my ($ns) = @_;
	my $result = '';
	
	if(defined $ns->{name}) {
		$result .= &textIndent . "namespace " . $ns->{name};
		$result .= &textIndent . " {\n";
	}

	foreach(@{$ns->{classes}}) {	
		$result .= &textClass($_);
	}
	
	if(defined $ns->{name}) {
		$result .= &textIndent . "}\n";
	}
	
	return $result;
}

sub textClass
{
	my ($class) = @_;
	my $result = '';
	
	if(defined $class->{name}) {
		$result .= &textIndent . "class " . $class->{name};
		$result .= &textIndent . " {\n";
		$result .= &textIndent . "public:\n";
		
		&enterIndent;
	}
	
	$result .= &textFunctionList($class->{functions});
	
	if(defined $class->{name}) {
		$result .= &textReflectClass($class);
		&leaveIndent;
		$result .= &textIndent . "};\n";
	}
	
	return $result;
};

sub textFunctionList
{
	my ($funcList) = @_;
	my $result = '';
	
	for(my $i = 0; $i <= $#{@{$funcList}}; ++$i) {
		$result .= &textFunction($funcList->[$i]);
		$result .= "\n";
	}
	
	return $result;
}

sub textFunction
{
	my ($func) = @_;
	my $result = '';
	
	$result .= &textFunctionDeclaration($func, 0) . "\n";
	$result .= &textIndent . "{\n";
	&enterIndent;
		$result .= &textFunctionBody($func) . "\n";
	&leaveIndent;
	$result .= &textIndent . "}\n";
	
	return $result;
}

sub textFunctionBody
{
	my ($func) = @_;
	my $result = '';

	foreach(@{$func->{params}}) {
		$result .= &textVerifyParam($_);
	}
	
	if(defined $func->{result}) {
		$result .= &textIndent . 'return ' . &textResultCast($func->{result}) . ';';
	}
	
	return $result;
}

sub textResultCast
{
	my ($return) = @_;
	my $result = '';
	
	if($return->{reference}) {
		$result .= 'TestConstructReference<' . &textParamType($return, 0) . '>::construct(' . $return->{valueList}->[0] . ')';
	}
	else {
		$result .= '(' . &textParamType($return, 0) . ')(' . $return->{valueList}->[0] . ')';
	}
	
	return $result;
}

# generate function declaration without semicolon
sub textFunctionDeclaration
{
	my ($func, $prefixWithClassName) = @_;
	my $result = &textIndent;
	
	if(defined $func->{class}) {
		if($func->{virtual}) {
			$result .= 'virtual ';
		}
		else {
			if($func->{staticl} && !$prefixWithClassName) {
				$result .= 'static ';
			}
		}
	}

	$result .= &textParamType($func->{result}, 1);
	
	$result .= ' ';
	
	if($prefixWithClassName && defined $func->{class} && defined $func->{class}->{name}) {
		$result .= $func->{class}->{name} . '::';
	}
	

	$result .= $func->{name} . '(';
	
	for(my $i = 0; $i <= $#{@{$func->{params}}}; ++$i) {
		$result .= ', ' if($i);
		$result .= &textParamType($func->{params}->[$i], 1) . ' ' . $func->{params}->[$i]->{name};
	}
	
	$result .= ')';
	
	return $result;
}

sub textParamType
{
	my ($param, $includeTopLevelCV) = @_;
	my $result = '';
	
	return 'void' unless defined($param);
	
	$result .= 'const ' if($param->{constPointer} && $param->{pointers} || $param->{constReference} && $param->{reference});
	$result .= 'volatile ' if($param->{volatilePointer} && $param->{pointers} || $param->{volatileReference} && $param->{reference});
	
	$result .= $param->{type}->{name};
	for(my $i = 0; $i < $param->{pointers}; ++$i) {
		$result .= ' ' if(!$i);
		$result .= '*';
	}
	$result .= ' &' if($param->{reference});
	
	if($includeTopLevelCV) {
		$result .= ' const' if($param->{const});
		$result .= ' volatile' if($param->{volatile});
	}
	
	return $result;
}

sub textVerifyParam
{
	my ($param) = @_;
	my $result = '';
	
	if($param->{pointers} > 1) {
		--$param->{pointers};
		for(my $i = 0; $i < $configMaxValues; ++$i) {
			$result .= &textIndent . &textAssertEqual($param->{name} ."[$i]", &textCastParam($param, $i)) . ";\n";
		}
		++$param->{pointers};
	}
	else {
		$result .= &textIndent . &textAssertEqual(($param->{pointers} ? '*' : '') . $param->{name}, $param->{valueList}->[0]) . ";\n";
	}
	
	return $result;
}

sub textAssertEqual
{
	my ($value1, $value2) = @_;
	my $assert = $configAssertEqual;
	
	$assert =~ s/x1/$value1/;
	$assert =~ s/x2/$value2/;
	
	return $assert;
}

sub generateParamList
{
	my ($paramIndex, $count) = @_;

	my $params = [];
	my $index = 0;
	
	for(my $i = 0; $i < $count; ++$i) {
		my $p = &getNextParam($paramIndex, $i);
		
		push @$params, $p;
	}
	
	return $params;
};

sub generateFunction
{
	my ($paramIndex, $order, $paramCount, $class) = @_;
	my $result = &getNextParam($paramIndex, 0);
	my $func = {
		name => 'TestFunction' . $order,
		params => &generateParamList($paramIndex, $paramCount),
		result => $result,
		resultValue => &generateParamValue($result),
		virtual => 0,
		static => 0,
		class => $class,
	};
	
	return $func;
}

sub generateClass
{
	my ($order) = @_;
	my $class = {
		name => 'TestObject' . $order,
		functions => [],
		enumName => 'EType',
		super => undef,
		namespace => undef,
	};

	undef($class->{name}) if($order < 0);
	
	my $count = $configCoveredParamCount;
	my %paramIndex = %paramIndexProto;
	
	while($count > 0) {
		my $func = &generateFunction(\%paramIndex, $#{@{$class->{functions}}} + 1, $configMaxParamCount, $class);
		push @{$class->{functions}}, $func;
		$count -= $configMaxParamCount;
	}
	
	return $class;
};

sub generateNamespace
{
	my ($order) = @_;
	my $ns = {
		name => 'TestNS' . $order,
		classes => [],
	};
	
	undef($ns->{name}) if($order < 0);
	
	push @{$ns->{classes}}, &generateClass(0);
	push @{$ns->{classes}}, &generateClass(-1);
	
	return $ns;
}

sub generateParamValue
{
	my ($param) = @_;
	
	return 0 unless defined($param);
	
	return $param->{type}->{generator}($param->{type});
}

sub generateNumeric
{
	my ($type) = @_;
	
	my $result = int($type->{min} + rand($type->{max} + 1 - $type->{min}));
	
	if(&isFloatType($type)) {
		$result += 0.5;
	}
	
	return $result;
}

sub textTest
{
	my ($nsList) = @_;
	my $result = '';
	
	$result .= &textIndent . "void testAutogen() {\n";
	&enterIndent;
		$result .= &textIndent . "testReflectGlobal();\n\n";
		foreach(@{$nsList}) {
			$result .= textTestNamespace($_) . "\n";
		}
	&leaveIndent;
	$result .= &textIndent . "}\n";
	
	return $result;
}

sub textReflectGlobal
{
	my ($nsList) = @_;
	my $result = '';
	
	$result .= &textIndent . "void testReflectGlobal() {\n";
	&enterIndent;
		$result .= &textIndent . "using namespace cpgf;\n\n";
		foreach(@{$nsList}) {
			my $ns = $_;
			foreach(@{$ns->{classes}}) {
				my $class = $_;
				next if(defined $class->{name});
				$result .= &textReflectClass($class);
			}
		}
	&leaveIndent;
	$result .= &textIndent . "}\n";
	
	return $result;
}

sub textIndent
{
	return "\t" x $indent;
}

sub enterIndent
{
	++$indent;
}

sub leaveIndent
{
	--$indent;
}

sub isFloatType
{
	my ($type) = @_;
return 0;
	return $type->{name} =~ /float|double/;
}
