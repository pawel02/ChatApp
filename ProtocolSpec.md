# This outlines the protocol spec used
Protocol uses small endian

First two bytes are for the message type (e.g. 0x0002 = json)
Bytes 3 to 6 are for the message size as an unsigned long long (e.g 18,446,744,073,709,551,615)

All the bytes after are message specific

# Example Plain text (0x0001)
0x00 0x01 - Message type of Plain text
0x00 0x00 0x00 0x05 - Message size of 5
0x05 0x01 0x05 0x02 0x01 - Message which is some random characters


# Example JSON (0x0002)
An example JSON message would be 

0x00 0x02 - Message type of JSON
0x00 0x00 0x00 0x05 - Message size of 5
0x05 0x01 0x05 0x02 0x01 - Message which is some random characters (this is not valid JSON)
