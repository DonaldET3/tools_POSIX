# tools for POSIX
simple tools for POSIX systems

<br />

## bin2hex
bin2hex takes binary data from stdin and outputs it as hexadecimal digits to stdout.

The digits are written in pairs seperated by either a space or a newline. The program takes an optional numeric argument which sets the number of digit pairs to put on each row. The default value is 26 because that fits neatly in 80 columns.

<br />

## hex2bin
hex2bin takes hexadecimal digits from stdin and outputs it as binary data to stdout.

The hexadecimal digits can be pairs of digits seperated by whitespace (such as a space or a newline) or simply a string of digits with no seperators. Other formats may work but those are the only two formats explicitly supported.

<br />

## break
Accepts a byte count and a filename as command line arguments. The filename can contain hash symbols where a number will be substituted. The file to be broken is input through stdin.

<br />

## merge
Accepts a filename from the command line. The filename must contain hash symbols where the file number is. The merged file is written to stdout.
