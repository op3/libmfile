#/usr/local/bin/perl

use ExtUtils::MakeMaker;
WriteMakefile(
	'NAME'		=> 'mfile',
	'LIBS'		=> ['-L/ikp/local/lib -lmfile', '-lm'],
	'OBJECT'	=> 'mfile_wrap.o bs_visus_lib.o'
);
