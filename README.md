# C Lists Library

Simple and leightwight library for lists datastructures, no dependencies, C99 compliant, cross platform (Windows/Linux/MacOs).

Library instance can be generated for basic types (`char`, `float`, `int` etc.) and for `void *`. 
My goal is to give one list's interface, to whatever type You may need.

User can generate version of each list's seperatlly, based on defined macros.

Template for new c projects. Includes build system, test, mocking, readme, license, pipelines. 

1. [Getting Started](#Getting-Started)
3. [Prerequisites](#Prerequisites)
4. [Building](#Building)
5. [Tests](#Tests)
6. [Pipelines](#Pipelines)
7. [Authors](#Authors)
8. [License](#License)

## Design decision
Each list's interface follows FIRST-CLASS ADT Pattern. 


## Getting Started

Clone project
```
git clone https://github.com/KubaTaba1uga/c_project_template.git <new project name>
```

Delete template git files
```
rm -rf <new project name>/.git
```

Initiate new git files
```
cd <new project name>
git init
```

Once you have initialized the repository, create a remote repository somewhere like GitHub.com.

Add the remote URL to your local git repository
```
git remote add origin <URL>
```

Commit at least one file

Push changes
```
git push -u origin master
```

I recommend installing perequesitest and confirming that all tests are passing before src/* and test/* are actually deleted.

## Prerequisites
- [Meson](https://mesonbuild.com/)
- [Ruby](https://www.ruby-lang.org/en/)

Ruby is used by CMock.

## Building


Install perequesites by scripts
```
./scripts/install_ruby.sh
./scripts/install_meson.sh
```

If installing perequesites by scripts has failed, You need to install them manually.


Configure build for app and tests
```
meson setup build
```

Compile build config
```
meson compile -C build
```

## Tests

Run all tests
```
meson test -C build
```

### Style test

New tests units should be grouped by directories.
To understand more look on ./test directory.

Hierarchial build is very favoured by Meson, splitting test unit's into seperate directories allow keeping meson.builds short and simple.

## Pipelines

Two pipelines are configured. <br>
unit-tests.yaml makes sure that all unit tests passed. <br>
merge-gatekeeper.yaml makes sure that all pipelines ran successfully before merging pull request.



## Authors

  - **Jakub Buczyński** - *C Project Template* -
    [KubaTaba1uga](https://github.com/KubaTaba1uga)

## License

This project is licensed under the [MIT](LICENSE.md)
License - see the [LICENSE.md](LICENSE.md) file for
details
