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
		_sourceFileName => undef,
		_classList => [],
		_fileMap => undef,
		_config => undef,
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub addClass { my ($self, $v) = @_; Util::listPush($self->{_classList}, $v); }

sub writeHeader
{
	my ($self) = @_;
	
	mkpath(File::Spec->catfile($self->{_config}->{outputDir}, ''));

	my $outFileName = $self->makeOutputFileName($self->{_config}->{headerExtension});
	my $cw = new CodeWriter;

	Util::writeAutoComment($cw);	

	my $guardName = '__' . uc(Util::normalizeSymbol($self->getDestFileName())) . '_H';
	$guardName =~ s/\./_/g;

	$cw->out('#ifndef ' . $guardName . "\n");
	$cw->out('#define ' . $guardName . "\n");
	$cw->out("\n\n");

	$cw->out('#include "cpgf/gmetadefine.h"' . "\n");
	$cw->out('#include "cpgf/metadata/gnamereplacer.h"' . "\n");
	$cw->out('#include "cpgf/metadata/gmetadataconfig.h"' . "\n");
	$cw->out('#include "cpgf/metadata/private/gmetadata_header.h"' . "\n");
	$cw->out("\n\n");
			
	my @buildFunctionNameList = ();
	
	foreach(@{$self->{_classList}}) {
		my $class = $_;
		my $writer = new MetaClassWriter(
			_class => $class,
			_codeWriter => $cw,
			_config => $self->{_config}
		);
		
		my $funcName = $self->createFunctionName($class, $self->{_config}->{metaClassFunctionPrefix});

		push @buildFunctionNameList, $funcName;
		
		$self->beginMetaFunction($cw, $funcName);
		$writer->write();
		$self->endMetaFunction($cw);
		
		$cw->out("\n\n");
	}

	foreach(@{$self->{_fileMap}->getNamespaceList}) {
		my $ns = $_;
		$cw->out("using namespace " . $ns . ";\n");
	}
	
	$cw->out("\n\n");
	$cw->out('#include "cpgf/metadata/private/gmetadata_footer.h"' . "\n");
	$cw->out("\n\n");
	$cw->out('#endif');
	
	Util::writeToFile($outFileName, $cw->getText);
}

sub writeSource
{
	my ($self, $createFunctionNames) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	mkpath(File::Spec->catfile($self->{_config}->{cppOutputDir}, ''));

	my $outFileName = File::Spec->catfile($self->{_config}->{cppOutputDir}, $self->getDestFileName()) . $self->{_config}->{sourceExtension};
	my $cw = new CodeWriter;

	Util::writeAutoComment($cw);	

	if(defined($self->{_config}->{sourceHeaderCode})) {
		$cw->out($self->{_config}->{sourceHeaderCode});
		$cw->out("\n");
	}
	if(defined($self->{_config}->{sourceHeaderReplacer})) {
		my $fileName = $self->{_sourceFileName};
		$fileName =~ s/\\/\//g;
		$fileName = &{$self->{_config}->{sourceHeaderReplacer}}($fileName, $self->getBaseFileName());
		$cw->out('#include "' . $fileName . "\"\n");
		$cw->out("\n");
	}
	$cw->out('#include "' . $self->{_config}->{metaHeaderPath} . $self->getDestFileName() . ".h\"\n");
	$cw->out("\n");
	$cw->out('#include "cpgf/gmetapolicy.h"' . "\n");
	$cw->out("\n");
	
	$cw->out("using namespace cpgf;\n");
	$cw->out("\n");

	foreach(@{$self->{_classList}}) {
		my $class = $_;
		next if($class->isTemplate);

		my $funcName = $self->createFunctionName($class, $self->{_config}->{metaClassCreatePrefix});
		Util::listPush($createFunctionNames, $funcName);

		$cw->out("GDefineMetaInfo $funcName()\n");
		$cw->out("{\n");
		
		$cw->incIndent();
		
		Util::createMetaClass($self->{_config}, $cw, $class, '_d', $class->getPolicyRules());
		
		my $callFunc = $self->createFunctionName($class, $self->{_config}->{metaClassFunctionPrefix});
		$cw->out("$callFunc(0, _d, NULL, GMetaPolicyCopyAllConstReference());\n");
		$cw->out("return _d.getMetaInfo();\n");
		
		$cw->decIndent();

		$cw->out("}\n");
		$cw->out("\n\n");
	}
	
	Util::writeToFile($outFileName, $cw->getText);
}

sub createFunctionName
{
	my ($self, $class, $prefix) = @_;

	my $className = $self->getGlobalPostfix();
	$className = $class->getName if(not $class->isGlobal());
	$className = Util::getBaseName($className);
	$className = ucfirst($className);
		
	return $prefix . $className;
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
	
	return File::Spec->catfile($self->{_config}->{outputDir}, $self->getDestFileName()) . $extension;
}

sub getGlobalPostfix
{
	my ($self) = @_;
	
	my $g = 'global_' . $self->getFileNameSymbol;
	$g = lc($g);

	return $g;
}

sub getFileNameSymbol
{
	my ($self) = @_;
	
	my $s = Util::getBaseFileName(basename($self->{_sourceFileName}));
	$s =~ s/\./_/g;

	return $s;
}

sub getBaseFileName
{
	my ($self) = @_;
	
	return Util::getBaseFileName(basename($self->{_sourceFileName}));
}

sub getDestFileName
{
	my ($self) = @_;
	
	return $self->{_config}->{sourceFilePrefix} . $self->getBaseFileName();
}


1;
