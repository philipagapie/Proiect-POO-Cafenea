# Proiect-POO-Cafenea

# Cafe Management System (OOP)

## Overview
This is a C++ console application that simulates a complete Cafe Management System. The project relies strictly on Object-Oriented Programming principles and uses local CSV files to provide a robust, persistent database for all core cafe operations. 

This project was developed as part of the Object-Oriented Programming coursework at the Faculty of Automatic Control and Computer Science (UNSTPB).

## Project Architecture & Files
The entire logic is handled by a centralized source file, while data is divided relationally across distinct CSV files. A visual representation of the OOP classes and their relationships is available in `diagrama.png`.

* **`Codcafenea.cpp`**: The main application source code containing all class definitions, methods, and the main execution loop.
* **`Produse.csv`**: Database for the cafe's menu and inventory items.
* **`Angajati.csv`**: Database for staff records and management.
* **`Comenzi.csv`**: Logs of customer orders and transactions.
* **`Evenimente.csv`**: Scheduling and details for special events hosted at the cafe.
* **`Rapoarte.csv`**: Automated business reports and system logs.

## Technologies Used
* **Language:** C++
* **Concepts:** OOP (Classes, Encapsulation, Polymorphism), File I/O, Data Serialization.
* **Environment:** Windows executable included (`Codcafenea.exe`).

## How to Run

### Option 1: Run the pre-compiled executable (Windows)
Simply double-click the `Codcafenea.exe` file, or run it from the command prompt:
`Codcafenea.exe`

### Option 2: Compile from source
If you want to compile the code yourself (e.g., using `g++`), run the following command in the terminal:
`g++ Codcafenea.cpp -o Codcafenea`

Then execute it:
`Codcafenea.exe` (For Windows)
`./Codcafenea` (For Linux or macOS)

*(Note: Make sure all the `.csv` files are kept in the same directory as the executable so the application can read and write data correctly).*
