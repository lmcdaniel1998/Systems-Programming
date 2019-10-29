--------
Designer
--------

Luke McDaniel

------------
Functonality
------------

The file detab.c contains a program that removes all of the tabs from a file
and replaces them with the proper number of spaces so that all of the tab
columns are maintained.

-----
Usage
-----

This program must be compiled and run in a Unix shell environment.
The following steps must be taken to execute this process.

    1. Navigate to the subdirectory where the program is stored

    2. enter the following lines into the command line:
        
        % gcc -o detab -Wall detab.c
        % ./detab < [your input file] > detab.out
    
    3. Your input file data with all of the tabs removed is copied to the
       file named detab.out

    4. You can view the file contents by typing the following into the
       command line:

        % cat detab.out
