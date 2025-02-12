# xv6 Strace Implementation

## Overview
This project implements an `strace` command in the xv6 operating system, providing system call tracing functionality similar to Linux's `strace`. The implementation allows users to monitor system calls made by processes, filter calls based on success or failure, and store logs in a ring buffer for later retrieval.

## Features
- **strace on/off**: Enables or disables system call tracing globally.
- **strace run <command>**: Runs a specific command with tracing enabled.
- **strace dump**: Retrieves the last N system call logs from the kernel ring buffer.
- **strace -e <syscall>**: Filters tracing to a specific system call.
- **strace -s / -f**: Filters successful or failed system calls.
- **strace -o <filename>**: Saves traced system calls to a file.
- **Child Process Tracing**: Captures system calls from child processes.

## Implementation Details
Our approach implemented `strace` as a system call in the kernel. We then integrated `strace` directly into the xv6 shell (`sh.c`), modifying `syscall.c` to track system calls based on user commands. A kernel ring buffer stores the last N system calls, which can be retrieved using a `dump()` system call.

### Key Modifications:
- **Shell (`sh.c`)**: Parses `strace` commands and sets global tracing flags.
- **Syscall Handling (`syscall.c`)**: Logs system calls based on tracing state.
- **Ring Buffer**: Stores the last N system calls for retrieval.

## Example Usage
### Enabling Tracing
```sh
$ strace on
$ ls
[123] ls -> open() -> 3
[123] ls -> read() -> 1024
[123] ls -> close() -> 0
```

### Tracing a Single Command
```sh
$ strace run cat README.md
[456] cat -> open() -> 3
[456] cat -> read() -> 512
[456] cat -> close() -> 0
```

### Filtering Specific System Calls
```sh
$ strace -e write echo "Hello"
[789] echo -> write() -> 6
```

### Save Trace Logs to File
```sh
$ strace -o trace_log.txt ls
$ cat trace_log.txt
[123] ls -> open() -> 3
[123] ls -> read() -> 1024
[123] ls -> close() -> 0
```

## Example Usage
![image](https://github.com/user-attachments/assets/8ee9ff82-26f2-4c6d-a2a2-9bc005f4bfa1)

## Challenges & Learnings
 - **Kernel Space Constraints**: Debugging within xv6 required careful memory management to avoid kernel crashes.
 - **Process Tracking**: Implementing per-process tracing required modifying global and process-specific flags.
 - **Ring Buffer Limitations**: We initially struggled with dynamically allocating a flexible ring buffer, leading to a fixed-size implementation.

## Future Improvements
 - Implementing dynamic ring buffer allocation.
 - Extending filtering options for finer control over system call tracing.
