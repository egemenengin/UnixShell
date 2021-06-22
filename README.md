# UnixShell
Unix Shell C Project

# How to run
	For batch mode: ./shell inputFile
	For interactive mode: ./shell
If there is not any executable shell file, user should have run Makefile which is “./MakeFile”

# Design Overview
	In this project, for compiling the excepted task we used some code structures.
Firstly, we organized our code for both interactive and batch modes. If program is wanted to execute in batch mode, program must be invoked exactly as ./shell inputFile (inputFile is the file that we used for program's inputs).If program is wanted to execute in interactive mode, program must be invoked exactly ./shell.
If program is in batch mode, it takes input from file line by line; if program is in interactive mode, it takes command line as an input. 
Secondly, according to what is written into command line, program gets rid of the extra characters which are “ ; “,  “| “,  “   “ . After program eliminates those characters, we split input from “|” and added them into an array which is seperatedArr. The reason that program splits them like this is that these elements have to be worked in order. 
For the next part, program looks every element in the array; it counts semicolon and it opens thread pointer with using malloc to create a thread array according to the number of semicolon (for creating dynamic array). If there is not any semicolon, it checks whether it is quit or not. If that was “quit”, program will be terminated when the line is completed; if it is not “quit”, it creates single thread, then it executes the command. If there are semicolon(s), program takes until ”;”, again if it is “quit”  ”program will be terminated when the line is completed; if it is not “quit”, it creates single thread, then it executes the command. It continuous that until the end of the current element of separatedArr. The ones which are spilt from “;” will be executed simultaneously.  Before going into the other elements, every thread which are created is joined to wait until they are completed. Program does this process until the end of the seperatedArr. 
After that, if there was “quit” command in the line, program will be terminated. Otherwise, it will keep takes commands.
Finally, we created 3 different functions for helping to accomplish the task which are
•	char * removeExtra(char*, int) 
•	int isQuit(char *)
•	void * threadCall(void *)

# Complete Specification 
1-	If file pointer equals to null which means file could not open or found, program turns into interactive mode. 

2-	If there is not “quit” command in file and if it is the end of the file, program turns into interactive mode.

3-	If the input line is longer than 512 characters without new line, it prints an error message and it tries to take a new input. 

4-	If program is in batch mode, it prints the line which is read.

5-	If there is a character more than one, program will remove them with removeExtra function.
For example:  ls ;;; ls -la || ls -l -> ls ; ls -la | ls -l

6-	If there is no commands between characters, program will be create a thread again but it won’t be executed anything because there isn’t any command. 
For example: ls ; ; ls 
For this example, program will create 3 threads but nothing will be occur for that gap. We can see the execution of other commands. 

7-	 The ones that are separated from “|” works in order but the separated ones’ that they include “;” works simultaneously. 
For example: a ; b | c ; d
For this example, firstly a and b are executing simultaneously, c and d waits their completion; then c and d are executing simultaneously.

8-	Extra white spaces which does not break the form of the command is accepted by the program. 
For example:                                   ls                       -la                                       ;        ls
For this example, ls -la and ls will be executed. 
For example:                                   ls                       -                 la                                       ;        ls
		For this example, ls -la will not be executed but the last ls command will be executed.
		
9-	Empty command line is accepted by the program and it does not execute anything and it takes another input. 

10-	Once program starts to execute if  ./shell is called again, it starts to execute and takes new comments; if we want to quit from the program completely we will have to enter “quit” command twice. Also, this could be used for batch mode.
For example: prompt>  ./shell  or  prompt>  ./shell inputFile 
	
11-	Once program starts to execute but if there is not enough memory allocation, program returns null and it gives an error message. After that, it continues to execute.

12-	If there is an “echo” command, program prints until ; or | characters came. However, it only prints what comes after echo, it doesn’t execute them. 
For example: echo ls -la | ls ;; cat test1

	For this example, program prints “ls -la” but it executes ls and cat commands.
For example: echo quit
For this example, this line does not terminate the program, it only prints “quit”.

# Known bugs or problems

These bugs do not terminate the program, they only gives an error message and it continues.
1-	If command is invalid, program gives an error message but it still keeps executing with taking new inputs. Also, program executes other valid commands in the current command line. 
For example: a ; ls 
Error message is: “a : not found” but it executes ls command and it continuous to take new inputs. 

2-	White spaces which break the form of command causes the error message. Also, program executes other valid commands in the current command line and it continuous to take new inputs. 
For example: ls –  la  ;  ls
Error message: “ ls: cannot access '-': No such file or directory”
            “ ls: cannot access 'la': No such file or directory” but it executes ls command and it continuous to take new inputs
            
3-	We tried uttermost 400 KB so that we don’t exactly what program will do when input file is greater than 400 KB; it might give an error or it might crash. For such cases, we have buffers but we couldn’t try input files which is greater 400KB.

4-	Comment that is cd does not work but it does not cause to stop the program’s execution.

5-	When commands are read from a “.bat” file which are “@echo off” and “pause” they give an error (not found) but program keeps executing.






