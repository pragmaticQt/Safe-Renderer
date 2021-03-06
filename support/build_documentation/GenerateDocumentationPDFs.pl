#!/usr/bin/perl

=head1 NAME
  GenerateDocumentationPDFs.pl

  Generate PDF files and put them in root folder of repository for files in
  * doc\SUG_LSR_Build_Configuration
  * doc\SUG_LSR_Editor_User_Manual
  * doc\SUG_LSR_Hardware_Requirements
  * doc\SUG_LSR_Safety_Notes

=head1 USAGE
  perl GenerateDocumentationPDFs.pl

=cut

BEGIN {
    if ($^O eq 'msys') {
        die <<'EOF';

You are running this script using Perl from Git installation.
Batch files can't be executed correctly from msys ("git") environment.
Please install Strawberry Perl as described on
https://adc.luxoft.com/confluence/display/POPULUS/Software#Software-perl

EOF
    }
}

use strict;
use warnings;
use File::Basename;
use File::Spec;
use Cwd qw(getcwd abs_path);

sub GetVariables () {
    # precondition: current folder is script folder

    # read parameters into hash
    my %vars = ();
    open(my $fh, '<', '../../config/DocBookConfig.ent') or die("Can't open 'config/DocBookConfig.ent' for reading: $!\n");
    while (my $line = <$fh>) {
        if ($line =~ /^\s*<!ENTITY\s+(\S+)\s+"(\S+)">/) {
            $vars{$1} = $2;
        }
    }
    close($fh);

    # check variables names
    my @validNames = (
        'ChangeLogDate',
        'ChangeLogRelease',
        'ChangeLogIssuer',
        'ChangeLogProduct_LSR',
        'CopyrightYear',
    );
    my %validNames = map { $_ => 1 } @validNames;
    for my $name (keys %vars) {
        unless ($validNames{$name}) {
            die("error: unknown parameter '$name'\n");
        }
    }
    return \%vars;
}

sub EscapePath ($) {
    my $path = shift;
    $path = '"' . $path . '"'
        if $path =~ m/ /;
    return $path;
}

sub RunCmd {
    system(@_) == 0
        or die "system '" . join(" ", @_) . "' failed: $?\n";
}

my $outputDir = getcwd();
my $scriptDir = abs_path($0);
$scriptDir =~ s!\\!/!g;
$scriptDir = dirname($scriptDir);

unless (chdir($scriptDir)) {
    print STDERR "error: can't change folder to $scriptDir\n";
    exit(1);
}

my $vars = GetVariables();
my %files = (
    '..\doc\SUG_LSR_Build_Configuration\LXFT.LSR.SUG-(LSR_Build_Configuration).xml'    => "LXFT.LSR.SUG.$$vars{'ChangeLogRelease'}-(Luxoft Safe Renderer Build Configuration).pdf",
    '..\doc\SUG_LSR_Editor_User_Manual\LXFT.LSR.SUG-(LSR_Editor_User_Manual).xml'    => "LXFT.LSR.SUG.$$vars{'ChangeLogRelease'}-(Luxoft Safe Renderer Editor User Manual).pdf",
    '..\doc\SUG_LSR_Hardware_Requirements\LXFT.LSR.SUG-(LSR_Hardware_Requirements).xml'    => "LXFT.LSR.SUG.$$vars{'ChangeLogRelease'}-(Luxoft Safe Renderer Hardware Requirements).pdf",
    '..\doc\SUG_LSR_Safety_Notes\LXFT.LSR.SUG-(LSR_Safety_Notes).xml'    => "LXFT.LSR.SUG.$$vars{'ChangeLogRelease'}-(Luxoft Safe Renderer Safety Notes).pdf",
);

print "Converting documentation XMLs into PDFs...\n";
while (my ($docbook, $pdf) = each(%files)) {
    print "\n\nProcessing $docbook...\n";
    RunCmd('..\DocBook\ConvertArticleToPDF.bat', EscapePath("..\\$docbook"), EscapePath("$outputDir\\$pdf"));
}

chdir('..\..');
print "\n\nDone! Resulting files are in " . $outputDir . "\n";
exit(0)
