Lab #4 — Interprocess Message Queue System

The program implements a message passing system between processes using a shared binary file as a ring buffer.
Multiple Sender processes can send messages to one Receiver process through synchronized file access.
Synchronization is handled using mutexes, condition variables, and file locking.

Implemented
-MessageQueue class — represents a thread-safe ring buffer in a binary file, provides blocking and non-blocking push/pop operations.
-Receiver class — manages message consumption from the queue, runs interactive command loop for reading messages.
-Sender class — handles message production to the queue, runs interactive command loop for sending messages.
-Processes interact safely through file-based synchronization using std::mutex and std::condition_variable.
-Main Programs - initialize the queue, run processes independently, and handle user commands cleanly.

Standard
-C++17