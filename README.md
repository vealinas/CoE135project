CoE 135 Project
Linux Terminal
Ubuntu on Oracle VM Virtualbox

How to compile and run
1. Make sure 'snake.c' and 'project.py' are in the same directory.
2. In the C code 'snake.c', change the value of eximpath into the correct path (where the codes are located).
3. Compile by typing in the following command in the Linux terminal:
    $ sudo gcc snake.c -o snake -l python2.7 -W -Wall -pthread
    $ sudo ./snake
4. In the VM Virtualbox menu bar, quickly deselect Input > Mouse Integration.
5. Click anywhere in the terminal and select Capture when a dialogue box pops up.
6. You can now control the snake with the mouse movement.
7. To bring back to normal operation, press the right Ctrl key and select Input > Mouse Integration. 

Bug
+ When mouse direction changes quickly or mouse moves very fast, game ends (game over) even without the snake bumping into the borders or to itself.
