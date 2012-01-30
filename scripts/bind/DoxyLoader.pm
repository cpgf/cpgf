package DoxyLoader;

use strict;
use warnings;

use Util;

use Class;
use Constructor;
use Param;
use Operator;
use Field;
use Method;


sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		classList => [],

		currentClass => undef,

		%args
	};

	bless $self, $class;

	return $self;
}

sub parse
{
	my ($self, $doc) = @_;

	$self->parseClasses($doc->{classes});
};

sub parseClasses
{
	my ($self, $classes) = @_;
	my $classCount = $#{$classes} + 1;
	my $i;

	for($i = 0; $i < $classCount; ++$i) {
		my $docClass = $classes->[$i];

		my $class = new Class(
			name => $docClass->{name}
		);

		$self->{currentClass} = $class;

		Util::listPush($self->{classList}, $class);

		$self->parseBaseClasses($docClass->{base});

		$self->parseMembers($self->getMemebers($docClass->{public_members}));
		$self->parseMembers($self->getMemebers($docClass->{public_methods}));
		$self->parseMembers($self->getMemebers($docClass->{public_typedefs}));

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
	my ($self, $baseClasses) = @_;

	return unless defined $baseClasses;

   	foreach(@{$baseClasses}) {
   		my $baseName = $_->{name};
   		my $baseClass = Util::findItemByName($self->{classList}, $baseName);
   		Util::fatal("Can't find base class $baseName") unless defined $baseClass;
   		Util::listPush($self->{currentClass}->{baseList}, $baseClass);
   	}
}

sub parseMembers
{
	my ($self, $members) = @_;

	return unless defined $members;

	foreach(@{$members}) {
		my $mem = $_;
		my $kind = $mem->{kind};

		if($kind eq 'function') {
			$self->parseMethod($mem);
		}
		elsif($kind eq 'variable') {
			$self->parseField($mem);
		}
		elsif($kind eq 'enum') {
			$self->parseEnum($mem);
		}
		elsif($kind eq 'enumvalue') {}
	}
}

sub parseMethod
{
	my ($self, $member) = @_;

	if(not $self->{currentClass}->isGlobal()) {
		if(Util::getBaseName($self->{currentClass}->{name}) eq $member->{name}) { # constructor
			my $constructor = new Constructor;
			$self->parseParams($member->{parameters}, $constructor->{paramList});
			Util::listPush($self->{currentClass}->{constructorList}, $constructor);

			return;
		}
	}
	
	if($member->{name} =~ /^.*\boperator(\b.*)$/) { # operator
		my $op = $1;
		my $operator = new Operator(
			operator => $op
		);
		$self->parseParams($member->{parameters}, $operator->{paramList});
		Util::listPush($self->{currentClass}->{operatorList}, $operator);

		return;
	}

	my $method = new Method(
		name => $member->{name},
		returnType => $member->{type},
		static => Util::valueYesNo($member->{static})
	);
	$self->parseParams($member->{parameters}, $method->{paramList});
	Util::listPush($self->{currentClass}->{methodList}, $method);
}

sub parseParams
{
	my ($self, $docParams, $params) = @_;

	return unless defined $docParams;

	foreach(@{$docParams}) {
		my $p = $_;
		my $param = new Param(
			name => $p->{declaration_name},
			type => $p->{type},
			defaultValue => $p->{default_value}
		);
		Util::listPush($params, $param);
	}
}

sub parseField
{
	my ($self, $member) = @_;

	my $field = new Field(
		name => $member->{name},
		type => $member->{type},
		static => Util::valueYesNo($member->{static})
	);
	Util::listPush($self->{currentClass}->{fieldList}, $field);
}

sub parseEnum
{
	my ($self, $member) = @_;
}


1;
