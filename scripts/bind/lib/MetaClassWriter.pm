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
	my ($self) = @_;

	return ($self->{class}->isGlobal() ? '' : $self->{classType} . '::');
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

	foreach(@{$self->{class}->{constructorList}}) {
		my $item = $_;
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<void * (");
		Util::writeParamList($cw, $item->{paramList}, 0);
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
		my $name = $item->{name};
		
		next unless($self->canWrite($item));
		
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
		my $name = $item->{name};
		++$methodOverload{$name};
	}

	foreach(@{$self->{class}->{methodList}}) {
		my $item = $_;
		my $name = $item->{name};
		my $overload = $methodOverload{$name} > 1;
		
		next if($item->{template});
		next unless($self->canWrite($item));

		$cw->out($action);
		$cw->out('(' . $self->getReplace($name) . ", ");
		if($overload) {
			$cw->out("(" . $item->{returnType} . " (" . $prefix . "*) (");
			Util::writeParamList($cw, $item->{paramList}, 0);
			$cw->out("))");
		}
		$cw->out("&" . $prefix . $name . ", _p);\n");
	}
}

sub writeEnum
{
	my ($self) = @_;
	my $cw = $self->{codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_enum");

	my $index = 0;

	foreach(@{$self->{class}->{enumList}}) {
		my $item = $_;
		my $name = $item->{name};
		
		next unless($self->canWrite($item));

		my $typeName = $prefix . $name;

		if($name =~ /\@/ or $name eq '') {
			$name = 'GlobalEnum_' . $index;
			$typeName = 'long long';
			++$index;
		}
		
		$cw->out($action . "<" . $typeName . '>(' . $self->getReplace($name) . ")\n");
		$cw->incIndent();
			foreach(@{$item->{valueList}}) {
				my $value = $_;
				my $n = $value->{name};
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

	my $index = 0;
	
	return if($#{@{$self->{class}->{defineList}}} < 0);

	$cw->out($action . "<long long>(" . $self->getReplace("GlobalDefine_") . ")\n");
	$cw->incIndent();
	
	foreach(@{$self->{class}->{defineList}}) {
		my $item = $_;
		
		my $value = $item->{value};
		if((not defined $value) or $value eq '') {
			next;
		}
		
		$cw->out('._element(' . $self->getReplace($item->{name}) . ', ' . $item->{name} . ")\n");
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
		my $name = $item->{name};
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<" . $item->{returnType} . " (*)(");
		if(not $item->{static}) {
			if($item->{const}) {
				$cw->out('const GMetaSelf &');
			}
			else {
				$cw->out('GMetaSelf');
			}
		}
		my $op = $item->{operator};
		my $opText = '';
		if($op eq '++' or $op eq '--') {
		}
		else {
			if($#{@{$item->{paramList}}} >= 0) {
				$cw->out(', ');
			}
			Util::writeParamList($cw, $item->{paramList}, 0);
		}
		$cw->out(")>(");
		my $realParamCount = $#{@{$item->{paramList}}} + 1;
		if(not $item->{static}) {
			++$realParamCount;
		}
		if($op eq '()') {
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
		my $name = $item->{name};
		
		next unless($self->canWrite($item));
		
		$cw->out("{\n");
		$cw->incIndent();
		
		Util::defineMetaClass($cw, $item, '_nd', 'declare');
		my $writer = new MetaClassWriter(
			class => $item,
			codeWriter => $cw,
			config => $self->{config},
			define => '_nd',
			classType => $item->{name},
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
