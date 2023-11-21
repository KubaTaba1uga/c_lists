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


## Getting Started

Clone project
```
git clone https://github.com/KubaTaba1uga/c_lists.git
```

Currently supported lists:
 - [Array List](https://en.wikipedia.org/wiki/Dynamic_array)

Variables to define:
 - ARL_VALUE_TYPE macro standing for type that You would like to use with arl_list.c

To confirm that everything is working we can go to `examples/create_custom_types_gcc` and compile the example.
```
gcc main.c ../../src/arl_list.c  -I../../include -o main -DARL_VALUE_TYPE=char && \
./main
```

## Prerequisites
- [Meson](https://mesonbuild.com/)
- [Ruby](https://www.ruby-lang.org/en/)

If installing perequesites by scripts has failed, You need to install them manually.

## Building

Tool use for building is Meson.

There are three building options available:
 - `enable_tests` flag indicating tests compilation
 - `arl_prefix` prefix for all array list's public functions
 - `arl_type` type of array list's elements

Create build with some options configured
```
meson setup build -Denable_tests=false -Darl_prefix=my_prefix -Darl_type=float
```

Compile build
```
meson compile -C build
```

Src file and header file shoudl appear in build dir `build/my_prefix_list.c` `build/my_prefix_list.h`.
These are regenerated files to include into Your project. Meson is capabile of doing much more, 
here are described only cutom functions. 

## Tests

Run all tests
```
meson test -C build
```

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
