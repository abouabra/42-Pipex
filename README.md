# Pipex

## Description

Pipex is a project that aims to create a program that simulates the behavior of the shell's pipe operator.
it takes a file as input, and a series of commands to be executed, and an output file. The output of the first command is used as the input of the second command, and so on, until the last command, and the output of the last command is written to the output file.
The program takes a command line with the following structure:
```bash
./pipex InFile "cmd1" "cmd2" ... "cmdN" OutFile
```

```
InFile: input file
cmd1, cmd2, ..., cmdN: commands to be executed
OutFile: output file
```

it has the same structure as:
```bash
< InFile cmd1 | cmd2 | ... | cmdN > OutFile
```

## Installation

1. Clone the repository:
```bash
git clone https://github.com/abouabra/42-Pipex.git
```
2. Change the current working directory:
```bash
cd 42-Pipex
```
3. Compile the program:
```bash
make
```


## Example

1. make sure you have an input file:
### example.txt
```txt
Hello, World!
How are you?
I'm fine, thank you.
Goodbye!
```

2. Run the program:
```bash
./pipex example.txt "cat" "grep you" "wc -l" "cat -e" output.txt
```

3. Check the output file:
### output.txt
```txt
       2$
```
## Bonus

The program has a bonus feature that allows the use of the `here_doc` operator and the `appending` operator. The `here_doc` operator is used to redirect the standard input of a command to a file. The `appending` operator is used to append the output of a command to a file. The command line has the following structure:
```bash
./pipex here_doc LIMITER "cmd1" "cmd2" ... "cmdN" OutFile
```
it has the same structure as:
```bash
<< LIMITER | cmd1 | cmd2 | ... | cmdN >> OutFile
```

1. Run the program:
```text
./pipex here_doc LIMITER "cat" "grep you" "wc -l" "cat -e" output.txt

LIMITER
Hello, World!
How are you?
I'm fine, thank you.
Goodbye!
LIMITER

```

2. Check the output file:
### output.txt
```txt
       2$
       2$
```
we used the same output file, so the output of the second command was appended to the file.

## Final Note
If you have any suggestions or improvements, feel free to open an issue or submit a pull request.
