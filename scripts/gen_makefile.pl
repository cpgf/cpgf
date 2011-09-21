use strict;
use warnings;

my @sourceFiles = qw (
	tl.cpp
	
	cpgf/src/gmetatype.cpp
	cpgf/src/gmetaannotation.cpp
	cpgf/src/gmetaclass.cpp
	cpgf/src/gmetacommon.cpp
	cpgf/src/gmetaconverter.cpp
	cpgf/src/gmetaenum.cpp
	cpgf/src/gmetafield.cpp
	cpgf/src/gmetafundamental.cpp
	cpgf/src/gmetamethod.cpp
	cpgf/src/gmetaoperator.cpp
	cpgf/src/gmetaproperty.cpp
	cpgf/src/gmetaapiservice.cpp
	cpgf/src/gmetaapiutil.cpp
	cpgf/src/gmetatypereg.cpp

	cpgf/src/testcase/testreflection_main.cpp

	cpgf/src/testcase/reflection/helloworld.cpp
	
	cpgf/src/testcase/reflection/operator_arithmetic.cpp
	cpgf/src/testcase/reflection/operator_arithmetic_assign.cpp
	cpgf/src/testcase/reflection/operator_logic.cpp
	cpgf/src/testcase/reflection/operator_bitwise.cpp
	cpgf/src/testcase/reflection/operator_bitwise_assign.cpp
	cpgf/src/testcase/reflection/operator_unary.cpp
	cpgf/src/testcase/reflection/operator_other.cpp
	
	cpgf/src/testcase/reflection/annotation.cpp
	cpgf/src/testcase/reflection/class.cpp
	cpgf/src/testcase/reflection/constructor.cpp
	cpgf/src/testcase/reflection/enum.cpp
	cpgf/src/testcase/reflection/field.cpp
	cpgf/src/testcase/reflection/global.cpp
	cpgf/src/testcase/reflection/inheritance_single.cpp
	cpgf/src/testcase/reflection/inheritance_multiple.cpp
	cpgf/src/testcase/reflection/method.cpp
	cpgf/src/testcase/reflection/metatype.cpp
	cpgf/src/testcase/reflection/nestedclass.cpp
	cpgf/src/testcase/reflection/property.cpp
	cpgf/src/testcase/reflection/template.cpp
);

my $target = 'a.exe';
my $objPath = 'obj/';

my $buildTarget = 'g++ -Id:/test/test/cpgf/include/ -ID:/Qt/2010.05/mingw/include/ $(OBJ_FILES)';
my $buildObj = 'g++ -c -Id:/test/test/cpgf/include/ -ID:/Qt/2010.05/mingw/include/ -o $@ $<';

#my $objDepend = 'cpgf/include/gmetaclass.h cpgf/include/gmetafield.h  cpgf/include/gmetamethod.h  cpgf/include/gmetaproperty.h  cpgf/include/gmetaannotation.h  cpgf/include/gmetaenum.h  cpgf/include/gmetaoperator.h';
my $objDepend = '';


&doMain;

sub outputLine
{
	print @_, "\n";
}

sub extractFileName
{
	my ($fullPath) = @_;

	$fullPath =~ s/.*\///;

	return $fullPath;
}

sub replaceFileExt
{
	my ($fn, $ext) = @_;

	$fn =~ s/\.[^.]*$/.$ext/;

	return $fn;
}

sub doMain
{
	&outputLine('OBJ_FILES = \\');

	my $s;

	for(my $i = 0; $i <= $#sourceFiles; ++$i) {
		$s = "\t" . $objPath . &replaceFileExt(&extractFileName($sourceFiles[$i]), 'o');
		$s .= ' \\' if($i < $#sourceFiles);
		&outputLine($s);
	}
	&outputLine("");

	&outputLine('a.exe : $(OBJ_FILES)');
	&outputLine("\t", $buildTarget);
	&outputLine("");

	for(my $i = 0; $i <= $#sourceFiles; ++$i) {
		my $fn = &extractFileName($sourceFiles[$i]);
		$s = $objPath . &replaceFileExt($fn, 'o') . " : " . $sourceFiles[$i] . ' ' . $objDepend;
		&outputLine($s);
		&outputLine("\t", $buildObj);
		&outputLine("");
	}

}
