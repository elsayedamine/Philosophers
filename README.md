# 🧠 Philosophers — 42 Project

## 📘 Overview

The **Dining Philosophers** problem is a classic concurrency challenge illustrating synchronization, deadlock avoidance, and resource sharing in concurrent programming. This project, part of the 42 Network curriculum, simulates philosophers who alternately think, eat, and sleep while sharing forks.

This repository contains both the **mandatory** implementation using **threads and mutexes**, and the **bonus** implementation using **processes and named semaphores**, demonstrating two approaches to concurrency in C.

---

## ⚙️ Project Goals

- Correctly simulate philosopher behaviors without deadlock or starvation  
- Manage shared resources (forks) with synchronization primitives  
- Implement precise timing and death detection  
- Provide clean and safe termination of threads/processes  
- Handle edge cases and invalid inputs gracefully  

---

## 🚀 Implementations

### Mandatory: Threads + Mutexes

- Philosophers are represented by **POSIX threads** (`pthread`)  
- Forks are protected by **mutexes** to prevent simultaneous access  
- Thread-safe logging and state monitoring  
- Precise timing control with minimal CPU usage  
- Graceful exit when a philosopher dies or all have eaten enough  

### Bonus: Processes + Named Semaphores

- Philosophers are separate **processes** created with `fork()`  
- Synchronization achieved with **named POSIX semaphores** for forks, printing, and death signaling  
- Dedicated monitor process oversees philosopher states and signals termination  
- Robust inter-process communication and resource cleanup  
- Works on Unix-like systems supporting semaphores  

---

## 🧪 Usage

```bash
# Threads + Mutexes version
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

# Processes + Semaphores bonus version
./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

**Parameters:**

- `number_of_philosophers`: Number of philosophers/forks  
- `time_to_die`: Time in milliseconds a philosopher can survive without eating  
- `time_to_eat`: Time in milliseconds to eat  
- `time_to_sleep`: Time in milliseconds to sleep  
- `[number_of_times_each_philosopher_must_eat]`: *(optional)* simulation ends after each philosopher eats this many times  

---

## 🗂️ File Structure

```
philo/
├── Makefile
├── philo.h
├── src/
│ ├── philo.c
│ ├── simulation.c
│ ├── simulation_parse.c
│ ├── simulation_actions.c
│ └── simulation_init.c

philo_bonus/
├── Makefile
├── philo_bonus.h
├── src/
│ ├── philo_bonus.c
│ ├── simulation_bonus.c
│ ├── simulation_parse_bonus.c
│ ├── simulation_actions_bonus.c
│ └── simulation_init_bonus.c
README.md
```

---

## 🧵 Core Concepts & Challenges

- Mutual exclusion and synchronization with mutexes or semaphores  
- Process and thread lifecycle management  
- Accurate time tracking and sleeping without busy-waiting  
- Deadlock and starvation prevention  
- Thread/process-safe logging  
- Resource cleanup on termination or errors  

---

## 🧼 Resource Management

- Proper creation, handling, and destruction of mutexes and semaphores  
- Joining and terminating threads/processes cleanly  
- Closing and unlinking named semaphores (bonus)  
- Memory management and error checking  

---

## 🧠 What I Learned

- The nuances of concurrency using threads vs processes  
- Advanced synchronization techniques with mutexes and semaphores  
- Designing a responsive, safe, and clean concurrent system  
- Handling asynchronous events and precise timing in C  
- Debugging and structuring complex systems with multiple actors  

---

## 📜 License

This project is part of the 42 curriculum and intended for educational use only.

---

## ✍️ Author

**Amine Elsayed**  
42 Student — Systems programming and concurrency enthusiast

---

> _“Concurrency is not parallelism; concurrency is about dealing with lots of things at once.”_  

