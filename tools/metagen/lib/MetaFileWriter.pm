package MetaFileWriter;

use strict;
use warnings;

use File::Spec;
use File::Basename;
use File::Path;

use Util;
use CppWriter;
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
	
	my $cw = new CppWriter;

	Util::writeAutoComment($cw);	

	$cw->beginIncludeGuard(Util::normalizeSymbol($self->getDestFileName()) . '_H');

	$cw->include('cpgf/gmetadefine.h');
	$cw->include('cpgf/metadata/gnamereplacer.h');
	$cw->include('cpgf/metadata/gmetadataconfig.h');
	$cw->include('cpgf/metadata/private/gmetadata_header.h');
	$cw->out("\n\n");
			
	my @buildFunctionNameList = ();

	$cw->beginNamespace($self->{_config}->{cppNamespace});

	my $sortedClassList = Util::sortClassList($self->{_classList});
	foreach(@{$sortedClassList}) {
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

	$cw->endNamespace($self->{_config}->{cppNamespace});
	
	foreach(@{$self->{_fileMap}->getNamespaceList}) {
		my $ns = $_;
		$cw->useNamespace($ns);
	}
	
	$cw->out("\n\n");
	$cw->include('cpgf/metadata/private/gmetadata_footer.h');
	
	$cw->endIncludeGuard();
	
	mkpath(File::Spec->catfile($self->{_config}->{headerOutput}, ''));
	my $outFileName = $self->makeOutputFileName($self->{_config}->{headerExtension});
	Util::trace("Writing header $outFileName.");
	Util::writeToFile($outFileName, $cw->getText);
}

sub writeSource
{
	my ($self, $createFunctionNames) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	my $cw = new CppWriter;

	Util::writeAutoComment($cw);	

	if(defined($self->{_config}->{sourceHeaderCode})) {
		$cw->out($self->{_config}->{sourceHeaderCode});
		$cw->out("\n");
	}
	if(defined($self->{_config}->{sourceHeaderReplacer})) {
		my $fileName = $self->{_sourceFileName};
		$fileName =~ s/\\/\//g;
		$fileName = &{$self->{_config}->{sourceHeaderReplacer}}($fileName, $self->getBaseFileName());
		$cw->include($fileName);
		$cw->out("\n");
	}
	$cw->include($self->{_config}->{metaHeaderPath} . $self->getDestFileName() . '.h');
	$cw->out("\n");
	$cw->include('cpgf/gmetapolicy.h');
	$cw->out("\n");
	
	$cw->useNamespace("cpgf");
	$cw->out("\n");

	$cw->beginNamespace($self->{_config}->{cppNamespace});
	
	my $sortedClassList = Util::sortClassList($self->{_classList});
	foreach(@{$sortedClassList}) {
		my $class = $_;
		next if($class->isTemplate);

		my $funcName = $self->createFunctionName($class, $self->{_config}->{metaClassCreatePrefix});
		Util::listPush($createFunctionNames, $funcName);

		$cw->out("GDefineMetaInfo $funcName()\n");
		
		$cw->beginBlock();
		
		Util::createMetaClass($self->{_config}, $cw, $class, '_d', $class->getPolicyRules());
		
		my $callFunc = $self->createFunctionName($class, $self->{_config}->{metaClassFunctionPrefix});
		$cw->out("$callFunc(0, _d, NULL, GMetaPolicyCopyAllConstReference());\n");
		$cw->out("return _d.getMetaInfo();\n");
		
		$cw->endBlock();

		$cw->out("\n\n");
	}
	
	$cw->endNamespace($self->{_config}->{cppNamespace});
	
	mkpath(File::Spec->catfile($self->{_config}->{sourceOutput}, ''));
	my $outFileName = File::Spec->catfile($self->{_config}->{sourceOutput}, $self->getDestFileName()) . $self->{_config}->{sourceExtension};
	Util::trace("Writing source $outFileName.");
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
	$cw->beginBlock();
	$cw->out("(void)config; (void)_d; (void)_r; (void)_d; (void)_p;\n");
	$cw->useNamespace("cpgf");
	$cw->out("\n");
}

sub endMetaFunction
{
	my ($self, $cw) = @_;

	$cw->endBlock();
}

sub makeOutputFileName
{
	my ($self, $extension) = @_;
	
	return File::Spec->catfile($self->{_config}->{headerOutput}, $self->getDestFileName()) . $extension;
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
