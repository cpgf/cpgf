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
		fileMap => undef,
		config => undef,
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub writeHeader
{
	my ($self) = @_;
	
	mkpath(File::Spec->catfile($self->{config}->{outputDir}, ''));

	my $outFileName = $self->makeOutputFileName('.h');
	my $cw = new CodeWriter;

	my $guardName = '__' . uc($self->getDestFileName()) . '_H';
	$guardName =~ s/\./_/g;
	
	$cw->out("//auto generated file, don't modify.\n");
	$cw->out('#ifndef ' . $guardName . "\n");
	$cw->out('#define ' . $guardName . "\n");
	$cw->out("\n\n");

	$cw->out('#include "cpgf/gmetadefine.h"' . "\n");
	$cw->out('#include "cpgf/metadata/gnamereplacer.h"' . "\n");
	$cw->out('#include "cpgf/metadata/gmetadataconfig.h"' . "\n");
	$cw->out('#include "cpgf/metadata/private/gmetadata_header.h"' . "\n");
	$cw->out("\n\n");
			
	my @buildFunctionNameList = ();
	
	foreach(@{$self->{classList}}) {
		my $class = $_;
		my $writer = new MetaClassWriter(
			class => $class,
			codeWriter => $cw,
			config => $self->{config}
		);
		my $className = $self->getGlobalPostfix();
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
	foreach(@{$self->{fileMap}->{namespaceList}}) {
		my $ns = $_;
		$cw->out("using namespace " . $ns . ";\n");
	}
	
	$cw->out("\n\n");
	$cw->out('#include "cpgf/metadata/private/gmetadata_footer.h"' . "\n");
	$cw->out("\n\n");
	$cw->out('#endif');
	
	Util::writeToFile($outFileName, $cw->{text});
}

sub writeSource
{
	my ($self) = @_;

	return unless($self->{config}->{autoRegisterToGlobal});
	
	mkpath(File::Spec->catfile($self->{config}->{cppOutputDir}, ''));

	my $outFileName = File::Spec->catfile($self->{config}->{cppOutputDir}, $self->getDestFileName()) . '.cpp';
	my $cw = new CodeWriter;

	if(defined($self->{config}->{headerCode})) {
		$cw->out($self->{config}->{headerCode});
		$cw->out("\n");
	}
	if(defined($self->{config}->{headerReplacer})) {
		my $fileName = $self->{sourceFileName};
		$fileName =~ s/\\/\//g;
		$fileName = &{$self->{config}->{headerReplacer}}($fileName, $self->getBaseFileName());
		$cw->out('#include "' . $fileName . "\"\n");
		$cw->out("\n");
	}
	$cw->out('#include "' . $self->{config}->{metaHeaderPath} . $self->getDestFileName() . ".h\"\n");
	$cw->out("\n");
	$cw->out('#include "cpgf/gmetapolicy.h"' . "\n");
	$cw->out('#include "cpgf/goutmain.h"' . "\n");
	$cw->out("\n");
	
	my $namespaceSymbol = Util::makeNamespaceSymbol($self->{config});
	$cw->out('extern const char * ' . $namespaceSymbol . ";\n");
	$cw->out("\n");
	
	$cw->out("using namespace cpgf;\n");
	$cw->out("\n");

	$cw->out("namespace {\n");
	$cw->out("\n");

	$cw->out("G_AUTO_RUN_BEFORE_MAIN()\n");
	$cw->out("{\n");
	$cw->incIndent();

	foreach(@{$self->{classList}}) {
		my $class = $_;
		next if($class->{template});

		$cw->out("{\n");
		
		my $className = $self->getGlobalPostfix();
		$className = $class->{name} if(not $class->isGlobal());
		$className = Util::getBaseName($className);
		$className = ucfirst($className);
		
		my $funcName = $self->{config}->{metaClassFunctionPrefix} . $className;
		my $code = $funcName . "(0, _d, NULL, GMetaPolicyCopyAllConstReference());\n";
		
		Util::defineMetaClass($self->{config}, $cw, $class, '_d', 'define', $class->getPolicyRules(), $code);
		
		$cw->out("}\n");
		$cw->out("\n\n");
	}

	$cw->decIndent();
	$cw->out("}\n");
	$cw->out("\n");

	$cw->out("} // unnamed namespace\n");
	$cw->out("\n");
	
	Util::writeToFile($outFileName, $cw->{text});
}

sub invokeMetaFunction
{
	my ($self, $cw, $name) = @_;

	$cw->out($name . "(config, _d, _r, _p);\n");
}

sub beginMetaFunction
{
	my ($self, $cw, $name) = @_;

	$cw->out("template <typename D, typename Policy>\n");
	$cw->out("void " . $name . "(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)\n");
	$cw->out("{\n");
	$cw->incIndent();
	$cw->out("(void)config; (void)_d; (void)_r; (void)_d; (void)_p;\n");
	$cw->out("using namespace cpgf;\n");
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
	my ($self, $extension) = @_;
	
	return File::Spec->catfile($self->{config}->{outputDir}, $self->getDestFileName()) . $extension;
}

sub getGlobalPostfix
{
	my ($self) = @_;
	
	my $g = 'global_' . Util::getBaseFileName(basename($self->{sourceFileName}));
	$g = lc($g);
	$g =~ s/\./_/g;

	return $g;
}

sub getBaseFileName
{
	my ($self) = @_;
	
	return Util::getBaseFileName(basename($self->{sourceFileName}));
}

sub getDestFileName
{
	my ($self) = @_;
	
	return $self->{config}->{sourceFilePrefix} . $self->getBaseFileName();
}


1;
