package DoxyXmlLoader;

use strict;
use warnings;

use XML::DOM;
use File::Basename;

use Data::Dumper;

use Util;

use Class;
use Constructor;
use Destructor;
use Param;
use Operator;
use Field;
use Method;
use Enum;
use EnumValue;
use Define;

use FileInfo;


sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		classList => [],
		fileMap => {},

		currentClass => undef,
		currentNamespace => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getVisibility
{
	my ($self, $xmlNode) = @_;

	my $v = Util::getAttribute($xmlNode, 'prot');

	if($v =~ /public/) {
		return 'public';
	}
	elsif($v =~ /private/) {
		return 'private';
	}
	else {
		return 'protected';
	}
}

sub takeVisibility
{
	my ($self, $xmlNode, $item) = @_;

	$item->setVisibility($self->getVisibility($xmlNode));
}

sub getLocation
{
	my ($self, $xmlNode) = @_;

	return Util::getAttribute(Util::getNode($xmlNode, 'location'), 'file');
}

sub takeLocation
{
	my ($self, $xmlNode, $item) = @_;

	$item->setLocation($self->getLocation($xmlNode));
}

sub resolveNamespace
{
	my ($self, $item) = @_;

	if(defined $self->{currentNamespace}) {
		$item->setName($self->{currentNamespace} . '::' . $item->getName);
	}
}

sub fixup
{
	my ($self) = @_;

	$self->{classList} = Util::fixupClassList($self->{classList});
}

sub parseFile
{
	my ($self, $fileName) = @_;

	if(!(-e $fileName)) {
		print "File $fileName doesn't exists.\n";
		return;
	}

	my $parser = new XML::DOM::Parser;
	my $doc = $parser->parsefile ($fileName);
	$self->parse($doc, dirname($fileName));
	$doc->dispose();
}

sub parse
{
	my ($self, $xmlNode, $path) = @_;

	my $root;
	$root = $xmlNode->getElementsByTagName('doxygen')->[0];
	$root = $xmlNode->getElementsByTagName('doxygenindex')->[0] unless defined $root;
	my $childNodeList = $root->getChildNodes();
	foreach(@{$childNodeList}) {
		my $node = $_;
		my $nodeName = $node->getNodeName();

		if($nodeName eq 'compounddef') {
			$self->parseCompounddef($node);
		}
		elsif($nodeName eq 'compound') {
			$self->parseCompound($node, $path);
		}
		else {
		}
	}
};

sub parseCompound
{
	my ($self, $xmlNode, $path) = @_;

	my $refid = Util::getAttribute($xmlNode, 'refid');
	my $fileName = $path . '/' . $refid . '.xml';

	$self->parseFile($fileName);
}

sub parseCompounddef
{
	my ($self, $xmlNode) = @_;

	my $kind = Util::getAttribute($xmlNode, 'kind');
	my $location = $self->getLocation($xmlNode);

	if($kind eq 'class') {
		$self->parseClass($xmlNode, $location);
	}
	elsif($kind eq 'struct') {
		$self->parseClass($xmlNode, $location);
	}
	elsif($kind eq 'file') {
		if(not defined $self->{fileMap}->{$location}) {
			$self->{fileMap}->{$location} = new FileInfo(
				location => $location
			);
		}
		my $fileInfo = $self->{fileMap}->{$location};
		foreach($xmlNode->getElementsByTagName('innernamespace')) {
			my $n = $_;
			&Util::listPush($fileInfo->{namespaceList}, &Util::getNodeText($n));
		}

		$self->parseDefFile($xmlNode, $location);
	}
	elsif($kind eq 'namespace') {
		$self->parseNamespace($xmlNode, $location);
	}
}

sub parseDefFile
{
	my ($self, $xmlNode, $location) = @_;
	
	$self->{currentClass} = new Class;
	Util::listPush($self->{classList}, $self->{currentClass});
	$self->{currentClass}->setLocation($location);

	foreach(@{$xmlNode->getElementsByTagName('sectiondef', 0)}) {
		$self->parseSectiondef($_);
	}
}

sub parseNamespace
{
	my ($self, $xmlNode, $location) = @_;

	$self->{currentNamespace} = Util::getNodeText(Util::getNode($xmlNode, 'compoundname'));
	
	$self->parseDefFile($xmlNode, $location);

	$self->{currentNamespace} = undef;
}

sub parseClass
{
	my ($self, $xmlNode, $location) = @_;

	my $className = Util::getNodeText(Util::getNode($xmlNode, 'compoundname'));

	my $class = new Class(
		_name => $className
	);

	$self->{currentClass} = $class;
	$class->setLocation($location);

	Util::listPush($self->{classList}, $class);

	$self->parseBaseClasses($xmlNode);
	$self->parseInnerClasses($xmlNode);

	foreach(@{$xmlNode->getElementsByTagName('sectiondef', 0)}) {
		$self->parseSectiondef($_);
	}
	
	$self->parseTemplateParams($xmlNode, $class);
}

sub parseBaseClasses
{
	my ($self, $xmlNode) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('basecompoundref', 0)}) {
		my $node = $_;
   		my $name = Util::getNodeText($node) . '~' . $self->getVisibility($node);
   		Util::listPush($self->{currentClass}->{baseNameList}, $name);
   	}
}

sub parseInnerClasses
{
	my ($self, $xmlNode) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('innerclass', 0)}) {
		my $node = $_;
   		my $name = Util::getNodeText($node) . '~' . $self->getVisibility($xmlNode);
   		Util::listPush($self->{currentClass}->{classNameList}, $name);
   	}
}

