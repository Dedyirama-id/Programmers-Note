# Todoist - Programmers Notes CLI Application

Todoist is a Command Line Interface (CLI) application that allows users to manage their accounts, todos, and notebooks. This application provides a variety of features for creating and managing tasks and notes efficiently.

## Features

- User account management (register, login, logout, remove)
- Create and manage todos
- Create and manage notebooks and notes
- Send notebooks to other users
- Import and manage incoming notebooks
- Sort and display todos

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- Make sure you have the necessary libraries and headers (`hashtable.h`, `graph.h`, `binarytree.h`, `queue.h`, `account.h`, `note.h`, `app.h`, `utils.h`)

### Installation

1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd <repository-directory>
    ```

2. Compile the program:
    ```sh
    g++ todoist.cpp -o todoist
    ```

3. Run the program:
    ```sh
    ./todoist
    ```

## Usage

Once the application is running, you can use the following commands:

- `exit`          : Exit the application
- `help`          : Show the help message with available commands
- `login`         : Login to your account
- `logout`        : Logout of your account
- `au`            : Add a new user account
- `ru`            : Remove a user account
- `do <todo>`     : Add a new todo item
- `cn <notebook>` : Create a new notebook
- `an <note>`     : Add a new note to a notebook
- `on <notebook>` : Open a notebook to view its contents
- `sn <notebook>` : Send a notebook to another user
- `import`        : Manage notebook imports
- `todos`         : List all todos
- `dt <id>`       : Display details of a specific todo
- `rn <notebook>` : Remove a notebook
- `un`            : Undo the last notebook deletion
- `rt <id>`       : Remove a todo
- `wtd`           : Show what todos can be done now
- `sid`           : Sort todo list by ID
- `smp`           : Sort todo list by most possible to do
- `en <id>`       : Edit note by note ID
- `et <id>`       : Edit todo by todo ID

## Author
- Dedy Irama (23051204356)
- Usnida Faradila (23051204368)
- Rayyan (23051204385)