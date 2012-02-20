package MetaClassWriter;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		class => undef,
		codeWriter => undef,
		config => undef,
		
		define => '_d',
		classType => 'D::ClassType',

		%args
	};

	bless $self, $class;

	return $self;
}

sub getScopePrefix
{
	my ($self, $prefix) = @_;

	$prefix = '' unless defined $prefix;

	if($self->{class}->isGlobal()) {
		return '';
	}
	else {
		return $prefix . $self->{classType} . '::';
	}
}

sub write
{
	my ($self) = @_;

	$self->writeConstructor();
	$self->writeField();
	$self->writeMethod();
	$self->writeEnum();
	$self->writeDefine();
	$self->writeOperator();
	$self->writeClass();
}

sub writeConstructor
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $action = $self->getAction("_constructor");

	return if($self->{class}->isGlobal());
	return if($self->{class}->isAbstract());

	foreach(@{$self->{class}->getConstructorList}) {
		my $item = $_;
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<void * (");
		Util::writeParamList($cw, $item->getParamList, 0);
		$cw->out(")>(_p);\n");
	}
}

sub writeField
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_field");

	foreach(@{$self->{class}->{fieldList}}) {
		my $item = $_;
		my $name = $item->getName;
		
		next unless($self->canWrite($item));
		
		next if($name =~ /\@/ or $name eq ''); # anonymous union
		
		$cw->out($action);
		$cw->out('(' . $self->getReplace($name) . ", ");
		$cw->out("&" . $prefix . $name . ", _p);\n");
	}
}

sub writeMethod
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_method");

	my %methodOverload = ();

	foreach(@{$self->{class}->{methodList}}) {
		my $item = $_;
		my $name = $item->getName;
		++$methodOverload{$name};
	}

	foreach(@{$self->{class}->{methodList}}) {
		my $item = $_;
		my $name = $item->getName;
		my $overload = $methodOverload{$name} > 1;
		
		next if($item->isTemplate);
		next unless($self->canWrite($item));
		
		$overload = $overload || $self->{class}->isGlobal();

		$cw->out($action);
		$cw->out('(' . $self->getReplace($name) . ", ");
		if($overload) {
			$cw->out("(" . $item->getReturnType . " (" . $prefix . "*) (");
			Util::writeParamList($cw, $item->getParamList, 0);
			$cw->out(")");
			if(!$item->isStatic and $item->isConst) {
				$cw->out(" const");
			}
			$cw->out(")");
		}
		$cw->out("&" . $prefix . $name . ", _p);\n");
	}
}

sub writeEnum
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $typePrefix = $self->getScopePrefix('typename ');
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_enum");

	foreach(@{$self->{class}->{enumList}}) {
		my $item = $_;
		my $name = $item->getName;
		
		next unless($self->canWrite($item));

		my $typeName = $typePrefix . $name;

		if($name =~ /\@/ or $name eq '') {
			$name = 'GlobalEnum_'  . $self->{config}->{id} . "_" . Util::getUniqueID();
			$typeName = 'long long';
		}
		
		$cw->out($action . "<" . $typeName . '>(' . $self->getReplace($name) . ")\n");
		$cw->incIndent();
			foreach(@{$item->getValueList}) {
				my $value = $_;
				my $n = $value->getName;
				$cw->out('._element(' . $self->getReplace($n) . ', ' . $prefix . $n . ")\n");
			}
		$cw->decIndent();
		$cw->out(";\n");
	}
}

sub writeDefine
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_enum");

	return if($#{@{$self->{class}->{defineList}}} < 0);

	$cw->out($action . "<long long>(" . $self->getReplace("GlobalDefine_" . $self->{config}->{id} . "_" . Util::getUniqueID()) . ")\n");
	$cw->incIndent();
	
	foreach(@{$self->{class}->{defineList}}) {
		my $item = $_;
		
		my $value = $item->getValue;
		if((not defined $value) or $value eq '') {
			next;
		}
		
		$cw->out('._element(' . $self->getReplace($item->getName) . ', ' . $item->getName . ")\n");
	}
	
	$cw->decIndent();
	$cw->out(";\n");
}

sub writeOperator
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_operator");

	foreach(@{$self->{class}->{operatorList}}) {
		my $item = $_;
		my $name = $item->getName;
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<" . $item->getReturnType . " (*)(");
		
		my $op = $item->getOperator;
		
		my $isStatic = ($self->{class}->isGlobal() or $item->isStatic);
		my $isFunctor = $op eq '()';
		my $hasSelf = 0;
		
		if(not $isFunctor) {
			if(not $isStatic) {
				if($item->isConst) {
					$cw->out('const cpgf::GMetaSelf &');
				}
				else {
					$cw->out('cpgf::GMetaSelf');
				}
				
				$hasSelf = 1;
			}
		}
		my $opText = '';
		if($op eq '++' or $op eq '--') {
		}
		else {
			if($item->hasParam and $hasSelf) {
				$cw->out(', ');
			}
			Util::writeParamList($cw, $item->getParamList, 0);
		}
		$cw->out(")>(");
		my $realParamCount = $item->getParamCount;
		if(not $isStatic) {
			++$realParamCount;
		}
		if($isFunctor) {
			$opText = 'H(H)';
		}
		elsif($op eq '[]') {
			$opText = 'H[0]';
		}
		elsif($op =~ /\w/ and $realParamCount == 1) { # type convert T()
			$opText = 'H()';
		}
		else {
			if($realParamCount == 2) {
				if($op eq '++' or $op eq '--') {
					$opText = 'H' . $op;
				}
				else {
					$opText = 'H ' . $op . ' H';
				}
			}
			elsif($realParamCount == 1) {
				$opText = $op . 'H';
			}
			else {
				print "OP::::::::  ", $realParamCount, "  ", $op, "\n";
			}
		}
		$opText =~ s/H/mopHolder/g;
		$cw->out($opText . ', ');
		$cw->out("_p);\n");
	}
}

sub writeClass
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_class");

	foreach(@{$self->{class}->{classList}}) {
		my $item = $_;
		my $name = $item->getName;
		
		next unless($self->canWrite($item));
		
		$cw->out("{\n");
		$cw->incIndent();
		
		Util::defineMetaClass($cw, $item, '_nd', 'declare');
		my $writer = new MetaClassWriter(
			class => $item,
			codeWriter => $cw,
			config => $self->{config},
			define => '_nd',
			classType => $item->getName,
		);
		$writer->write();
		$cw->out($action . "(_nd);\n");
		
		$cw->decIndent();
		$cw->out("}\n");
	}
}

sub canWrite
{
	my ($self, $item) = @_;

	return ($self->{config}->{allowPublic} and Util::itemIsPublic($item))
		|| ($self->{config}->{allowProtected} and Util::itemIsProtected($item))
		|| ($self->{config}->{allowPrivate} and Util::itemIsPrivate($item))
	;
}

sub getAction
{
	my ($self, $name) = @_;
	
	return $self->{define} . '.CPGF_MD_TEMPLATE ' . $name;
}

sub getReplace
{
	my ($self, $name) = @_;
	
	return 'replaceName("' . $name . '", _r)';
}



1;
