package Class;

use base qw(Item);

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {};

	bless $self, ref $class || $class;

	$self = $self->SUPER::new(%args);

	my $values = {
		_category => Item::catClass,
		
		_baseNameList => [], # names of base classes, name~visibility
		_baseList => [], # base classes

		_constructorList => [],
		_destructor => undef,
		_fieldList => [],
		_methodList => [],
		_enumList => [],
		_operatorList => [],

		_classNameList => [], # names of inner classes
		_classList => [], # inner classes

		_defineList => [],

		_inner => 0,

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub getBaseNameList { return shift->{_baseNameList}; }
sub addBaseName { my ($self, $v) = @_; Util::listPush($self->{_baseNameList}, $v); }

sub getBaseList { return shift->{_baseList}; }
sub setBaseList { my ($self, $v) = @_; $self->{_baseList} = $v; }
sub addBase { my ($self, $v) = @_; Util::listPush($self->{_baseList}, $v); }
sub getBaseCount { return $#{@{shift->{_baseList}}} + 1; }

sub getConstructorList { return shift->{_constructorList}; }
sub addConstructor { my ($self, $v) = @_; Util::listPush($self->{_constructorList}, $v); }
sub getConstructorCount { return $#{@{shift->{_constructorList}}} + 1; }

sub getDestructor { return shift->{_destructor}; }
sub setDestructor { my ($self, $v) = @_; $self->{_destructor} = $v; }
sub hasDestructor { return defined shift->{_destructor}; }

sub getFieldList { return shift->{_fieldList}; }
sub addField { my ($self, $v) = @_; Util::listPush($self->{_fieldList}, $v); }
sub getFieldCount { return $#{@{shift->{_fieldList}}} + 1; }

sub getMethodList { return shift->{_methodList}; }
sub addMethod { my ($self, $v) = @_; Util::listPush($self->{_methodList}, $v); }
sub getMethodCount { return $#{@{shift->{_methodList}}} + 1; }

sub getEnumList { return shift->{_enumList}; }
sub addEnum { my ($self, $v) = @_; Util::listPush($self->{_enumList}, $v); }
sub getEnumCount { return $#{@{shift->{_enumList}}} + 1; }

sub getOperatorList { return shift->{_operatorList}; }
sub addOperator { my ($self, $v) = @_; Util::listPush($self->{_operatorList}, $v); }
sub getOperatorCount { return $#{@{shift->{_operatorList}}} + 1; }

sub getDefineList { return shift->{_defineList}; }
sub addDefine { my ($self, $v) = @_; Util::listPush($self->{_defineList}, $v); }
sub getDefineCount { return $#{@{shift->{_defineList}}} + 1; }

sub getClassNameList { return shift->{_classNameList}; }
sub addClassName { my ($self, $v) = @_; Util::listPush($self->{_classNameList}, $v); }

sub getClassList { return shift->{_classList}; }
sub setClassList { my ($self, $v) = @_; $self->{_classList} = $v; }
sub addClass { my ($self, $v) = @_; Util::listPush($self->{_classList}, $v); }
sub getClassCount { return $#{@{shift->{_classList}}} + 1; }

sub isInner { return shift->{_inner}; }
sub setInner { my ($self, $v) = @_; $self->{_inner} = $v; }

sub isGlobal
{
	my ($self) = @_;

	return not defined $self->getName;
}

sub isAbstract
{
	my ($self) = @_;

	foreach(@{$self->getMethodList}) {
		my $m = $_;
		if($m->isPureVirtual) {
			return 1;
		}
	}

	return 0;
}

sub getPolicyRules
{
	my ($self) = @_;
	my $rules = [];

	if(defined($self->getDestructor) and !Util::itemIsPublic($self->getDestructor)) {
		Util::listPush($rules, 'GMetaRuleDestructorAbsent');
	}

	my $hasDefaultCtor = 0;
	my $hasNonDefaultCtor = 0;
	my $hasNonPublicDefaultCtor = 0;
	foreach(@{$self->getConstructorList}) {
		my $c = $_;

		if(not $c->hasParam) {
			$hasDefaultCtor = 1;
			if(!Util::itemIsPublic($c)) {
				$hasNonPublicDefaultCtor = 1;
			}
		}
		else {
			$hasNonDefaultCtor = 1;
		}
	}

	if($hasNonPublicDefaultCtor or (!$hasDefaultCtor and $hasNonDefaultCtor)) {
		Util::listPush($rules, 'GMetaRuleDefaultConstructorAbsent');
	}

	if($self->isAbstract()) {
		Util::listPush($rules, 'GMetaRuleDefaultConstructorAbsent');
		Util::listPush($rules, 'GMetaRuleCopyConstructorAbsent');
	}

	return $rules;
};

sub fixupOwners
{
	my ($self) = @_;

	$self->{_destructor}->setOwner($self) if defined $self->{_destructor};

	$self->doFixupOwners($self->{_constructorList});
	$self->doFixupOwners($self->{_fieldList});
	$self->doFixupOwners($self->{_methodList});
	$self->doFixupOwners($self->{_enumList});
	$self->doFixupOwners($self->{_operatorList});
	$self->doFixupOwners($self->{_classList});
	$self->doFixupOwners($self->{_defineList});
}

sub doFixupOwners
{
	my ($self, $itemList) = @_;

	foreach(@{$itemList}) {
		my $item = $_;
		$item->setOwner($self);
	}
}


1;
