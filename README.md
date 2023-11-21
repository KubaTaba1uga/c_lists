# C Lists Library
Simple and leightwight library for lists datastructures. <br>
Generic values, no dependencies, C99 compliant, cross platform (Windows/Linux/MacOs).

1. [Why?](#Why)
1. [Getting Started](#Getting-Started)
3. [Prerequisites](#Prerequisites)
4. [Building](#Building)
5. [Tests](#Tests)
6. [Pipelines](#Pipelines)
7. [Authors](#Authors)
8. [License](#License)

## Why
Library can be used as any other C data structure library, by compiling with default settings.
```
gcc -c src/arl_list.c -I include/ -o arl_list_lib.o
```

This is basic scenario where list's genericness is done by treating it's elements as of `void *` type. 

As this may be fine for most use cases, this kind of implementation is creating unnecessery limits. <br>
Let's imagine that You would like to use a list for `char` type, to create kind of dynamic string. <br>
Function need to create and fill the string, but it's length cannot be predicted. <br>
Real world example would be sth like buffer. <br>
It would require allocatin memory for each of the chars before appending the char as an element
```
  // Dynamic string
  char *my_string_0 = "nanananananana";
  char *my_sting_1 = "RTRTRTRTRTRTR";
  char *c;
  arl_ptr l;
  size_t i;

  // Creates list's instance
  arl_create(&l, 10);

  for (i = 0; i < strlen(my_string_0); i++) {
    c = malloc(sizeof(c));
    *c = my_string_0[i];
    // Appends my_string_0's values
    arl_append(l, c);
  }
  for (i = 0; i < strlen(my_string_1); i++) {
    c = malloc(sizeof(c));
    *c = my_string_0[i];
    // Appends my_string_1's values
    arl_append(l, c);
  }
  ...
```
pretty painfull ain't it?

So why can't we use the same mechanism as for `void *` but for `char?` 

We can easilly imagine the same list but working on char type and this is exactly what this library is about.
You can create the same list that You are usually using (with `void *` type) but with any type that You need. 
```
gcc -c src/arl_list.c -I include/ -o arl_list_lib.o -DARL_VALUE_TYPE=char
```
```
  // Dynamic string
  char *my_string_0 = "nanananananana";
  char *my_sting_1 = "RTRTRTRTRTRTR";
  char c;
  arl_ptr l;
  size_t i;

  arl_create(&l, 10);

  for (i = 0; i < strlen(my_string_0); i++) {
    arl_append(l, my_string_0[i]);
  }
  for (i = 0; i < strlen(my_string_1); i++) {
     arl_append(l, my_string_1[i]);
  }
  ...
```

## BTW What is nice about C Lists Library?
 - Only two files are required to use particullar list, source file and header file. <br>
 - There is no macro overusage so the library is simple to understand and use. <br>
 - Library can be generated for basic types (`char`, `float`, `int` etc.) and for `void *`. <br> 
There are plans to allow custom structures and enums, if that's sth that You need, let me know ;) 
 - There is a mechanism to delete almost all macro, look on generating_sources section.
 - There is mechanism to delete names duplications, look on generating_sources section.
 - Meson support.
 - Only lists, no unnecessary stuff.

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
