package DoxyXmlLoader;

use strict;
use warnings;

use XML::DOM;
use File::Basename;

use Data::Dumper;

use Util;

use Class;
use Constructor;
use Param;
use Operator;
use Field;
use Method;
use Enum;
use EnumValue;
use Define;


sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		classList => [],

		currentClass => undef,

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

	$item->{visibility} = $self->getVisibility($xmlNode);
}

sub getLocation
{
	my ($self, $xmlNode) = @_;

	return Util::getAttribute(Util::getNode($xmlNode, 'location'), 'file');
}

sub takeLocation
{
	my ($self, $xmlNode, $item) = @_;

	$item->{location} = $self->getLocation($xmlNode);
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
	elsif($kind eq 'file') {
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
	$self->{currentClass}->{location} = $location;

	foreach(@{$xmlNode->getElementsByTagName('sectiondef', 0)}) {
		$self->parseSectiondef($_);
	}
}

sub parseNamespace
{
	my ($self, $xmlNode, $location) = @_;

	$self->parseDefFile($xmlNode, $location);
}

sub parseClass
{
	my ($self, $xmlNode, $location) = @_;

	my $className = Util::getNodeText($xmlNode->getElementsByTagName('compoundname', 0)->[0]);

	my $class = new Class(
		name => $className
	);

	$self->{currentClass} = $class;
	$class->{location} = $location;

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
   		my $name = Util::getNodeText($node) . '~' . $self->getVisibility($xmlNode);
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
		my $name = Util::getNodeText($node->getElementsByTagName('name', 0)->[0]);

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
		if(Util::getBaseName($self->{currentClass}->{name}) eq $name) { # constructor
			my $constructor = new Constructor;
			$self->parseParams($xmlNode, $constructor->{paramList});
			$self->parseTemplateParams($xmlNode, $constructor);
			Util::listPush($self->{currentClass}->{constructorList}, $constructor);
			$self->takeVisibility($xmlNode, $constructor);
			$self->takeLocation($xmlNode, $constructor);

			return;
		}
	}

	if($name =~ /^.*\boperator(\b.*)$/) { # operator
		my $op = $1;
		my $operator = new Operator(
			returnType => Util::getNodeText(Util::getNode($xmlNode, 'type')),
			operator => $op
		);
		$self->parseParams($xmlNode, $operator->{paramList});
		$self->parseTemplateParams($xmlNode, $operator);
		Util::listPush($self->{currentClass}->{operatorList}, $operator);
		$self->takeVisibility($xmlNode, $operator);
		$self->takeLocation($xmlNode, $operator);

		return;
	}

	my $method = new Method(
		name => $name,
		returnType => Util::getNodeText(Util::getNode($xmlNode, 'type')),
		static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static')),
		virtual => ((Util::getAttribute($xmlNode, 'virt') eq 'virtual') ? 1 : 0),
	);
	$self->parseParams($xmlNode, $method->{paramList});
	$self->parseTemplateParams($xmlNode, $method);
	Util::listPush($self->{currentClass}->{methodList}, $method);
	$self->takeVisibility($xmlNode, $method);
	$self->takeLocation($xmlNode, $method);
}

sub parseParams
{
	my ($self, $xmlNode, $params) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('param', 0)}) {
		my $node = $_;
		my $param = new Param(
			name => Util::getNodeText(Util::getNode($node, 'declname')),
			type => Util::getNodeText(Util::getNode($node, 'type')),
			defaultValue => Util::getNodeText(Util::getNode($node, 'defval'))
		);
		Util::listPush($params, $param);
	}
}

sub parseTemplateParams
{
	my ($self, $xmlNode, $item) = @_;

	$item->{template} = 0;

	return unless defined $xmlNode;

	$xmlNode = Util::getNode($xmlNode, 'templateparamlist');
	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('param', 0)}) {
		my $node = $_;
		my $param = new Param(
			name => Util::getNodeText(Util::getNode($node, 'declname')),
			type => Util::getNodeText(Util::getNode($node, 'type')),
			defaultValue => Util::getNodeText(Util::getNode($node, 'defval'))
		);
		Util::listPush($item->{templateParamList}, $param);
		$item->{template} = 1;
	}
}

sub parseField
{
	my ($self, $xmlNode) = @_;

	my $field = new Field(
		name => Util::getNodeText(Util::getNode($xmlNode, 'name')),
		type => Util::getNodeText(Util::getNode($xmlNode, 'type')),
		static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static'))
	);
	Util::listPush($self->{currentClass}->{fieldList}, $field);
	$self->takeVisibility($xmlNode, $field);
	$self->takeLocation($xmlNode, $field);
}

sub parseEnum
{
	my ($self, $xmlNode) = @_;

	my $enum = new Enum(
		name => Util::getNodeText(Util::getNode($xmlNode, 'name'))
	);
	Util::listPush($self->{currentClass}->{enumList}, $enum);
	$self->takeVisibility($xmlNode, $enum);
	$self->takeLocation($xmlNode, $enum);

	foreach(@{$xmlNode->getElementsByTagName('enumvalue', 0)}) {
		my $node = $_;
		my $value = new EnumValue(
			name => Util::getNodeText(Util::getNode($node, 'name')),
			value => Util::getNodeText(Util::getNode($node, 'initializer'))
		);
		Util::listPush($enum->{valueList}, $value);
	}
}

sub parseDefine
{
	my ($self, $xmlNode) = @_;

	return if defined Util::getNode($xmlNode, 'param');

	my $define = new Define(
		name => Util::getNodeText(Util::getNode($xmlNode, 'name')),
		value => Util::getNodeText(Util::getNode($xmlNode, 'initializer'))
	);
	Util::listPush($self->{currentClass}->{defineList}, $define);
	$self->takeVisibility($xmlNode, $define);
	$self->takeLocation($xmlNode, $define);
}



1;
