# Inter-process communication in Unix enviroment 
This project shows how processes can communicate with each other in Unix enviroment. All the code was written in C language programing on Linux Ubuntu using Geany compiler.

The program's task is to read lines of text and display the number of characters they contain. Text can be given via a text file or the keyboard. The program can be paused, resumed and ended by signals provided by the user.
The program uses four processes, each with a different function.

## Scheme
<TODO>
  
### Parent process 
### Child process 1
### Child process 2
### Child process 3 

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
