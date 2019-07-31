Name: Alexander Hamilton 
Candidate number: 13488
Date: 10/03/16

This file consists of two programs, zxpre.exe and zxenc.exe. zxpre.exe takes an
assembly file and removes whitespace, comments, and cleans up the code to make 
it more ordered and outputs this to stdout. The program can take either a file
put into the arguement or stdin. 

The second program is more of a work in progress at the moment. The main aim was
to encode the instructions from zxpre.exe into hexadecimal. So far the code can
convert lines at a time but runs into problems when trying to encode files and 
labels. zxenc.exe will still read files but give an incorrect return.

###INSTRUCTIONS###

To run zxpre.exe, use : "./zxpre.exe FILENAME" or "./zxpre.exe < FILENAME". The
test files for zxpre.exe can also be run by using the command: "make test". To
run zxenc.exe: " ./zxenc.exe -c 'INSTRUCTION' ". The "make" command will complie
both programs.