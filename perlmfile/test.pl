#/usr/local/bin/perl -w

use strict;
use mfile;

my $size = mfile::get_size("ge0.0025");

my $spec = mfile::ptrcreate('long',0,$size);
my $ref = mfile::ptrcreate('long',0,$size);

mfile::load_spec("ge0.0025", $spec, $size) 
    or die "Cannot load spec ge0.0025: $!\n";
mfile::load_spec("ge0.0024", $ref, $size) 
    or die "Cannot load spec ge0.0024: $!\n";

mfile::add_spec($spec, $ref, $size);

my $pos;
for ($pos = 0; $pos < $size; $pos++) {
#    my $val = mfile::ptrvalue($spec, $pos);
#   print "$pos\t$val\n";
}

my $a = mfile::ptrcreate('float');
my $b = mfile::ptrcreate('float');
my $x2 = mfile::ptrcreate('float');

if ( mfile::bs_shift_default($ref, $spec, $size, $a, $b, $x2) ) {
    my $dif = mfile::ptrvalue($a);
    my $lin = mfile::ptrvalue($b);
    my $chi = mfile::ptrvalue($x2);
    print $lin,"\t",$dif,"\t",$chi, "\n";
    my $pol = mfile::ptrcreate('double', 0, 2);
    print STDERR "pointer pol createt\n";
  mfile::ptrset($pol, $dif, 0);
  mfile::ptrset($pol, $lin, 1);
    print STDERR "polynom set\n";
  mfile::recal_poly($spec, $size, $pol, 2);
    print STDERR "recal done\n";
  mfile::save_spec('ge0.0025_shs', $spec, $size) 
      or die "Cannot write spec ge0.0025_shs: $!\n";
    print STDERR "spectrum written\n";
}


mfile::ptrfree($spec);
mfile::ptrfree($ref);

