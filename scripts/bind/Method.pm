package Method;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		name => undef,
		returnType => undef,
		
		location => undef,
		visibility => 'public',

		static => 0,
		virtual => 0,
		template => 0,

		paramList => [],
		templateParamList => [],

		%args
	};

	bless $self, $class;

	return $self;
}

sub getList
{
	my ($self, $class) = @_;

	return $class->{methodList};
}


1;
