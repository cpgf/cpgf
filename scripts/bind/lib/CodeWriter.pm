package CodeWriter;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		indent => 0,

		text => '',

		%args
	};

	bless $self, $class;

	return $self;
}

sub incIndent
{
	my ($self) = @_;

	++$self->{indent};
}

sub decIndent
{
	my ($self) = @_;

	--$self->{indent};
}

sub out
{
	my ($self, $s) = @_;

	my $indentText = ' ' x ($self->{indent} * 4);

	if($self->{text} =~ /\n$/s) {
		$self->{text} .= $indentText;
	}

	$s =~ s/(\n+)([^\n])/$1$2$indentText/msg;
	$self->{text} .= $s;
}


1;
