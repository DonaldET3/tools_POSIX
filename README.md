# tools for Unix systems

**These programs are in the middle of an update process.**

## bin2hex
bin2hex takes binary data from either standard input or a specified file and outputs it as hexadecimal digits to either standard output or a specified file.

The digits are written in pairs seperated by either a space or a newline. The program takes an optional numeric argument which sets the number of bytes (pairs of digits) to put on each line. The default value is 26 because that fits neatly in 80 columns.

### options
h: help  
w: bytes per line  
i: input file  
o: output file  

## hex2bin
hex2bin takes hexadecimal digits from either standard input or a specified file and outputs it as binary data to either standard output or a specified file.

The hexadecimal digits can be pairs of digits seperated by whitespace (such as a space or a newline) or simply a string of digits with no seperators. Other formats may work but those are the only two formats supported.

### options
h: help  
i: input file  
o: output file  

## break
Accepts a byte count and a filename as command line arguments. The filename can contain hash symbols where a number will be substituted. The file to be broken is input through stdin.

## merge
Accepts a filename from the command line. The filename can contain hash symbols where the file number is. The merged file is written to stdout.
