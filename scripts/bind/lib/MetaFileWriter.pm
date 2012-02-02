package MetaFileWriter;

use strict;
use warnings;

use File::Spec;
use File::Basename;
use File::Path;

use Util;
use CodeWriter;
use MetaClassWriter;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		sourceFileName => undef,
		classList => [],
		config => undef,
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub write
{
	my ($self) = @_;
	
	mkpath(File::Spec->catfile($self->{config}->{outputDir}, ''));

	my $outFileName = $self->makeOutputFileName();
	my $cw = new CodeWriter;

	my $guardName = '__' . uc($self->getDestFileName()) . '_H';
	$guardName =~ s/\./_/g;
	
	$cw->out("//auto generated file, don't modify.\n");
	$cw->out('#ifndef ' . $guardName . "\n");
	$cw->out('#define ' . $guardName . "\n");
	$cw->out("\n\n");

	$cw->out('include "cpgf/metadata/gnamereplacer.h"' . "\n");
	$cw->out('include "cpgf/metadata/gmetadataconfig.h"' . "\n");
	$cw->out('include "cpgf/metadata/private/gmetadata_header.h"' . "\n");
	$cw->out("\n\n");
			
	my @buildFunctionNameList = ();
	
	foreach(@{$self->{classList}}) {
		my $class = $_;
		my $writer = new MetaClassWriter(
			class => $class,
			codeWriter => $cw,
			config => $self->{config}
		);
		my $className = 'global';
		$className = $class->{name} if(not $class->isGlobal());
		$className = Util::getBaseName($className);
		$className = ucfirst($className);
		
		my $funcName = $self->{config}->{metaClassFunctionPrefix} . $className;
		push @buildFunctionNameList, $funcName;
		
		$self->beginMetaFunction($cw, $funcName);
		$writer->write();
		$self->endMetaFunction($cw);
		
		$cw->out("\n\n");
	}

	my $fileFuncName = $self->{config}->{metaFileFunctionPrefix} . ucfirst($self->getBaseFileName());
	$fileFuncName =~ s/\./_/g;
	$self->beginMetaFunction($cw, $fileFuncName);
	foreach(@buildFunctionNameList) {
		$self->invokeMetaFunction($cw, $_);
	}
	$self->endMetaFunction($cw);
	
	$cw->out("\n\n");
	$cw->out('include "cpgf/metadata/private/gmetadata_footer.h"' . "\n");
	$cw->out("\n\n");
	$cw->out('#endif');
	
	open FH, '>' . $outFileName or die "Can't write to file $outFileName. \n";
	print FH $cw->{text};
	close FH;
}

sub invokeMetaFunction
{
	my ($self, $cw, $name) = @_;

	$cw->out($name . "(config, _d, _r, _p);\n");
}

sub beginMetaFunction
{
	my ($self, $cw, $name) = @_;

	$cw->out("template <template <typename D, typename Policy>>\n");
	$cw->out("void " . $name . "(const GMetaDataConfigFlags & config, D _d, const GMetaDataNameReplacer * _r, const Policy & _p)\n");
	$cw->out("{\n");
	$cw->incIndent();
	$cw->out("(void)config; (void)_d; (void)_r; (void)_d;\n");
	$cw->out("\n");
}

sub endMetaFunction
{
	my ($self, $cw) = @_;

	$cw->decIndent();
	$cw->out("}\n");
}

sub makeOutputFileName
{
	my ($self) = @_;
	
	return File::Spec->catfile($self->{config}->{outputDir}, $self->getDestFileName()) . '.h';
}

sub getBaseFileName
{
	my ($self) = @_;
	
	my $fn = basename($self->{sourceFileName});
	$fn =~ s/\.[^.]+$//;
	
	return $fn;
}

sub getDestFileName
{
	my ($self) = @_;
	
	return $self->{config}->{sourceFilePrefix} . $self->getBaseFileName();
}


1;
