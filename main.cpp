
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
    pipe(pipe_fd);
    
  }
  

