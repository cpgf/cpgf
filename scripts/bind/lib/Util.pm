package Util;

use strict;
use warnings;

use Class;

our @EXPORT = qw(
	&fatal
	&findItemByName
	&listPush
	&getBaseName
	&valueYesNo
	&getAttribute

	&getBaseFileName
	&defineMetaClass

	&fixupClassList
	&dumpClass

	&writeParamList
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
		if(defined($base->{name}) and ($base->{name} eq $name)) {
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
	
	return undef unless defined $xmlNode;
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

sub getBaseFileName
{
	my ($fn) = @_;
	
	$fn =~ s/\.[^.]+$//;
	
	return $fn;
}

sub defineMetaClass
{
	my ($codeWriter, $class, $varName, $action) = @_;
	
	if($class->isGlobal()) {
		$codeWriter->out("GDefineMetaGlobal " . $varName . ";\n");
	}
	else {
		my $typeName = "GDefineMetaClass<" . $class->{name};
		foreach(@{$class->{baseNameList}}) {
			my @names = split('~', $_);
			if($names[1] eq 'public') {
				$typeName .= ", " . $names[0];
			}
		}
		$typeName .= ">";
		$codeWriter->out($typeName .  " " . $varName . " = " . $typeName . "::" . $action . "(\"" . Util::getBaseName($class->{name}) . "\");\n");
	}
}

sub itemIsPublic
{
	my ($item) = $_;

	return $item->{visibility} eq 'public';
}

sub itemIsProtected
{
	my ($item) = $_;

	return $item->{visibility} eq 'protected';
}

sub itemIsPrivate
{
	my ($item) = $_;

	return $item->{visibility} eq 'private';
}

sub fixupClassList
{
	my ($classList) = @_;

	$classList = &doFixupGlobals($classList);
	$classList = &doFixupBases($classList);
	$classList = &doFixupInners($classList);

	return $classList;
}

sub doFixupGlobals
{
	my ($classList) = @_;

	my %fileMap = ();
	my $finished = 0;
	while(not $finished) {
		$finished = 1;

		for(my $i = 0; $i <= $#{@{$classList}}; ++$i) {
			my $c = $classList->[$i];
			next unless($c->isGlobal());

			&doFixupGlobalItems(\%fileMap, $c->{fieldList});
			&doFixupGlobalItems(\%fileMap, $c->{methodList});
			&doFixupGlobalItems(\%fileMap, $c->{operatorList});
			&doFixupGlobalItems(\%fileMap, $c->{enumList});
			&doFixupGlobalItems(\%fileMap, $c->{defineList});

			splice(@{$classList}, $i, 1);
			$finished = 0;
			last;
		}
	}

	foreach(values(%fileMap)) {
		unshift @{$classList}, $_;
	}

	return $classList;
}

sub doFixupGlobalItems
{
	my ($fileMap, $itemList) = @_;

	foreach(@{$itemList}) {
		my $item = $_;
		my $location = $item->{location};
		if(not defined $fileMap->{$location}) {
			$fileMap->{$location} = new Class;
			$fileMap->{$location}->{location} = $location;
		}
		&listPush($item->getList($fileMap->{$location}), $item);
	}
}

sub doFixupBases
{
	my ($classList) = @_;

	foreach(@{$classList}) {
		my $target = $_;
		$target->{baseList} = [];

		foreach(@{$target->{baseNameList}}) {
			my @names = split('~', $_);
			my $baseClass = &findItemByName($classList, $names[0]);
			if(defined $baseClass) {
				push @{$target->{baseList}}, $baseClass;
			}
		}
	}

	return $classList;
}

sub doFixupInners
{
	my ($classList) = @_;

	foreach(@{$classList}) {
		my $target = $_;
		$target->{classList} = [];

		foreach(@{$target->{classNameList}}) {
			my @names = split('~', $_);
			my $innerClass = &findItemByName($classList, $names[0]);
			if(defined $innerClass) {
				$innerClass->{inner} = 1;
				push @{$target->{classList}}, $innerClass;
				$innerClass->{visibility} = $names[1];
			}
		}
	}
	
	for(my $i = $#{@{$classList}}; $i >= 0; --$i) {
		my $c = $classList->[$i];
		if($c->{inner}) {
			splice(@{$classList}, $i, 1);
		}
	}

	return $classList;
}

sub mergeClasses
{
	my ($a, $b) = @_;

	$a->{baseList} = &mergeArrays($a->{baseList}, $b->{baseList});
	$a->{constructorList} = &mergeArrays($a->{constructorList}, $b->{constructorList});
	$a->{fieldList} = &mergeArrays($a->{fieldList}, $b->{fieldList});
	$a->{methodList} = &mergeArrays($a->{methodList}, $b->{methodList});
	$a->{enumList} = &mergeArrays($a->{enumList}, $b->{enumList});
	$a->{operatorList} = &mergeArrays($a->{operatorList}, $b->{operatorList});
	$a->{classList} = &mergeArrays($a->{classList}, $b->{classList});

	return $a;
}

sub mergeArrays
{
	my ($a, $b) = @_;

	foreach(@{$b}) {
		push @{$a}, $_;
	}

	return $a;
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
	writeParamList($writer, $method->{paramList});
	$writer->out(");");
	$writer->out("\n");

	return $writer->{text};
}

sub writeParamList
{
	my ($writer, $paramList, $withName) = @_;
	my $comma = 0;
	foreach(@{$paramList}) {
		my $p = $_;
		$writer->out(', ') if($comma);
		&writeParam($writer, $p, $withName);
		$comma = 1;
	}
}

sub writeParam
{
	my ($writer, $param, $withName) = @_;
	
	$writer->out($param->{type} . ($withName ? ' ' . $param->{name} : ''));
}


1;
