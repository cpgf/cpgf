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
		baseNameList => [], # names of base classes, name~visibility
		baseList => [], # base classes

		_constructorList => [],
		_destructor => undef,
		fieldList => [],
		methodList => [],
		enumList => [],
		operatorList => [],

		classNameList => [], # names of inner classes
		classList => [], # inner classes

		defineList => [],

		inner => 0,

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub getConstructorList { return shift->{_constructorList}; }
sub addConstructor { my ($self, $v) = @_; Util::listPush($self->{_constructorList}, $v); }

sub getDestructor { return shift->{_destructor}; }
sub setDestructor { my ($self, $v) = @_; $self->{_destructor} = $v; }

sub isGlobal
{
	my ($self) = @_;

	return not defined $self->getName;
}

sub isAbstract
{
	my ($self) = @_;

	foreach(@{$self->{methodList}}) {
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


1;
