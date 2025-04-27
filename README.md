# Process Synchronization with Semaphores

This project demonstrates **process synchronization** using **System V semaphores** in C.\
Four child processes are created, each responsible for writing a specific character (`A`, `B`, `C`, `D`) **in order** to both the terminal and a file named `output.txt`.

The synchronization is achieved using semaphores to ensure that each process writes its character only when it's their turn.

## How It Works

- A set of 4 semaphores is created.
- Initially, the first semaphore is set to 1 (unlocked), and the others are set to 0 (locked).
- Each child process waits (`sem_wait`) for its own semaphore to become available.
- After writing its character, the child signals (`sem_signal`) the next process's semaphore, passing control to it.
- This creates a cyclic, ordered execution: A → B → C → D → A → B → C → D → ...

> **Note**: Each child process runs an infinite loop for continuous writing.

## Files

- `output.txt` — Output file where the characters are continuously appended.
- `main.c` — Source code for the project.

## Compilation

```bash
gcc -o semaphore_sync main.c
```

## Running

```bash
./semaphore_sync
```

You should see a continuous stream of `ABCDABCD...` printed on your terminal and written inside `output.txt`.

## Cleanup

After you stop the program (e.g., with `Ctrl+C`), the program automatically removes the created semaphores.

---

## Important Notes

- **Infinite Loop**: The child processes run indefinitely. You need to manually stop the program (`Ctrl+C`).
- **Resource Management**: The semaphores are cleaned up properly after the processes terminate.
- **File Handling**: Each child opens its own file pointer. Since they write sequentially (not simultaneously), file corruption is avoided.

---

## Example Output

Terminal:

```
ABCDABCDABCDABCD...
```

File (`output.txt`):

```
ABCDABCDABCDABCD...
```

---

## License

This project is provided for educational purposes.\
Feel free to use and modify it!

