void primes(int leftPipe[2]) {
    int prime = -1, num = -1;
    
    close(leftPipe[1]); // Close write end of the input pipe

    // Read the first number from the pipe, which will be the prime
    if (read(leftPipe[0], &prime, sizeof(prime)) == 0) {
        // If no number is read, close the pipe and exit the process
        close(leftPipe[0]);
        exit(0);
    }

    // Print the prime number
    printf("prime %d\n", prime);

    int rightPipe[2];  // Create a new pipe for the next process
    pipe(rightPipe);   // rightPipe will pass numbers to the next process

    if (fork() == 0) { 
        // Child process: recursive call to handle the next prime
        close(leftPipe[0]);  // Close the parent's read end
        primes(rightPipe);   // Pass the new pipe to the next child process
    } else {
        // Parent process: filter numbers and send them to the child
        close(rightPipe[0]); // Close the read end of the rightPipe

        // Read numbers from the leftPipe (input pipe)
        while (read(leftPipe[0], &num, sizeof(num)) > 0) {
            // Write the number to the next pipe if it is not divisible by the prime
            if (num % prime != 0) {
                write(rightPipe[1], &num, sizeof(num));
            }
        }

        // Close pipes and wait for the child to finish
        close(leftPipe[0]);  // Close the read end of the leftPipe
        close(rightPipe[1]); // Close the write end of the rightPipe
        wait(0);             // Wait for the child process to finish
        exit(0);             // Exit the process after finishing
    }
}

int main(int argc, char *argv[]) {
    int pipeMain[2];  // Create a pipe for the main process
    pipe(pipeMain);   // pipeMain will pass numbers from main to the first child

    if (fork() == 0) {
        // First child process: start filtering primes
        primes(pipeMain);  // Pass the pipe to the primes function
    } else {
        // Main process: generate numbers and send them to the first child
        close(pipeMain[0]);  // Close the read end of the pipe

        // Write all numbers from 2 to 280 into the pipe
        for (int i = 2; i <= 280; i++) {
            write(pipeMain[1], &i, sizeof(i));
        }

        close(pipeMain[1]);  // Close the write end of the pipe
        wait(0);             // Wait for the first child process to finish
        exit(0);             // Exit the main process
    }
}