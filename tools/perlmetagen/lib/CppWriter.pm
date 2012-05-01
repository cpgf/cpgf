package CppWriter;

use base qw(CodeWriter);

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
		%args
	};

	Util::assignValues($self, $values);

	return $self;
}

sub beginIncludeGuard
{
	my ($self, $name) = @_;

	$name =~ s/\./_/g;
	$name = uc($name);
	$name = '__' . $name;

	$self->out('#ifndef ' . $name . "\n");
	$self->out('#define ' . $name . "\n");
	$self->out("\n\n");
}

sub endIncludeGuard
{
	my ($self) = @_;

	$self->out("\n\n");
	$self->out('#endif');
}

sub include
{
	my ($self, $fileName, $useBrackets) = @_;
	my $begin = $useBrackets ? '<' : '"';
	my $end = $useBrackets ? '>' : '"';

	$self->out("#include ${begin}${fileName}${end}\n");
}

sub beginNamespace
{
	my ($self, $namespace) = @_;

	if(defined $namespace) {
		$namespace .= ' ' unless($namespace eq '');
		$self->out("namespace " . $namespace . "{ \n");
		$self->out("\n\n");
	}
}

sub endNamespace
{
	my ($self, $namespace) = @_;

	if(defined $namespace) {
		if($namespace eq '') {
			$self->out("} // unnamed namespace\n");
		}
		else {
			$self->out("} // namespace $namespace \n");
		}
		$self->out("\n\n");
	}
}

sub useNamespace
{
	my ($self, $namespace) = @_;

	$self->out("using namespace $namespace;\n");
}

sub beginBlock
{
	my ($self) = @_;
	
	$self->out("{\n");
	$self->incIndent();
}

sub endBlock
{
	my ($self) = @_;
	
	$self->decIndent();
	$self->out("}\n");
}


1;
