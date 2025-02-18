1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets()` is a good choice for this application because it works on a line by line basis.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  When cmd_buff is a fixed size array, it is not compatible to do the kinds of memory comparisons required.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Commands may not be passed through correctly and may not be recognized by our program properly.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  The types of redirection can be classified using their operators. First the `>` operator will redirect the output of the left operand to overwrite the contents of the right operand. Next there is the `>>` operator which is the same as the `>` operator except it appends to the contents of the right operand instead of overwriting the contents. Last the `<` uses the right operand as the input for the left operand.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  While redirection is a method of controlling where input and output are coming from. Piping is simply passing the output of a command into the input of another.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important for clarity that error output and normal output are separated.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  It would be best to find a way to merge them. This could involve validating the output of the command to see if it was the expected output. If the output is anything other than expected, output the merged message.