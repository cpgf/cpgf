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
		classList => undef,
		config => undef,
		fileWriterList => [],
		
		%args
	};

	bless $self, $class;

	return $self;
}

sub write
{
	my ($self) = @_;

	$self->buildFileWriterList();

	foreach(@{$self->{fileWriterList}}) {
		my $fileWriter = $_;
		$fileWriter->write();
	}
}

sub buildFileWriterList
{
	my ($self) = @_;
	my %fileMap = ();
	

	$self->{fileWriterList} = [];

	foreach(@{$self->{classList}}) {
		my $item = $_;
		my $location = $item->{location};

		if(not defined $fileMap{$location}) {
			$fileMap{$location} = new MetaFileWriter(
				sourceFileName => $location,
				config => $self->{config}
			);
			Util::listPush($self->{fileWriterList}, $fileMap{$location});
		}
		Util::listPush($fileMap{$location}->{classList}, $item);
	}
}



1;
