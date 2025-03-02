1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation has a loop that waits for each child process to finish before continuing. If I forgot to call waitpid() on all child processes I would most likely end up with a zombie process.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends because if they are not closed they could cause problematic behavior. Specifically the program could get stuck if two pipe ends catch each other during processing.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

"cd" is implemented as a built-in because it directly changes the state of the internal state of the shell. If it were implemented externally, "cd" would most likely run much slower.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

My shell already allocates and frees memory as it works so I would extend that to the piping function. Memory would be allocated as the command with the piped are parsed, thus only using as much memory as needed.