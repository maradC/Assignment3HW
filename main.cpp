
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int findMin(int arr[], int start, int end) {
    int min = arr[start];
    for (int i = start + 1; i < end; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

int main() {
    const int SIZE = 20;
    int arr[SIZE];
    int fd[2];
    
    // Seed random number generator
    srand(time(nullptr));
    
    // Fill array with random numbers (0-100)
    cout << "Array elements: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 101;
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // Create pipe
    if (pipe(fd) == -1) {
        cout << "Pipe failed" << endl;
        return 1;
    }
    
    // Create child process
    pid_t pid = fork();
    
    if (pid < 0) {
        cout << "Fork failed" << endl;
        return 1;
    }
    
    if (pid == 0) { // Child process
        close(fd[0]); // Close read end
        int childMin = findMin(arr, SIZE/2, SIZE);
        cout << "Child process (PID " << getpid() << ") found minimum: " << childMin << endl;
        write(fd[1], &childMin, sizeof(childMin));
        close(fd[1]);
        exit(0);
    } else { // Parent process
        close(fd[1]); // Close write end
        int parentMin = findMin(arr, 0, SIZE/2);
        cout << "Parent process (PID " << getpid() << ") found minimum: " << parentMin << endl;
        
        int childMin;
        read(fd[0], &childMin, sizeof(childMin));
        close(fd[0]);
        
        // Wait for child to finish
        wait(NULL);
        
        // Find overall minimum
        int overallMin = (parentMin < childMin) ? parentMin : childMin;
        cout << "Overall minimum of the array: " << overallMin << endl;
    }
    
    return 0;
}
