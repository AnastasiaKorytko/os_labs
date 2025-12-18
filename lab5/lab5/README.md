Lab #5 — Named Pipes Employee Management System

The program simulates a client-server system that manages employee records in a binary file.
The server process creates a binary file of employee records and handles client requests through named pipes.
Clients can read and modify employee records, with the server handling the data persistence.

## Implemented

- Employee struct — represents a single employee record with an ID, name, and hours worked.
- PipeMessage and ServerResponse structs — used for communication between client and server.
- PipeHandle class — RAII wrapper for Windows pipe handles to ensure proper resource management.
- Server Program — 
  - Creates a binary file of employee records (data input from console).
  - Sets up a named pipe server and waits for client connections.
  - Handles client requests: read, modify, and shutdown.
  - For read requests, the server sends the employee record to the client.
  - For modify requests, the server updates the employee record and saves the file.
  - For shutdown, the server stops.
- Client Program —
  - Connects to the server via a named pipe.
  - Allows the user to perform operations: read, modify, shutdown, and exit.
  - Sends requests to the server and displays the responses.

## Standard

- C++17