--------
Designer
--------

Luke McDaniel

-------------
Functionality
-------------

This program is named uniq. Its purpose is to remove all instances 
of repeated lines in a file while copying its content into a new file.

-----
Usage
-----

This program is intedned to be compiled an run in a Unix shell
enviornment. The following steps must be made in order to run this
program.

	1. Make sure the files: main.c readline.c Makefile are all in
	   the same subdirectory. Then navigate to that directory.

	2. To compile the program type "make" into the command line.

	3. To run the program type "./uniq < [filename] > uniq.out"
           into the command line.

	4. The file you input will not be changed but the contents of
	   uniq.out will be the same as the input file except with 
	   all of the repeated lines removed.
