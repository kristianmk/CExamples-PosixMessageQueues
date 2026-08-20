# CExamples-PosixMessageQueues
Small POSIX Message Queues example for the MAS418 lab.

Build both programs:

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic hello_receiver.c -o hello_receiver -lrt
gcc -std=c17 -Wall -Wextra -Wpedantic hello_sender.c -o hello_sender -lrt
```

Run the receiver and sender in separate terminals. Start the receiver first:
```sh
./hello_receiver
./hello_sender
```

The receiver creates and removes the message queue. If a receiver is interrupted
and leaves the queue behind, remove it before starting another receiver:
```sh
rm /dev/mqueue/mas418queue
```
