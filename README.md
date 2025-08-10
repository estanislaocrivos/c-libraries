# Common C/C++ Libraries 🎛️

A common repository for C/C++ libraries. The aim of this project is to provide a common set of libraries that can be used across different C/C++ projects.

## Usage

This repository can be cloned as a submodule in your own project repository. To use it as a submodule, run the following command in your project directory:

```bash
git submodule add <repository-url> <path>
```

## Prerequisites

### Install CMake

Follow the instructions on the [CMake website](https://cmake.org/download/) to install CMake for your platform. On Linux-based systems, you can install it using your package manager. For example, on Ubuntu:

```bash
sudo apt install cmake
```

### Install a C/C++ compiler

You need a C/C++ compiler installed on your system. If you are on MacOS, you may already have Apple Clang installed. If you are on Linux, you can install GCC using your package manager. For example, on Ubuntu:

```bash
sudo apt install build-essential
```

### Install Pre-Commit

Install [Pre-Commit](https://pre-commit.com/) to manage and maintain multi-language pre-commit hooks. You can install it using pip:

```bash
pip install pre-commit
```

On the root directory of the project, you can run the following command to install the pre-commit hooks:

```bash
pre-commit install
```

### Customize the project

#### Visual Studio Code paths configuration

This project includes a `.vscode` directory with a `c_cpp_properties.json` file that configures the C/C++ extension for Visual Studio Code. You may need to update the `compilerPath` to match your system's C/C++ compiler path. For example, if you are using GCC, you can set it to `/usr/bin/gcc` or `/usr/bin/g++`.

## Building the libraries

To build the libraries, you can run the following command in the root directory of the project:

```bash
cmake -B build -DBUILD_DEMO=OFF
cmake --build build
```

To build the `main.c` file, you can run:

```bash
cmake -B build -DBUILD_DEMO=ON
cmake --build build
```

## Testing environment

A minimal test setup is provided using [Ceedling](https://www.throwtheswitch.org/ceedling) (v1.0.1 or later), which is a test framework for C that provides a simple way to write and run tests for your code. It runs on Ruby, so you need to have Ruby installed on your system. You can install Ruby using your package manager or follow the instructions on the [Ruby website](https://www.ruby-lang.org/en/documentation/installation/) (Ceedling v1.0.1 or later requires Ruby 3.0 or later). After installing Ruby, you can install Ceedling by running:

```bash
gem install ceedling
```

After adding the `ceedling` command to your PATH, you can run the tests by executing the following command in the `test` directory of the project:

```bash
ceedling test:all
```

To run a specific test file, you can run:

```bash
ceedling test:your_test_file
```

You can also run all the tests inside `test/` and generate a coverage report by running the provided `run-tests.sh` script:

```bash
./run-tests.sh
```

## Generating documentation

Doxygen is the default chosen tool for generating documentation from annotated source code. To install Doxygen, follow the instructions on the [Doxygen website](https://www.doxygen.nl/download.html) or use your package manager. For example, on Ubuntu:

```bash
sudo apt install doxygen
```

Once Doxygen is installed, you can generate the documentation by running the following command in the root directory of the project:

```bash
doxygen Doxyfile
```

Or you may use the provided `generate-docs.sh` script, which will run Doxygen and open the generated documentation in your web browser:

```bash
./generate-docs.sh
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
