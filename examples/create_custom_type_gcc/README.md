Let's create array's list, with char as a value's type.

Compile example:
```
gcc main.c ../../src/arl_list.c  -I../../include -o main -DARL_VALUE_TYPE=char
```

There is no default `ARL_VALUE_TYPE` defined. Defining it is mandatory.

Run example:
```
./main
```
