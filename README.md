# Inter-process communication in Unix enviroment 
This project shows how processes can communicate with each other in Unix enviroment. All the code was written in C language programing on Linux Ubuntu using Geany compiler.

The program's task is to read lines of text and display the number of characters they contain. Text can be given via a text file or the keyboard. The program can be paused, resumed and ended by signals provided by the user.
The program uses four processes, each with a different function.

## Scheme

![so_proj2 drawio](https://github.com/KamilBorkowskiYB/Linux_inter-process_communication/assets/142045004/3b42a57e-8b16-43c3-8882-73f17877dfc8)
  
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
Processes comunicate with each other via SIGUSR2, that orders them to read signal value from message queue

## Processes
All child processes consider only those messages that were added by Parent process.
### Parent process 
Recives signals only form process 3, all recived signals are not executed but added to messege queue, then parent process sends SIGUSR2 to Child porcess 1 to read the message queue.
### Child process 1
Reads lines of text from text file or keyboard and passes them to Child process 2 via pipe.
Ignores all signals except SIGUSR2 only if it was sent by Parent process and then sends SIGUSR2 to Child process 2.
### Child process 2
Reads given by Child process 1 lines of text from pipe, calculates number of characters in each line as passes it to Child process 3 via shared memory.
Ignores all signals except SIGUSR2 only if it was sent by Child process 1 and then sends SIGUSR2 to Child process 3.
### Child process 3
Retrieves the number of characters in a line of text from Child process 2 via shared memory and displays it on the terminal.
Recives SIGSTP, SIGCONT, SIGTERM from the user and SIGUSR2 from Child process 2. Signals from user are passed to Parent process.

## Menu
  ![image](https://github.com/KamilBorkowskiYB/Linux_inter-process_communication/assets/142045004/f7c0e2e2-679b-4a79-92e7-d60a3523cc42)

At the start of the program all processes output theirs ids. User can choose if program should read text from file or from keyboard.
