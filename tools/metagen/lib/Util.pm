package Util;

use strict;
use warnings;

use Class;

our @EXPORT = qw(
	&fatal
	&fatalAbstract

	&createCallbackParam
	
	&getUniqueID
	
	&assignValues
	
	&getWrittenFileCount
	&getSkippedFileCount

	&writeToFile
	
	&findItemByName
	&listPush
	&getBaseName
	&valueYesNo
	&getAttribute

	&getBaseFileName
	
	&defineMetaClass
	&createMetaClass

	&dumpClass

	&writeParamList
	&writeDefaultParams

	&itemIsPublic
	&itemIsProtected
	&itemIsPrivate

	&writeAutoComment
	&normalizeSymbol

	&sortClassList

	&trace
);

my $currentUniqueID = 0;

my $writtenFileCount = 0;
my $skippedFileCount = 0;

sub getWrittenFileCount { return $writtenFileCount; }
sub getSkippedFileCount { return $skippedFileCount; }

sub fatal
{
	my @msg = @_;

	print @msg;
	die "\n";
}

sub fatalAbstract
{
	my ($methodName) = @_;

	fatal("Invoking abstract method " . ((defined $methodName) ? $methodName : " unknown."));
}

sub createCallbackParam
{
	my $param = {
		skipBind => 0
	};

	return $param
}

sub getUniqueID
{
	++$currentUniqueID;
	return $currentUniqueID;
}

sub assignValues
{
	my ($object, $values) = @_;

	foreach(keys %{$values}) {
		$object->{$_} = $values->{$_};
	}
}

sub writeToFile
{
	my ($fileName, $content) = @_;

	if(open FH, '<' . $fileName) {
		my @lines = <FH>;

		close FH;

		my $oldText = join('', @lines);
		my $temp = $content;

		$temp =~ s/\r//msg;
		$oldText =~ s/\r//msg;

		if($temp eq $oldText) {
			++$skippedFileCount;

#			print "Identical file $fileName ... skipped. \n";

			return;
		}

	}

	++$writtenFileCount;

	print "Write to file $fileName. \n";
	open FH, '>' . $fileName or fatal("Can't write to file $fileName. \n");
	print FH $content;
	close FH;
}

sub findItemByName
{
	my ($itemList, $name) = @_;

	foreach(@{$itemList}) {
		my $base = $_;
		if(defined($base->getName) and ($base->getName eq $name)) {
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

	return undef unless defined $xmlNode;
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
	my ($config, $codeWriter, $class, $varName, $action) = @_;
	my $rules = $class->getPolicyRules();
	
	my $namespace = '"' . ((defined $config->{metaNamespace}) ? $config->{metaNamespace} : '') . '"';
	
	if($class->isGlobal()) {
		if(defined $config->{metaNamespace}) {
			$codeWriter->out('GDefineMetaNamespace ' . $varName . ' = GDefineMetaNamespace::' . $action . '(' . $namespace . ");\n");
		}
		else {
			$codeWriter->out("GDefineMetaGlobal " . $varName . ";\n");
		}
	}
	else {
		my $typeName = "GDefineMetaClass<" . $class->getName;
		foreach(@{$class->getBaseNameList}) {
			my @names = split('~', $_);
			if($names[1] eq 'public') {
				$typeName .= ", " . $names[0];
			}
		}
		$typeName .= ">";
		my $policy = '';
		if(defined($rules) and scalar(@{$rules}) > 0) {
			$policy = '::Policy<MakePolicy<' . join(', ', @{$rules}) . '> >';
		}
		if(defined $config->{metaNamespace}) {
			$codeWriter->out('GDefineMetaNamespace _ns = GDefineMetaNamespace::' . $action . '(' . $namespace . ");\n");
			$codeWriter->out($typeName .  " " . $varName . " = " . $typeName . $policy . "::declare(\"" . Util::getBaseName($class->getName) . "\");\n");
			$codeWriter->out("_ns._class(" . $varName . ");\n");
		}
		else {
			$codeWriter->out($typeName .  " " . $varName . " = " . $typeName . $policy . "::" . $action . "(\"" . Util::getBaseName($class->getName) . "\");\n");
		}
	}
}

sub createMetaClass
{
	my ($config, $codeWriter, $class, $varName, $rules) = @_;
	
	if($class->isGlobal()) {
		$codeWriter->out("GDefineMetaGlobalDangle $varName = GDefineMetaGlobalDangle::dangle();\n");
	}
	else {
		my $typeName = "GDefineMetaClass<" . $class->getName;
		foreach(@{$class->getBaseNameList}) {
			my @names = split('~', $_);
			if($names[1] eq 'public') {
				$typeName .= ", " . $names[0];
			}
		}
		$typeName .= ">";
		my $policy = '';
		if(defined($rules) and scalar(@{$rules}) > 0) {
			$policy = '::Policy<MakePolicy<' . join(', ', @{$rules}) . '> >';
		}
		$codeWriter->out($typeName .  " " . $varName . " = " . $typeName . $policy . "::declare(\"" . Util::getBaseName($class->getName) . "\");\n");
	}
}

sub itemIsPublic
{
	my ($item) = @_;

	return $item->getVisibility eq 'public';
}

sub itemIsProtected
{
	my ($item) = @_;

	return $item->getVisibility eq 'protected';
}

sub itemIsPrivate
{
	my ($item) = @_;

	return $item->getVisibility eq 'private';
}

sub mergeClasses
{
	my ($a, $b) = @_;

	&mergeArrays($a->getBaseList, $b->getBaseList);
	&mergeArrays($a->getConstructorList, $b->getConstructorList);
	&mergeArrays($a->getFieldList, $b->getFieldList);
	&mergeArrays($a->getMethodList, $b->getMethodList);
	&mergeArrays($a->getEnumList, $b->getEnumList);
	&mergeArrays($a->getOperatorList, $b->getOperatorList);
	&mergeArrays($a->getClassList, $b->getClassList);

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

sub writeDefaultParams
{
	my ($writer, $paramList) = @_;
	my $index = scalar(@{$paramList}) - 1;

	if($index >= 0 && $paramList->[$index]->hasDefaultValue) {
		$writer->out("\n");
		$writer->incIndent;

		while($index >= 0 && $paramList->[$index]->hasDefaultValue) {
			$writer->out("._default(copyVariantFromCopyable(" . $paramList->[$index]->getDefaultValue . "))\n");
			--$index;
		}

		$writer->decIndent;
	}
	$writer->out(";\n");
}

sub writeParam
{
	my ($writer, $param, $withName) = @_;
	
	$writer->out($param->getType . ($withName ? ' ' . $param->getName : ''));
}

sub writeAutoComment
{
	my ($writer) = @_;
	
	$writer->out("// Auto generated file, don't modify.\n");
	$writer->out("\n");
}

sub normalizeSymbol
{
	my ($s) = @_;
	
	$s =~ s/\./_/g;

	return $s;
}

sub sortClassList
{
	my ($classList) = @_;

	my @sortedArray = sort {
		return -1 unless defined $a->getName;
		return 1 unless defined $b->getName;

		return uc($a->getName) cmp uc($b->getName);
	}
	@{$classList};

	my $newList = [];

	foreach(@sortedArray) {
		listPush($newList, $_);
	}
	
	return $newList;
}


sub trace
{
	my ($message) = @_;

	print $message, "\n";
}


1;
