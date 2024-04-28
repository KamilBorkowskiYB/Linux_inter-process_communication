# Inter-process communication in Unix enviroment 
This project shows how processes can communicate with each other in Unix enviroment. All the code was written in C language programing on Linux Ubuntu using Geany compiler.

The program's task is to read lines of text and display the number of characters they contain. Text can be given via a text file or the keyboard. The program can be paused, resumed and ended by signals provided by the user.
The program uses four processes, each with a different function.

## Scheme
<TODO>
  
### Parent process 
Recives signals only form process 3, all recived signals are not executed but sent to messege queue, then parent process sends signals to all chilld porcesses to read the message queue.
### Child process 1
Recives signals only form the parent process. Reads lines of text from text file or keyboard and passes them to process 2 via pipe. 
### Child process 2
Recives signals only form the parent process. Reads given by process 1 lines of text from pipe, calculates number of characters in each line as passes it to process 3 via shared memory. 
### Child process 3 
Recives signals only form the parent process and outside of program. Handles user-supplied signals and feeds them to the parent process Retrieves the number of characters in a line of text from process two via shared memory and displays it on the terminal.

## Menu
  ![image](https://github.com/KamilBorkowskiYB/Linux_inter-process_communication/assets/142045004/f7c0e2e2-679b-4a79-92e7-d60a3523cc42)

At the start of the program all processes output theirs ids. User can choose if program should read text from file or from keyboard.
## Commands
As stated earlier, only process 3 listens to users signals, so all signals should be adressed to process 3 id.

SIGSTP pauses the program, can be also input via keyboard using ctrl + z
```bash
 kill -SIGSTP p3_pid
```
SIGCONT resumes the program
```bash
 kill -SIGCONT p3_pid
```
SIGTERM ends the program, when program isn't reading text
```bash
 kill -SIGTERM p3_pid
```
