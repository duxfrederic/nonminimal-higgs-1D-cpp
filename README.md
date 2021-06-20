# What is this?
C++ code evolving a simplified equation of a complex Higgs evolving in an FLRW background during the preheating phase after inflation. The equation suffers from some pretty severe numerical instability, so is used to test different numerical schemes that might stabilize it. 
# How do I use it?
```bash
git clone git@github.com:duxfrederic/nonminimal-higgs-1D-cpp.git
cd nonminimal-higgs-1D-cpp

make
``` 
then run the example:
```bash
cd example
python analyze.py
```


