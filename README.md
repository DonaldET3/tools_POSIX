# tools for Unix systems

_______

## hexc
hexadecimal converter; converts binary data to and from hexadecimal digits

Digits are written in pairs seperated by either a space or a newline. The program takes an optional numeric argument which sets the number of bytes (pairs of digits) to put on each line. The default value is 26 because that fits neatly in 80 character columns.

### options
h: help  
x: generate binary file from hexadecimal input  
w: bytes per line (default: 26)  

The data is read from standard input and written to standard output.

_______

## break
Breaks a file into pieces of a specified size.

### options
h: help  
f: specify file to break (default: standard input)  
s: maximum size of each output file  
u: unit scale (K, M, G, T, or P)  
a: first part of output file names  
d: minimum number of digits in the file number  
z: last part of output file names  

_______

## merge
Merges many files into one.

### options
h: help  
f: specify output file name (default: standard output)  
a: first part of input file names  
d: minimum number of digits in the file number  
z: last part of input file names  

_______

## stow
Takes data from either standard input or a specified file and outputs it to standard output or a specified file in a format that allows the data to be retrieved from media without an end-of-file indicator. If the program is in retrieve mode, then the operation is reversed.

### options
h: help  
i: input file  
o: output file  
b: block size  
r: retrieve (reverse)  

### file format
data block length word  
data  
if last word was not zero, another data block length word  
