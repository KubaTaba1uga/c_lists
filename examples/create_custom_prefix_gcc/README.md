Readme assumes that Your current location is this dir.

Generate files with new prefix:
```
python3 ../../scripts/generate_sources.py ../../src/arl_list.c cal ./
```

If script succeed new files should appear:
  `./cal_list.c`
  `./cal_list.h`
  
Compile example:
```
gcc main.c cal_list.c -o main
```

Run example:
```
./main
```
