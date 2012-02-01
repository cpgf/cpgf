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


sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		classList => [],

		currentClass => undef,

		globalClass => new Class,

		%args
	};

	Util::listPush($self->{classList}, $self->{globalClass});

	bless $self, $class;

	return $self;
}

sub checkVisibility
{
	my ($self, $visibility) = @_;
return 1;
	return $visibility eq 'public';
}

sub checkNodeVisibility
{
	my ($self, $xmlNode) = @_;

	return $self->checkVisibility(Util::getAttribute($xmlNode, 'prot'));
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

	return unless $self->checkNodeVisibility($xmlNode);

	my $refid = Util::getAttribute($xmlNode, 'refid');
	my $fileName = $path . '/' . $refid . '.xml';

	$self->parseFile($fileName);
}

sub parseCompounddef
{
	my ($self, $xmlNode) = @_;

	return unless $self->checkNodeVisibility($xmlNode);

	my $kind = Util::getAttribute($xmlNode, 'kind');

	if($kind eq 'class') {
		$self->parseClass($xmlNode);
	}
	elsif($kind eq 'file') {
		$self->parseDefFile($xmlNode);
	}
	elsif($kind eq 'namespace') {
		$self->parseNamespace($xmlNode);
	}
}

sub parseDefFile
{
	my ($self, $xmlNode) = @_;
	
	$self->{currentClass} = $self->{globalClass};

	foreach(@{$xmlNode->getElementsByTagName('sectiondef', 0)}) {
		$self->parseSectiondef($_);
	}
}

sub parseNamespace
{
	my ($self, $xmlNode) = @_;

	$self->parseDefFile($xmlNode);
}

sub parseClass
{
	my ($self, $xmlNode) = @_;

	my $className = Util::getNodeText($xmlNode->getElementsByTagName('compoundname', 0)->[0]);

	my $class = new Class(
		name => $className
	);

	$self->{currentClass} = $class;

	Util::listPush($self->{classList}, $class);

	$self->parseBaseClasses($xmlNode);
	$self->parseInnerClasses($xmlNode);

	foreach(@{$xmlNode->getElementsByTagName('sectiondef', 0)}) {
		$self->parseSectiondef($_);
	}
}

sub getMemebers
{
	my ($self, $scope) = @_;

	return undef unless defined $scope;

	return $scope->{members};
}

sub parseBaseClasses
{
	my ($self, $xmlNode) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('basecompoundref', 0)}) {
		my $node = $_;
		next unless $self->checkNodeVisibility($node);
   		my $name = Util::getNodeText($node);
   		Util::listPush($self->{currentClass}->{baseNameList}, $name);
   	}
}

sub parseInnerClasses
{
	my ($self, $xmlNode) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('innerclass', 0)}) {
		my $node = $_;
		next unless $self->checkNodeVisibility($node);
   		my $name = Util::getNodeText($node);
   		Util::listPush($self->{currentClass}->{classNameList}, $name);
   	}
}

sub parseSectiondef
{
	my ($self, $xmlNode) = @_;

	return unless defined $xmlNode;

	foreach(@{$xmlNode->getElementsByTagName('memberdef', 0)}) {
		my $node = $_;
		next unless $self->checkNodeVisibility($node);
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
			Util::listPush($self->{currentClass}->{constructorList}, $constructor);

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
		Util::listPush($self->{currentClass}->{operatorList}, $operator);

		return;
	}

	my $method = new Method(
		name => $name,
		returnType => Util::getNodeText(Util::getNode($xmlNode, 'type')),
		static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static')),
		virtual => ((Util::getAttribute($xmlNode, 'virt') eq 'virtual') ? 1 : 0),
	);
	$self->parseParams($xmlNode, $method->{paramList});
	Util::listPush($self->{currentClass}->{methodList}, $method);
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

sub parseField
{
	my ($self, $xmlNode) = @_;

	my $field = new Field(
		name => Util::getNodeText(Util::getNode($xmlNode, 'name')),
		type => Util::getNodeText(Util::getNode($xmlNode, 'type')),
		static => Util::valueYesNo(Util::getAttribute($xmlNode, 'static'))
	);
	Util::listPush($self->{currentClass}->{fieldList}, $field);
}

sub parseEnum
{
	my ($self, $xmlNode) = @_;

	my $enum = new Enum(
		name => Util::getNodeText(Util::getNode($xmlNode, 'name'))
	);
	Util::listPush($self->{currentClass}->{enumList}, $enum);

	foreach(@{$xmlNode->getElementsByTagName('enumvalue', 0)}) {
		my $node = $_;
		my $value = new EnumValue(
			name => Util::getNodeText(Util::getNode($node, 'name')),
			value => Util::getNodeText(Util::getNode($node, 'initializer'))
		);
		Util::listPush($enum->{valueList}, $value);
	}
}


1;
