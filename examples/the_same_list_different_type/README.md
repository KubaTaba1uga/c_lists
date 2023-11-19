Readme assumes that Your current location is this dir.

Generate files with new prefix:
```
python3 ../../scripts/generate_sources.py ../../src/arl_list.c int ./
python3 ../../scripts/generate_sources.py ../../src/arl_list.c char ./
```

If script succeed new files should appear:
  `./int_list.c`
  `./int_list.h`
  `./char_list.c`
  `./char_list.h`
  
Compile example:
```
gcc main.c int_list.c char_list.c -o main -DCHAR_VALUE_TYPE=char -DINT_VALUE_TYPE=int
```

Run example:
```
./main
```
