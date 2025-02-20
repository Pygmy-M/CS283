1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  After calling execvp without using fork, our program will exit. But, when we use fork and delegate execvp to a child program, it allows for our original parent to keep running.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() sys call fails then it will return a value <0. In this case my program will return a ERR_EXEC_CMD value.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The first argument of execvp() gives execvp() information about what is the command it is running.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  We call wait because we don't want two processes running at the same time, so we tell the parent to wait until the child is done.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() provides the exit status of the last program that completed running. This information is important for making sure we have accurate and descriptive error messaging.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  When a quoted argument appears, my program finds the length until the next quote, and then writes everything in between the quotes into the respective index of argv.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I pretty much had to scrap 90% of what I had and start from scratch. That's ok though because I feel better about this implementation.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose of signals are to be a more simplistic but more efficient for of IPC. They also allow for the program to react a specific way to different signals depending on what the programmer chooses.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is a signal that immediately kills a process. This signal cannot be ignored or blocked in any way. It's intended use is being the strongest way to kill a program. The SIGTERM signal is similar to SIGKILL in that it is for terminating, but it can be ignored by the program. Lastly, SIGINT is just the signal for an interruption from the keyboard. This can be used to have any number of effects, with the linux default being program termination.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP halts a program but does not terminate it. It cannot be ignored like SIGINT because it is designed similarly to SIGKILL, it will stop the program no matter what. What sets SIGSTOP aside though is that it allows for the program to be continued later.