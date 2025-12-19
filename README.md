# Mystore

A small CLI written in C.
Designed for a access to filepath using aliasing.

## Features

- **Auto-Save:** Your paths are automatically saved to the `mystore.db` file.
- **Easy Integration:** Easy to use in shell-functions

## Installation & Usage

You need a working Rust environment (Cargo) installed.

1. Clone the repository:
   ```bash
   git clone https://github.com/KJCats247/mystore.git
   cd mystore
   gcc mystore.c -o mystore
   ```

2. For global access:
    ```bash
    sudo mv mystore /usr/local/bin/
    ```
