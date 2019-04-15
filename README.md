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
breaks a file into pieces of a specified size

### options
h: help  
s: maximum size of broken output files  
u: unit scale (K, M, G, T, or P)  
a: first part of broken file names  
d: minimum number of digits in the file number  
z: last part of broken file names  
r: reconstruct  

_______

## oh1
"Opal Hash 1" data hasher

By default, the program computes a hash from a stream of input data from standard input.

In list mode, the program interprets standard input as a list of file names seperated by newline characters and outputs the hash of each file followed by a space, the file name, and a newline.

In check mode, the program reads a stream that was previously output by the program in list mode and outputs each file name followed by either ": OK" or ": FAILED" depending on whether the hash matches.

### options
h: print help and exit  
l: list mode  
c: check mode  

### hash computation
The hash bit length must be divisible by two.  
The input stream is cut into blocks with the first bit in the most significant position. The input blocks are exactly half of the hash length. If the stream needs to be lengthened to fit the last block, a single set bit is appended, follwed by as many clear bits as necessary to fill the block.

Initialize the hash state to zero.

For each input block...  
add the input block into the hash  
do three mixing rounds  

### round function
increment the hash  
multiply the hash by the largest Fermat prime that fits in the hash state  
copy the state, shift it down by one less than half of the bit width, and XOR that into the state  

### Fermat primes (decimal and hexadecimal)
3, 3  
5, 5  
17, 11  
257, 101  
65537, 10001  

_______

## list
make a list of file names

A list of files and directories are entered into the command line, then the program writes a list of file paths to standard output separated by newline characters. With the appropriate options, the program can output just regular files, just directories, or both.

### options
h: help  
f: output regular file paths  
d: output directory paths  
s: prepend file size in bytes to each file name  

_______

## stow
This program converts data into a format that allows it to be retrieved from a device. Input is received from standard input and sent to standard output.

### options
h: help  
b: buffer size  
r: retreive mode  

### stowage file format
file header  
data segments  

#### file header
null-terminated magic string "ODS1"  
version number  

#### data segment
segment length (2B)  
if length is not zero, segment data  

#### header byte format
Each header byte is written as three bytes to provide some error correction ability.  
The first byte is simply the data byte.  
In the second byte, the four most significant bits are inverted.  
In the third byte, the four least significant bits are inverted.  

_______

## parity
make a parity file for a list of files

### options
h: help  
s: maximum number of bytes to output  
