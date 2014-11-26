214-pa4
=======

Assignment 4: Search

###Version
1.0

###Introduction
Implement a search function for the inverted list file that is created from pa3.


###Invocation
    make
    ./search <index file>

###Commands
Each one of these commands are described below:

- sa:
    	String AND
    	This command gives a list of path files that contain all the terms(arguments) in the
    	sa command
- so:
        String OR
	    This command gives a list of path files that contain any of the terms(arguments) specirfied
	    for the so command

- sx:
        String XOR
        List of file paths that only contain one of the input terms

- nw:
        New Inverted-File
        Uses the inverter file to create a new inverted text based on the arguments that are passed to it

- ld:
        Load Inverted-File
        gives the ability to re-input an inverted text file

- help:
        Prints this page out to the console

- dp:
        dumps all the tokens to the console

- exit:
        Gracefully exits the program q: works as well
