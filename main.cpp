#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    int arr[20];

    // Fill array with random numbers from 1 to 50
    srand(time(0));
    cout << "Array numbers: ";
    for(int i = 0; i < 20; i++) {
        arr[i] = rand() % 50 + 1;  
        cout << arr[i] << " ";
    }
    cout << endl;

    // Create pipe for communication
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1; 
    }

    int pid = fork();

    if(pid == 0) {  // Child process
        int min = arr[10];
        for(int i = 11; i < 20; i++) {
            if(arr[i] < min) {
                min = arr[i];
            }
        }

        cout << "Child process (PID: " << getpid() << ") found min: " << min << endl;

        // Send result to parent
        if (write(pipe_fd[1], &min, sizeof(min)) == -1) {
            perror("write");
            return 1;
        }
        close(pipe_fd[1]);
        return 0;
    } else {  // Parent process
        int min = arr[0];
        for(int i = 1; i < 10; i++) {
            if(arr[i] < min) {
                min = arr[i];
            }
        }

        cout << "Parent process (PID: " << getpid() << ") found min: " << min << endl;
        wait(NULL);

        // Get child's result
        int child_min;
        if (read(pipe_fd[0], &child_min, sizeof(child_min)) == -1) {
            perror("read");
            return 1;
        }

        // Find overall minimun
        int final_min = (min < child_min) ? min : child_min;
        cout << "\nSmallest number in entire array: " << final_min << endl;

        close(pipe_fd[0]); 
    }

    return 0;
}
