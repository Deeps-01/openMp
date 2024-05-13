# Parallelising in OpenMP

OpenMP, or Open Multi-Processing, is an API (Application Programming Interface) that supports shared-memory multiprocessing programming in C, C++, and Fortran. It's widely used for parallel programming on shared-memory systems, where multiple processors or cores can access the same memory.

Architecture:

Threads: OpenMP operates on the concept of threads. Threads are independent sequences of instructions that can be executed concurrently. OpenMP allows developers to create and manage multiple threads within a program.
Shared Memory: All threads in an OpenMP program share the same memory space. This shared memory model simplifies data sharing among threads, as they can access and modify shared variables directly.
Master-Worker Model: OpenMP follows a master-worker model, where one thread (the master thread) creates a team of threads (worker threads) to perform parallel tasks. The master thread also participates in the computation.
Role in Parallel Programming:

Parallelism: OpenMP enables programmers to add parallelism to their code easily. By annotating sections of code with OpenMP directives, developers can instruct the compiler to parallelize those sections, distributing work across multiple threads for concurrent execution.
Scalability: OpenMP supports scalability by allowing programs to utilize all available processor cores. As hardware with more cores becomes common, OpenMP facilitates efficient utilization of these resources, improving performance for parallelizable tasks.
Synchronization: OpenMP provides constructs for thread synchronization, such as barriers, atomic operations, and critical sections. These constructs help manage shared resources and ensure data consistency when multiple threads access shared variables simultaneously.
Portability: One of OpenMP's strengths is its portability across different platforms and compilers. Programs written with OpenMP directives can typically run on various systems without significant modifications, provided the compilers support OpenMP standards.
Productivity: OpenMP promotes productivity by offering a high-level approach to parallel programming. Developers can focus on expressing parallelism using directives rather than dealing with low-level thread creation, synchronization, and management, which can be error-prone and complex.
Overall, OpenMP simplifies parallel programming by abstracting many of the complexities associated with multi-threading and shared-memory architectures, making it an essential tool for exploiting parallelism in modern computing systems.

-> Single source shortest path algorithm (SSSP)  <br>
-> Strongly Connected Components (SCC) <br>
FB = Forward Backward algorithm

