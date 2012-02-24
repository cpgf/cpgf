package MetaClassWriter;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_class => undef,
		_codeWriter => undef,
		_config => undef,
		
		_define => '_d',
		_classType => 'D::ClassType',

		_callbackParam => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub getScopePrefix
{
	my ($self, $prefix) = @_;

	$prefix = '' unless defined $prefix;

	if($self->{_class}->isGlobal()) {
		return '';
	}
	else {
		return $prefix . $self->{_classType} . '::';
	}
}

sub doCallback
{
	my ($self, $item) = @_;

	my $callback = $self->{_config}->{metaOutputCallback};

	if(defined $callback) {
		$self->{_callbackParam} = Util::createCallbackParam;
		&{$callback}($item, $self->{_callbackParam});
	}
	else {
		$self->{_callbackParam} = Util::createCallbackParam unless defined $self->{_callbackParam};
	}

	return $item;
}

sub skipItem
{
	my ($self) = @_;

	return $self->{_callbackParam}->{skipBind};
}

sub allowedMetaData
{
	my ($self, $category) = @_;

	foreach(@{$self->{_config}->{allowedMetaData}}) {
		if($_ == $category) {
			return 1;
		}
	}

	return 0;
}

sub write
{
	my ($self) = @_;

	$self->writeConstructor() if($self->allowedMetaData(Item::catConstructor));
	$self->writeField() if($self->allowedMetaData(Item::catField));
	$self->writeMethod() if($self->allowedMetaData(Item::catMethod));
	$self->writeEnum() if($self->allowedMetaData(Item::catEnum));
	$self->writeDefine() if($self->allowedMetaData(Item::catDefine));
	$self->writeOperator() if($self->allowedMetaData(Item::catOperator));
	$self->writeClass() if($self->allowedMetaData(Item::catClass));
}

sub writeConstructor
{
	my ($self) = @_;
	my $cw = $self->{_codeWriter};
	my $action = $self->getAction("_constructor");

	return if($self->{_class}->isGlobal());
	return if($self->{_class}->isAbstract());

	foreach(@{$self->{_class}->getConstructorList}) {
		my $item = $_;

		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<void * (");
		Util::writeParamList($cw, $item->getParamList, 0);
		$cw->out(")>(_p)");
		
		Util::writeDefaultParams($cw, $item->getParamList);
	}
}

sub writeField
{
	my ($self) = @_;
	my $cw = $self->{_codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_field");

	foreach(@{$self->{_class}->getFieldList}) {
		my $item = $_;
		my $name = $item->getName;
		
		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
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
	my $cw = $self->{_codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_method");

	my %methodOverload = ();

	foreach(@{$self->{_class}->getMethodList}) {
		my $item = $_;
		my $name = $item->getName;
		++$methodOverload{$name};
	}

	foreach(@{$self->{_class}->getMethodList}) {
		my $item = $_;
		my $name = $item->getName;
		my $overload = $methodOverload{$name} > 1;
		
		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
		next if($item->isTemplate);
		next unless($self->canWrite($item));
		
		$overload = $overload || $self->{_class}->isGlobal();

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
		$cw->out("&" . $prefix . $name . ", _p)");

		Util::writeDefaultParams($cw, $item->getParamList);
	}
}

sub writeEnum
{
	my ($self) = @_;
	my $cw = $self->{_codeWriter};
	my $typePrefix = $self->getScopePrefix('typename ');
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_enum");

	foreach(@{$self->{_class}->getEnumList}) {
		my $item = $_;
		my $name = $item->getName;
		
		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
		next unless($self->canWrite($item));

		my $typeName = $typePrefix . $name;

		if($name =~ /\@/ or $name eq '') {
			$name = 'GlobalEnum_'  . $self->{_config}->{projectID} . "_" . Util::getUniqueID();
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
	my $cw = $self->{_codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_enum");

	return unless($self->{_class}->getDefineCount > 0);

	$cw->out($action . "<long long>(" . $self->getReplace("GlobalDefine_" . $self->{_config}->{projectID} . "_" . Util::getUniqueID()) . ")\n");
	$cw->incIndent();
	
	foreach(@{$self->{_class}->getDefineList}) {
		my $item = $_;
		
		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
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
	my $cw = $self->{_codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_operator");

	foreach(@{$self->{_class}->getOperatorList}) {
		my $item = $_;
		my $name = $item->getName;
		
		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
		next unless($self->canWrite($item));
		
		$cw->out($action . "<" . $item->getReturnType . " (*)(");
		
		my $op = $item->getOperator;
		
		my $isStatic = ($self->{_class}->isGlobal() or $item->isStatic);
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
		$cw->out("_p)");
		
		Util::writeDefaultParams($cw, $item->getParamList);
	}
}

sub writeClass
{
	my ($self) = @_;
	my $cw = $self->{_codeWriter};
	my $prefix = $self->getScopePrefix();
	my $action = $self->getAction("_class");

	foreach(@{$self->{_class}->getClassList}) {
		my $item = $_;
		my $name = $item->getName;
		
		$item = $self->doCallback($item);
		
		next if($self->skipItem);
		
		next unless($self->canWrite($item));
		
		$cw->beginBlock();
		
		Util::defineMetaClass($self->{config}, $cw, $item, '_nd', 'declare');
		my $writer = new MetaClassWriter(
			_class => $item,
			_codeWriter => $cw,
			_config => $self->{_config},
			_define => '_nd',
			_classType => $item->getName,
		);
		$writer->write();
		$cw->out($action . "(_nd);\n");
		
		$cw->endBlock();
	}
}

sub canWrite
{
	my ($self, $item) = @_;

	return ($self->{_config}->{allowPublic} and Util::itemIsPublic($item))
		|| ($self->{_config}->{allowProtected} and Util::itemIsProtected($item))
		|| ($self->{_config}->{allowPrivate} and Util::itemIsPrivate($item))
	;
}

sub getAction
{
	my ($self, $name) = @_;
	
	return $self->{_define} . '.CPGF_MD_TEMPLATE ' . $name;
}

sub getReplace
{
	my ($self, $name) = @_;
	
	return 'replaceName("' . $name . '", _r)';
}



1;
