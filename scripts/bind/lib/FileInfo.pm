package FileInfo;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		location => undef,

		namespaceList => [],

		%args
	};

	bless $self, $class;

	return $self;
}


1;
