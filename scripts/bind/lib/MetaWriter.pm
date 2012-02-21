package MetaWriter;

use strict;
use warnings;

use CodeWriter;
use Util;
use MetaFileWriter;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_classList => undef,
		_fileMap => undef,
		_config => undef,
		_fileWriterList => [],
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub write
{
	my ($self) = @_;
	my @createFunctionNames = ();

	$self->buildFileWriterList();

	foreach(@{$self->{_fileWriterList}}) {
		my $fileWriter = $_;
		$fileWriter->writeHeader();
		$fileWriter->writeSource(\@createFunctionNames);
	}

	$self->createMainHeader(\@createFunctionNames);
	$self->createMainSource(\@createFunctionNames);
}

sub getMainFileName
{
	my ($self) = @_;

	return $self->{_config}->{mainSourceFilePrefix} . $self->{_config}->{id};
}

sub getMainFunctionName
{
	my ($self) = @_;

	return $self->{_config}->{metaClassMainRegisterPrefix} . $self->{_config}->{id};
}

sub createMainHeader
{
	my ($self, $createFunctionNames) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	my $outFileName = File::Spec->catfile($self->{_config}->{outputDir}, $self->getMainFileName) . '.h';
	my $cw = new CodeWriter;

	Util::writeAutoComment($cw);	

	my $guardName = '__' . uc(Util::normalizeSymbol($self->getMainFileName())) . '_H';
	$guardName =~ s/\./_/g;

	$cw->out('#ifndef ' . $guardName . "\n");
	$cw->out('#define ' . $guardName . "\n");
	$cw->out("\n\n");

	$cw->out('#include "cpgf/gmetadefine.h"' . "\n");

	$cw->out("\n\n");
	
	$cw->out("using namespace cpgf;\n");
	$cw->out("\n");

	foreach(@{$createFunctionNames}) {
		my $funcName = $_;

		$cw->out("GDefineMetaInfo $funcName();\n");
	}
	
	$cw->out("\n\n");

	$cw->out("template <typename Meta>\n");
	$cw->out("void " . $self->getMainFunctionName . "(Meta _d)\n");
	$cw->out("{\n");
	$cw->incIndent();

	$cw->out("_d\n");

	$cw->incIndent();
	foreach(@{$createFunctionNames}) {
		my $funcName = $_;

		$cw->out("._class($funcName())\n");
	}
	$cw->decIndent();
	$cw->out(";\n");

	$cw->decIndent();
	$cw->out("}\n");
	
	$cw->out("\n\n");
	$cw->out('#endif');

	Util::writeToFile($outFileName, $cw->getText);
}

sub createMainSource
{
	my ($self, $createFunctionNames) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	my $outFileName = File::Spec->catfile($self->{_config}->{cppOutputDir}, $self->getMainFileName) . '.cpp';
	my $cw = new CodeWriter;

	Util::writeAutoComment($cw);	

	$cw->out('#include "' . $self->{_config}->{metaHeaderPath} . $self->getMainFileName() . ".h\"\n");
	$cw->out('#include "cpgf/gmetadefine.h"' . "\n");
	$cw->out('#include "cpgf/goutmain.h"' . "\n");

	$cw->out("\n\n");
	
	$cw->out("using namespace cpgf;\n");
	$cw->out("\n");

	$cw->out("namespace {\n");
	$cw->out("\n");

	$cw->out("G_AUTO_RUN_BEFORE_MAIN()\n");
	$cw->out("{\n");
	$cw->incIndent();

	my $global = new Class;
	Util::defineMetaClass($self->{_config}, $cw, $global, '_d', 'define');

	$cw->out($self->getMainFunctionName . "(_d);\n");

	$cw->decIndent();
	$cw->out("}\n");
	$cw->out("\n");

	$cw->out("} // unnamed namespace\n");
	$cw->out("\n");

	Util::writeToFile($outFileName, $cw->getText);
}

sub buildFileWriterList
{
	my ($self) = @_;
	my %fm = ();
	

	$self->{_fileWriterList} = [];

	foreach(@{$self->{_classList}}) {
		my $item = $_;
		my $location = $item->getLocation;
		my $key = lc($location);

		if(not defined $fm{$key}) {
			$fm{$key} = new MetaFileWriter(
				_sourceFileName => $location,
				_fileMap => $self->{_fileMap}->{$location},
				_config => $self->{_config}
			);
			Util::listPush($self->{_fileWriterList}, $fm{$key});
		}
		$fm{$key}->addClass($item);
	}
}



1;
