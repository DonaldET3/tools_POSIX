# tools for POSIX
simple tools for POSIX systems

<br />

# bin2hex
bin2hex takes binary data from stdin and outputs it as hexadecimal digits to stdout.
The digits are written in pairs seperated by either a space or a newline. The program takes an optional numeric argument which sets the number of pairs of hexadecimal numbers to put on each row. The default value is 26 because that fits neatly in 80 columns.

<br />

# hex2bin
hex2bin takes hexadecimal digits from stdin and outputs it as binary data to stdout.
The hexadecimal digits can be pairs of digits seperated by whitespace (such as a space or a newline) or simply a string of digits with no seperators. Other formats may work but those are the only two formats explicitly supported.
