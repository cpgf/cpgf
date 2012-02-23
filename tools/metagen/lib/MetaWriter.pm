package MetaWriter;

use strict;
use warnings;

use CppWriter;
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

	return $self->{_config}->{mainSourceFile};
}

sub getMainFunctionName
{
	my ($self) = @_;

	return $self->{_config}->{metaClassMainRegisterPrefix} . $self->{_config}->{projectID};
}

sub createMainHeader
{
	my ($self, $createFunctionNames) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	my $outFileName = File::Spec->catfile($self->{_config}->{headerOutput}, $self->getMainFileName) . '.h';
	my $cw = new CppWriter;

	Util::writeAutoComment($cw);	

	$cw->beginIncludeGuard(Util::normalizeSymbol($self->getMainFileName()) . '_H');

	$cw->include('cpgf/gmetadefine.h');

	$cw->out("\n\n");
	
	$cw->useNamespace("cpgf");
	$cw->out("\n");

	$cw->beginNamespace($self->{_config}->{cppNamespace});
	
	my @sortedCreateFunctionNames = sort(@{$createFunctionNames});

	foreach(@sortedCreateFunctionNames) {
		my $funcName = $_;

		$cw->out("GDefineMetaInfo $funcName();\n");
	}
	
	$cw->out("\n\n");

	$cw->out("template <typename Meta>\n");
	$cw->out("void " . $self->getMainFunctionName . "(Meta _d)\n");

	$cw->beginBlock();

	$cw->out("_d\n");

	$cw->incIndent();

	foreach(@sortedCreateFunctionNames) {
		my $funcName = $_;

		$cw->out("._class($funcName())\n");
	}
	$cw->decIndent();
	$cw->out(";\n");

	$cw->endBlock();
	
	$cw->out("\n");
	
	$cw->endNamespace($self->{_config}->{cppNamespace});

	$cw->endIncludeGuard();	

	Util::writeToFile($outFileName, $cw->getText);
}

sub createMainSource
{
	my ($self, $createFunctionNames) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	my $outFileName = File::Spec->catfile($self->{_config}->{sourceOutput}, $self->getMainFileName) . '.cpp';
	my $cw = new CppWriter;

	Util::writeAutoComment($cw);	

	$cw->include($self->{_config}->{metaHeaderPath} . $self->getMainFileName() . '.h');
	$cw->include('cpgf/gmetadefine.h');
	$cw->include('cpgf/goutmain.h');

	$cw->out("\n\n");
	
	$cw->useNamespace("cpgf");
	$cw->out("\n");

	$cw->beginNamespace($self->{_config}->{cppNamespace});
	
	$cw->beginNamespace('');

	$cw->out("G_AUTO_RUN_BEFORE_MAIN()\n");

	$cw->beginBlock();

	my $global = new Class;
	Util::defineMetaClass($self->{_config}, $cw, $global, '_d', 'define');

	$cw->out($self->getMainFunctionName . "(_d);\n");

	$cw->endBlock();
	$cw->out("\n");

	$cw->endNamespace('');
	
	$cw->endNamespace($self->{_config}->{cppNamespace});

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
