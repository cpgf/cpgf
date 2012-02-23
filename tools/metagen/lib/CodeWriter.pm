package CodeWriter;

use strict;
use warnings;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_indent => 0,

		_text => '',

		%args
	};

	bless $self, ref $class || $class;

	return $self;
}

sub getText { return shift->{_text}; }

sub incIndent
{
	my ($self) = @_;

	++$self->{_indent};
}

sub decIndent
{
	my ($self) = @_;

	--$self->{_indent};
}

sub out
{
	my ($self, $s) = @_;

	my $indentText = ' ' x ($self->{_indent} * 4);

	if($self->{_text} =~ /\n$/s) {
		$self->{_text} .= $indentText;
	}

	$s =~ s/(\n+)([^\n])/$1$2$indentText/msg;
	$self->{_text} .= $s;
}


1;
