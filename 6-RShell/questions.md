MESSAGE TO GRADER: PLEASE START A SERVER BEFORE RUNNING TESTS

1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

We use the EOF character to signify the end of the message so that the client knows when it received the full message. If a partial read is sent we keep reading to get the whole message.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

The networked shell can tell when commands begin and end using the EOF character. We send it with the message to indicate a command ended, and thus the next message is the next command.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols use the state and context around the execution of a protocol, while stateless treats everything as independently run.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

While unreliable, User Datagram Protocols (UDP) are very fast in their delivery, which is why we still use them in some cases.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides an Application Programming Interface (API) that allows for programs to interacted with networks through an interfaces without having to worry about all the details.