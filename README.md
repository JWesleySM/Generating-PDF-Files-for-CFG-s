# Generating PDF Files for CFG's

This project is a generator of PDF's for CFG's. It's implemented as a LLVM pass that creates dot files from bytecode files. It was made in LLVM 6.0.0

Before use it, [download and install](http://releases.llvm.org/) LLVM. 

To compile the pass:

1. unpack the directory in /llvm/lib/Transforms 
2. edit the file /llvm/lib/Transforms/CMakeLists.txt: add the line add_subdirectory(Generating-PDF-Files-for-CFG-s)
3. goto /llvm/build directory
4. type ```make Project1``` (or just type ```make``` to compile the entire LLVM)

To execute the pass:
```
   opt -load /llvm/build/lib/GenDot.so -GenDot file.bc
```
  
*if you are running on Mac OS, the extension of the shared library will be .dylib instead .so
  
To generate the PDF's:
  
```
   dot -Tpdf file.dot -o file.pdf
```
