use strict;
use warnings;

use File::DirCompare;
use File::Copy;
use File::Path;
use File::RelDir;
use File::Spec;
use File::Copy::Recursive qw(dircopy);
use File::Path qw(rmtree);

my $sourcePath;
my $destPath;

my @actionList = ();

my $ACT_ADD = 1;
my $ACT_DELETE = 2;
my $ACT_MODIFIED = 3;
my $ACT_UNKNOWN = 4;

&doMain;

sub doMain
{
	&parseCommandLine;
	&process;
}

sub process
{
	File::DirCompare->compare($sourcePath, $destPath, \&onCallback #, { cmp => \&onCompare }
	);
	
	if(scalar(@actionList) == 0) {
		print "All up to date, nothing to do. \n";
	}
	else {
		foreach my $action(@actionList) {
			if($action->{action} == $ACT_UNKNOWN) {
				print "Unknown action ", $action->{source}, " => ", $action->{dest}, "\n";
				die "Please fix it before going on. \n";
			}
		}

		foreach my $action(@actionList) {
			printAction($action);
			executeAction($action);
		}
	}
}

sub onCompare
{
	my ($sourceFile, $destFile) = @_;

	return 1 if -d $sourceFile and ! -d $destFile;

	return -1 if ! -d $sourceFile and -d $destFile;
	
	return -M $sourceFile <=> -M $destFile;
}

sub onCallback
{
	my ($sourceFile, $destFile) = @_;
	
	return if(shouldIgnore($sourceFile) or shouldIgnore($destFile));
	
	my $action = {
		action=> $ACT_UNKNOWN,
		source => $sourceFile,
		dest => $destFile,
		isDir => 0
	};
	
	if(! $sourceFile) {
		$action->{action} = $ACT_DELETE;
		$action->{isDir} = (-d $destFile);
		$action->{source} = makeFullFileName($destPath, $destFile, $sourcePath);
	}
	elsif(! $destFile) {
		$action->{action} = $ACT_ADD;
		$action->{isDir} = (-d $sourceFile);
		$action->{dest} = makeFullFileName($sourcePath, $sourceFile, $destPath);
	}
	else {
		if(sameFileType($sourceFile, $destFile)) {
			$action->{action} = $ACT_MODIFIED;
			$action->{isDir} = (-d $sourceFile);
		}
		else {
			$action->{action} = $ACT_UNKNOWN;
		}
	}
	
	push @actionList, $action;
}

sub makeFullFileName
{
	my ($sourcePath, $sourceFile, $destPath) = @_;

	my $r = File::RelDir->New($sourcePath);
	my $rel = $r->Path($sourceFile);
	return File::Spec->catfile($destPath, $rel);
}

sub executeAction
{
	my ($action) = @_;

	if($action->{action} == $ACT_ADD) {
		if($action->{isDir}) {
			dircopy($action->{source}, $action->{dest});
		}
		else {
			copy($action->{source}, $action->{dest});
		}
	}
	elsif($action->{action} == $ACT_DELETE) {
		if($action->{isDir}) {
			rmtree($action->{dest});
		}
		else {
			unlink($action->{dest});
		}
	}
	elsif($action->{action} == $ACT_MODIFIED) {
		if($action->{isDir}) {
			die "Modified dir??? \n";
		}
		else {
			copy($action->{source}, $action->{dest});
		}
	}
	else {
	}
}

sub printAction
{
	my ($action) = @_;
	my $s = "";
	
	if($action->{action} == $ACT_ADD) {
		$s .= "ADD ";
		$s .=  $action->{source} . "[A] => " . $action->{dest};
	}
	elsif($action->{action} == $ACT_DELETE) {
		$s .= "DELETE ";
		$s .=  $action->{source} . " => " . $action->{dest} . "[D]";
	}
	elsif($action->{action} == $ACT_MODIFIED) {
		$s .= "MODIFIED ";
		$s .=  $action->{source} . " => " . $action->{dest};
	}
	else {
		$s .= "UNKNWON ";
		$s .=  $action->{source} . " => " . $action->{dest};
	}
	if($action->{isDir}) {
		$s .= " (DIR)";
	}
	print $s, "\n";
}

sub sameFileType
{
	my ($sourceFile, $destFile) = @_;
	
	return 0 if -d $sourceFile and ! -d $destFile;

	return 0 if ! -d $sourceFile and -d $destFile;
	
	return 1;
}

sub shouldIgnore
{
	my ($fileName) = @_;
	
	return 0 unless defined $fileName;

	return $fileName =~ /\.svn/;
}

sub parseCommandLine
{
	$sourcePath = $ARGV[0];
	$destPath = $ARGV[1];
	
	&usage unless defined $sourcePath and defined $destPath;
	die "$sourcePath must be directory \n" unless -d $sourcePath;
	die "$destPath must be directory \n" unless -d $destPath;
}

sub usage
{
	die "Usage: syncfolder source_folder dest_folder \n";
}
