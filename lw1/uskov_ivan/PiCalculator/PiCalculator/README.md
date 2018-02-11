# PI Calculator

## Multi-Threaded Example:
```
cmd> PiCalculator.exe --tests=1000000 --threads=8
3.14063
```

## Single-Threaded Example:
```
cmd> PiCalculator.exe --tests=1000000
3.1422
```

## Help Example:
```
cmd> PiCalculator.exe --help

PI Calculator, evaluates PI number with Monte Carlo Method
Options:
  --threads=<threadsCount> : threads count, should be between 1 and 64
  --tests=<testsCount>     : tests count (monte carlo iterations) should be between 1 and 100000000
  ```