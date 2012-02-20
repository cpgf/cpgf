package MetaWriter;

use strict;
use warnings;

use CodeWriter;
use Util;
use MetaFileWriter;

sub new
{
	my $class = shift || {};
	my %args = @_;

	my $self = {
		_classList => undef,
		_fileMap => undef,
		_config => undef,
		_fileWriterList => [],
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub write
{
	my ($self) = @_;

	$self->buildFileWriterList();

	foreach(@{$self->{_fileWriterList}}) {
		my $fileWriter = $_;
		$fileWriter->writeHeader();
		$fileWriter->writeSource();
	}
	
	$self->createNamespaceCpp();
}

sub createNamespaceCpp
{
	my ($self) = @_;

	return unless($self->{_config}->{autoRegisterToGlobal});
	
	my $outFileName = File::Spec->catfile($self->{_config}->{cppOutputDir}, 'meta_namespace_' . $self->{_config}->{id}) . '.cpp';
	my $content = <<EOM;
// Auto generated file, don't modify.

EOM

	$content .= 'const char * ' . Util::makeNamespaceSymbol($self->{_config}) . ' = ';
	if(defined $self->{_config}->{namespace}) {
		$content .= '"' . $self->{_config}->{namespace} . '"';
	}
	else {
		$content .= '0';
	}
	$content .= ";\n";

	Util::writeToFile($outFileName, $content);
}

sub buildFileWriterList
{
	my ($self) = @_;
	my %fm = ();
	

	$self->{_fileWriterList} = [];

	foreach(@{$self->{_classList}}) {
		my $item = $_;
		my $location = $item->getLocation;

		if(not defined $fm{$location}) {
			$fm{$location} = new MetaFileWriter(
				_sourceFileName => $location,
				_fileMap => $self->{_fileMap}->{$location},
				_config => $self->{_config}
			);
			Util::listPush($self->{_fileWriterList}, $fm{$location});
		}
		$fm{$location}->addClass($item);
	}
}



1;
