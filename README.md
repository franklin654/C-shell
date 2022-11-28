Name              : Sai Sivakesh
Enrollment number : 20/11/EC/026                        
                                                                                                    *** A Basic Rudementary Shell ***


we import the necessary header files required to use functions like execvp, wait and error handling functions.

we define the maximum characters that can be passed.

maximum size of each argument that can be passed.

define the delimetert to split the command input.

define the inbuilt shell commands which are handled locally.

initialize the inbuilt command functions.

lsh_readline tries to read the user input and returns either the input or the error and what type of error if any.

lsh_parser takes the user input returned by lsh_readline and splits it into arguments based on the delimeters we defined earlier.

lsh_execute takes the arguments from lsh_parser and checks if the command is a shell built_in or not if it is the corrosponding function is called to handle the command else lsh_execute forks a new process and uses execvp to call the command passed by the user and provides it the arguments passed by the user. and then waits until the command is finished executing or an error occured. if and error occured then it prints the error too.

lsh_loop function repeats the above steps repeatedly until either the user exits or an error occured.