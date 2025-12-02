# ping pong with pipes

Note to self... remember to fix the hole in your pipe!

Two child processes talk to each other via a set of pipes, ping-ponging
a byte back and fourth. The program measures how many times the byte is
transferred between processes in a time interval specified by the user.

```
+->ifd > [ c1 ] > ofd->--|
|          +-------<-----+
+----<-----|---<------+--+
+-----<----+             |
|                        |
+-> ifd > [ c2 ] > ofd --+

```
