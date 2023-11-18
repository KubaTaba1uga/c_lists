Readme assumes that Your current location is this dir.

Generate files with new prefix:
```
NEW_PREFIX=cal_ python3 ../../scripts/generate_sources.py ../../src/arl_list.c ../../include/c_lists/arl_list.h
```

If script succeed new files should appear:
  `../../src/cal_list.c`
  `../../iinclude/c_lists/cal_list.h`
  
Compile example:
```
gcc main.c ../../src/cal_list.c -I../../include -o main
```

Run example:
```
./main
```
