-------------
README for fw
-------------

/*---------------------------------------------------------*/
/*---------------------------------------------------------*/

---------
Designers
---------

Luke McDaniel (ltmcdani) and David Meyenberg (dmeyenb)

-------------
Functionality
-------------

This program is named fw. Its purpose is to count all of the 
words in a text file or files and output the most frequent
10 words or a user designated number of words.

This program is in its infant stages of development so it has
a few limitations. First off there was a bug in rehash function
created so the hash has a fixed size. The program quits out
when the table reaches 0.8 of its load factor. Our getlines
function is also on the fritz and not allowing us to read
extremely long lines. Due to these limitations this program
will crash with a seg fault when longer lines are in the
file or exit when it has reached ts load factor limit.

Our hashtable works well in the condition it is in now but
if this project is going to be scaled more work needs to
be done.

-----
Usage
-----

This program is intedned to be compiled an run in a 
Unix shell enviornment. The following steps must be made in 
order to run thisprogram.

	1. Place all of the program files in the same folder.

	2. Type "make" into the command line to compile
	   all of the files.

	3. To run the program you have a few options:

		1. First enter "./fw" into the command line.

		2. To select the number of words you want
		   to view type "-n" followed by the
		   the number of words and their frequncies
		   you want displayed.

		3. You can either specify the files you want
		   to run the program as arguments behind
		   to the command prompt by typing out their
		   files names. If you don't specify any
		   files the program will read from stdin.
	
	4. The output of the program is directed the the 
	   stdout file. You can see you output by typing the
	   following into the command line:
	   "./a.out > fw.out" and the output of your file
	   will be directed to fw.out.

