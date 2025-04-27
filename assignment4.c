#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>

// Functions for semaphore operations
void sem_wait(int semid, int index)
{
    struct sembuf sb = {index, -1, 0}; // Decrease the semaphore value (wait operation)
    semop(semid, &sb, 1);              // Perform the operation
}

void sem_signal(int semid, int index)
{
    struct sembuf sb = {index, 1, 0}; // Increment the semaphore value (signal operation)
    semop(semid, &sb, 1);             // Perform the operation
}

void child_process(int semid, int i)
{
    // Each child writes its character to the file
    FILE *file = fopen("output.txt", "a");
    if (file == NULL)
    {
        perror("fopen failed"); // Error opening file
        exit(EXIT_FAILURE);
    }

    char c = 'A' + i; // Character to be printed by the child process

    while (1)
    {
        sem_wait(semid, i); // Wait for the semaphore of this process

        // Write the character to the screen and the file
        printf("%c", c);
        fprintf(file, "%c", c);

        sem_signal(semid, (i + 1) % 4); // Signal the next semaphore
    }

    fclose(file); // Close the file (unreachable due to infinite loop)
    exit(0);      // Exit the child process
}

int main()
{
    // Create a unique key for the semaphore set
    key_t key = getpid(); // Using process ID as the key
    if (key == -1)
    {
        perror("ftok failed"); // Error generating key
        exit(EXIT_FAILURE);
    }

    // Create and initialize semaphores
    int semid = semget(key, 4, 0666 | IPC_CREAT); // Create a set of 4 semaphores
    if (semid == -1)
    {
        perror("semget failed"); // Error creating semaphores
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores, the first one starts at 1, others start at 0
    for (int i = 0; i < 4; i++)
    {
        int value = (i == 0) ? 1 : 0;
        if (semctl(semid, i, SETVAL, value) == -1)
        {
            perror("semctl failed");    // Error setting semaphore value
            semctl(semid, 0, IPC_RMID); // Remove the semaphore set
            exit(EXIT_FAILURE);
        }
    }

    // Create child processes manually
    if (fork() == 0)
    {
        child_process(semid, 0); // Child process 0
    }

    if (fork() == 0)
    {
        child_process(semid, 1); // Child process 1
    }

    if (fork() == 0)
    {
        child_process(semid, 2); // Child process 2
    }

    if (fork() == 0)
    {
        child_process(semid, 3); // Child process 3
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 4; i++)
    {
        wait(NULL); // Wait for any child to finish
    }

    // Remove the semaphore
    semctl(semid, 0, IPC_RMID);

    return 0;
}
