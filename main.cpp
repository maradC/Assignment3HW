
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    // Create array of 20 numbers
    int arr[20];
    
    // Fill array with random numbers from 1 to 50
    srand(time(0));
    cout << "Array numbers: ";
    for(int i = 0; i < 20; i++) {
        arr[i] = rand() % 50 + 1;  // Numbers between 1 and 50
        cout << arr[i] << " ";
    }
    cout << endl;

    // Create pipe for communication
    int pipe_fd[2];
    pipe(pipe_fd);
    
    // Create child process
    int pid = fork();
    
    if(pid == 0) {  // Child process
        // Find minimum in second half (index 10 to 19)
        int min = arr[10];
        for(int i = 11; i < 20; i++) {
            if(arr[i] < min) {
                min = arr[i];
            }
        }
        
        cout << "Child process (PID: " << getpid() << ") found min: " << min << endl;
        
        // Send result to parent
        write(pipe_fd[1], &min, sizeof(min));
        return 0;
        
    } else {  // Parent process
        // Find minimum in first half (index 0 to 9)
        int min = arr[0];
        for(int i = 1; i < 10; i++) {
            if(arr[i] < min) {
                min = arr[i];
            }
        }
        
        cout << "Parent process (PID: " << getpid() << ") found min: " << min << endl;
        
        // Get child's result
        int child_min;
        read(pipe_fd[0], &child_min, sizeof(child_min));
        
        // Find overall minimum
        int final_min = (min < child_min) ? min : child_min;
        cout << "\nSmallest number in entire array: " << final_min << endl;
    }
    
    return 0;
}
