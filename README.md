# tools for Unix systems

**These programs are in the middle of an update process.**

_______

## bin2hex
bin2hex takes binary data from either standard input or a specified file and outputs it as hexadecimal digits to either standard output or a specified file.

The digits are written in pairs seperated by either a space or a newline. The program takes an optional numeric argument which sets the number of bytes (pairs of digits) to put on each line. The default value is 26 because that fits neatly in 80 columns.

### options
h: help  
w: bytes per line  
i: input file  
o: output file  

_______

## hex2bin
hex2bin takes hexadecimal digits from either standard input or a specified file and outputs it as binary data to either standard output or a specified file.

The hexadecimal digits can be pairs of digits seperated by whitespace (such as a space or a newline) or simply a string of digits with no seperators. Other formats may work but those are the only two formats supported.

### options
h: help  
i: input file  
o: output file  

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
