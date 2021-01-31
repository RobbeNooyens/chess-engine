# Chess Engine
Bachelor in Computer Science

## Project overview

This project is made for the Introduction to Programming course at the University of Antwerp. We had to implement the logic for a basic game of chess which was visualized with the QT library. After finishing the obligated parts, I had a basic game of chess running with minimal functionalities. Some rules however weren't implemented yet. Therefor there were some optional excercises which allowed us to complete the game with rokade, en passant and promotion and to gain the maximum score. On top of that, we had the choice to implement a save/load and undo/redo algorithm or make a simple AI that allowed you to play against the computer. These optional tasks are implemented in this project too.

## Code

The Mainwindow class handles the GUI display. The logic specific to pieces can be found in the SchaakStuk class and derived classes. The Game class contains the game logic, handles events and checks for check- or stalemate. The ChessBot class is the AI from the optional excercises. It first checks for checkmate and then for a normal check. In the AI can't check the opponent, it'll try to either take a more valuable piece if possible or move a piece so that more fields are in the range of the AI's pieces.

## QT Library

QT can be downloaded from https://www.qt.io/.
> Qt (pronounced "cute") is a free and open-source widget toolkit for creating graphical user interfaces as well as cross-platform applications that run on various software and hardware platforms such as Linux, Windows, macOS, Android or embedded systems with little or no change in the underlying codebase while still being a native application with native capabilities and speed. - Wikipedia

## Install and run

 1. Download the repository.
 2. Download the QT library and a C++ compiler and add them to the PATH environment variable.
 3. Set it up as a CMake project.
 4. Change settings in the config.h if you want to. (You can turn the AI on/off here.)
 5. Compile the sourcecode.
 6. Run the generated executable named 'schaken.exe'.
 

## Warning

*Plagiarism is illegal and will have consequences. Do NOT copy parts of this repository claiming it's your own work.*
