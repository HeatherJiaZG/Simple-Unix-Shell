# Simple Unix Shell
This is a command line interpreter (CLI). When a command accepted (in response to its prompt), the shell creates a child process that executes the command just entered and then prompts for more user input when it has finished.<br>

- basic shell `wish`: This is an interactive loop which repeatedly prints a prompt `wish>`, parses the input, executes the command specified on that line of input, and waits for the command to finish. This is repeated until the user types exit.<br>

Also implement `exit`, `cd`, `history`, and `path` as built-in commands.
- `exit`: When the user types `exit`, this shell calls the `exit` system call with 0 as a parameter. It is an error to pass any arguments to `exit`.

- `cd`: `cd` takes one argument (0 or >1 args should be signaled as an error). Used the `chdir()` system call with the argument supplied by the user to change directories; if `chdir` fails, that is also an error.

- `history`: When the user enters the `history` command with no arguments, then the shell prints a list of all the lines of input which the user has entered since the shell was started including the history command. The user may also supply a single argument to the `history` command `n` where `n` is a number. When this happens the shell prints the previous `n` lines of input (if `n` is not an integer, then its value is rounded up to the next greatest integer). 
