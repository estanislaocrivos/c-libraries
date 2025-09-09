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
mkdir -p build
cd build
cmake ..
make
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

## Adding a new library to the project

To add a new library to the project, you may use the `new-lib-template` folder as a template. Follow these steps:

1. Copy the `new-lib-template` folder and rename it to your library name.

2. Update the `CMakeLists.txt` file in the root directory of your library to match your library name and source files:

   ```cmake
   add_subdirectory(my_library)
   ```

3. Add your source files to the `src` directory, your header files to the `inc` directory (if any), and your test files to the `test` directory (if any).

4. Update the `CMakeLists.txt` file inside your library folder to include your source files, replacing the template library identifiers (`NEW_LIB_TEMPLATE_SOURCES` and `new-lib-template`) with the proper ones.

5. If you added tests to the `test` directory, you must add the path to the new library inside the `project.yml`, under the `:paths:` section, so that Ceedling can find the library when running the tests.

6. You may also add the new headers files path to the `vscode/c_cpp_properties.json` file for better IntelliSense support in Visual Studio Code.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
