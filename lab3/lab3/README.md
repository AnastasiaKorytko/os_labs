Lab #3 — Multithreaded Marker Management System

The program simulates a multithreaded system that manages a set of markers.
Each marker can be activated or deactivated by multiple threads working simultaneously.
Synchronization is handled using mutexes, condition variables, and atomic flags.

## Implemented

-Marker class — represents a single marker with an ID and active state, provides thread-safe activation/deactivation.
-MarkerSystem class — manages multiple markers, starts threads for:
	random marker toggling,
	monitoring current states,
	handling user commands.
-Threads interact safely through std::mutex and std::condition_variable.
-Main Program - initializes the system, runs threads, processes user input, and stops all threads cleanly.

## Standard
-C++17