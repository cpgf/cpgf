package Util;

use strict;
use warnings;

our @EXPORT = qw(
	&fatal
	&findItemByName
	&listPush
	&getBaseName
	&valueYesNo
	&getAttribute
	&dumpClass
);

sub fatal
{
	my @msg = @_;

	print @msg;
	die "\n";
}

sub findItemByName
{
	my ($itemList, $name) = @_;

	foreach(@{$itemList}) {
		my $base = $_;
		if($base->{name} eq $name) {
			return $base;
		}
	}

	return undef;
}

sub listPush
{
	my ($list, $item) = @_;

	push @{$list}, $item;
}

sub getBaseName
{
	my ($name) = @_;

	$name =~ s/^.*\b(\w+)$/$1/;

	return $name;
}

sub valueYesNo
{
	my ($s) = @_;

	return lc($s) eq 'no' ? 0 : 1;
}

sub getNode
{
	my ($xmlNode, $nodeName) = @_;
	
	return "" unless defined $xmlNode;
	my $nodeList = $xmlNode->getElementsByTagName($nodeName, 0);
	return $nodeList->[0];
}

sub getAttribute
{
	my ($xmlNode, $attrName) = @_;

	my $attrNode = $xmlNode->getAttributes()->getNamedItem($attrName);
	return defined $attrNode ? $attrNode->getValue() : undef;
}

sub getNodeText
{
	my ($xmlNode, $attrName) = @_;
	
	return undef unless defined $xmlNode;

	my $result = '';

	foreach($xmlNode->getChildNodes()) {
		my $node = $_;

		if($node->getNodeName() eq '#text') {
			$result .= $node->getData();
		}
		else {
			$result .= getNodeText($node);
		}
	}

	return $result;
}


sub dumpClass
{
	my ($writer, $class) = @_;

	$writer->out("class " . $class->{name});
	$writer->out("\n");
	$writer->out("{\n");

	$writer->addIndent();

	foreach(@{$class->{methodList}}) {
		my $method = $_;
		&dumpMethod($writer, $method);
	}

	$writer->decIndent();

	$writer->out("}\n");

	return $writer->{text};
}

sub dumpMethod
{
	my ($writer, $method) = @_;

	$writer->out($method->{returnType} . ' ' . $method->{name} . '(');
	dumpParamList($writer, $method->{paramList});
	$writer->out(");");
	$writer->out("\n");

	return $writer->{text};
}

sub dumpParamList
{
	my ($writer, $paramList) = @_;
	my $comma = 0;
	foreach(@{$paramList}) {
		my $p = $_;
		$writer->out(', ') if($comma);
		&dumpParam($writer, $p);
		$comma = 1;
	}
}

sub dumpParam
{
	my ($writer, $param) = @_;
	
	$writer->out($param->{type} . ' ' . $param->{name});
}


1;
