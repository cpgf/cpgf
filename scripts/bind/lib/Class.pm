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

		constructorList => [],
		destructor => undef,
		fieldList => [],
		methodList => [],
		enumList => [],
		operatorList => [],

		classNameList => [], # names of inner classes
		classList => [], # inner classes

		defineList => [],

		template => 0,
		templateParamList => [],

		inner => 0,

		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

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
		if($m->{pureVirtual}) {
			return 1;
		}
	}

	return 0;
}

sub getPolicyRules
{
	my ($self) = @_;
	my $rules = [];

	if(defined($self->{destructor})) { # and !Util::itemIsPublic($self->{destructor})) {
		Util::listPush($rules, 'GMetaRuleDestructorAbsent');
	}

	my $hasDefaultCtor = 0;
	my $hasNonDefaultCtor = 0;
	my $hasNonPublicDefaultCtor = 0;
	foreach(@{$self->{constructorList}}) {
		my $c = $_;

		if($#{@{$c->{paramList}}} < 0) {
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