sub parseSectiondef
{
	my ($self, $xmlNode) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('memberdef', 0)}) {
		my $node = $_;
		my $kind = Util::getAttribute($node, 'kind');
		my $name = Util::getNodeText(Util::getNode($node, 'name'));

		if($kind eq 'function') {
			$self->parseMethod($node, $name);
		}
		elsif($kind eq 'variable') {
			$self->parseField($node, $name);
		}
		elsif($kind eq 'enum') {
			$self->parseEnum($node, $name);
		}
		elsif($kind eq 'define') {
			$self->parseDefine($node, $name);
		}
		elsif($kind eq 'enumvalue') {}
   	}
}

sub parseMethod
{
	my ($self, $xmlNode, $name) = @_;

	if(not $self->{currentClass}->isGlobal()) {
		if($name =~ /~/) {
			my $destructor = new Destructor;
			$self->{currentClass}->setDestructor($destructor);
			$self->takeVisibility($xmlNode, $destructor);
			$self->takeLocation($xmlNode, $destructor);
			$self->resolveNamespace($destructor);

			return;
		}
		
		if(Util::getBaseName($self->{currentClass}->getName) eq $name) { # constructor
			my $constructor = new Constructor;
			$self->parseParams($xmlNode, $constructor);
			$self->parseTemplateParams($xmlNode, $constructor);
			$self->{currentClass}->addConstructor($constructor);
			$self->takeVisibility($xmlNode, $constructor);
			$self->takeLocation($xmlNode, $constructor);
			$self->resolveNamespace($constructor);

			return;
		}
	}

	if($name =~ /^.*\boperator(\b.*)$/) { # operator
		my $op = $1;
		$op =~ s/\s//g;
		my $operator = new Operator(
			_returnType => Util::getNodeText(Util::getNode($xmlNode, 'type')),
			_operator => $op,
			_static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static')),
			_const => Util::valueYesNo(Util::getAttribute($xmlNode, 'const')),
		);
		if($operator->getReturnType eq '') { # type convertion operator, T()
			$operator->setReturnType($operator->getOperator);
		}
		$self->parseParams($xmlNode, $operator);
		$self->parseTemplateParams($xmlNode, $operator);
		Util::listPush($self->{currentClass}->{operatorList}, $operator);
		$self->takeVisibility($xmlNode, $operator);
		$self->takeLocation($xmlNode, $operator);
		$self->resolveNamespace($operator);

		return;
	}

	my $method = new Method(
		_name => $name,
		_returnType => Util::getNodeText(Util::getNode($xmlNode, 'type')),
		_static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static')),
		_const => Util::valueYesNo(Util::getAttribute($xmlNode, 'const')),
		_virtual => ((Util::getAttribute($xmlNode, 'virt') eq 'virtual') ? 1 : 0),
		_pureVirtual => ((Util::getAttribute($xmlNode, 'virt') eq 'pure-virtual') ? 1 : 0),
	);
	$self->parseParams($xmlNode, $method);
	$self->parseTemplateParams($xmlNode, $method);
	Util::listPush($self->{currentClass}->{methodList}, $method);
	$self->takeVisibility($xmlNode, $method);
	$self->takeLocation($xmlNode, $method);
	$self->resolveNamespace($method);
}

sub parseParams
{
	my ($self, $xmlNode, $item) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('param', 0)}) {
		my $node = $_;
		my $param = new Param(
			_name => Util::getNodeText(Util::getNode($node, 'declname')),
			_type => Util::getNodeText(Util::getNode($node, 'type')),
			_defaultValue => Util::getNodeText(Util::getNode($node, 'defval'))
		);
		$item->addParam($param);
	}
}

sub parseTemplateParams
{
	my ($self, $xmlNode, $item) = @_;

	return unless defined $xmlNode;

	$xmlNode = Util::getNode($xmlNode, 'templateparamlist');
	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('param', 0)}) {
		my $node = $_;
		my $param = new Param(
			_name => Util::getNodeText(Util::getNode($node, 'declname')),
			_type => Util::getNodeText(Util::getNode($node, 'type')),
			_defaultValue => Util::getNodeText(Util::getNode($node, 'defval'))
		);
		$item->addTemplateParam($param);
	}
}

sub parseField
{
	my ($self, $xmlNode) = @_;

	my $field = new Field(
		_name => Util::getNodeText(Util::getNode($xmlNode, 'name')),
		_type => Util::getNodeText(Util::getNode($xmlNode, 'type')),
		_static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static'))
	);
	Util::listPush($self->{currentClass}->{fieldList}, $field);
	$self->takeVisibility($xmlNode, $field);
	$self->takeLocation($xmlNode, $field);
	$self->resolveNamespace($field);
}

sub parseEnum
{
	my ($self, $xmlNode) = @_;

	my $enum = new Enum(
		_name => Util::getNodeText(Util::getNode($xmlNode, 'name'))
	);
	Util::listPush($self->{currentClass}->{enumList}, $enum);
	$self->takeVisibility($xmlNode, $enum);
	$self->takeLocation($xmlNode, $enum);
	$self->resolveNamespace($enum);

	foreach(@{$xmlNode->getElementsByTagName('enumvalue', 0)}) {
		my $node = $_;
		my $value = new EnumValue(
			_name => Util::getNodeText(Util::getNode($node, 'name')),
			_value => Util::getNodeText(Util::getNode($node, 'initializer'))
		);
		$enum->addValue($value);
	}
}

sub parseDefine
{
	my ($self, $xmlNode) = @_;

	return if defined Util::getNode($xmlNode, 'param');

	my $define = new Define(
		_name => Util::getNodeText(Util::getNode($xmlNode, 'name')),
		_value => Util::getNodeText(Util::getNode($xmlNode, 'initializer'))
	);
	Util::listPush($self->{currentClass}->{defineList}, $define);
	$self->takeVisibility($xmlNode, $define);
	$self->takeLocation($xmlNode, $define);
}



1;
