/**
# Save binary bytes as printable text

Printable characters' range is from [0|010000|0] to [0|111111|0] (0x20 to 0x7E), as form [H|MMMMMM|L].
We split binary byte to high 2-bit and lower 6-bit as HH[??XXXX] or HH[00XXXX].
Here `??` is `01`, `10`, or `11`, so `??XXXX` can directly map to character's middle 6 bits `MMMMMM`.

Consider HH[??XXXX] first, it has following 4 possibles for each `HH`.
The first possible 00[??XXXX] can be directly encoded using one character as [0|??XXXX|0].
About other possibles, we use 2, 3, or 4 characters to encode a binary byte sequence.
The first character indicates how many bytes and what kinds of byte in the sequence.
For example, [0|011011|1][0|??XXXX|0][0|??XXXX|0][0|??XXXX|0] represents 3 bytes sequence 01[??XXXX]10[??XXXX]11[??XXXX].
And [0|010000|1][0|??XXXX|0] represents 1 byte sequence 01[??XXXX].
Notice that the first character use 1 as its last bit, it indicates there is a binary byte sequence encoded following.
The first character's content `011011` indicates that the first byte has type 01[??XXXX], the second byte has type 10[??XXXX], and so on.
So using 2, 3, or 4 characters can encode a sequence has 1, 2, or 3 binary bytes.

    00[??XXXX] directly encode to one character [0|??XXXX|0]
    01[??XXXX] -> identified by 01
    10[??XXXX] -> identified by 10
    11[??XXXX] -> identified by 11

Consider about HH[00XXXX], it also has 4 pssibles as below.
We use `0000`, `0001`, `0010`, and `0011` to identify them.

    00[00XXXX] -> identified by 0000
    01[00XXXX] -> identified by 0001
    10[00XXXX] -> identified by 0010
    11[00XXXX] -> identified by 0011

Summary about binary byte:
- type 00[??XXXX] is directly encoded using one character
- other types indentified by 01, 10, 11, 0000, 0001, 0010, 0011 are all encoded as a sequnce form using multiple characters

Special rules:
- binary byte has `0000` type can only be encoded using 2 characters
- 3 continuous binary bytes of `11` type cannot be encoded together, because the first character will be 0x7F (it is not printable)

So middle 6 bits `MMMMMM` in the first character [0|MMMMMM|1] has following possibles:
- encode a sequence with 1-byte
        => 010000 100000 110000 000000 000100 001000 001100
- encode a sequence with 2-byte
        => 010100 011000 011100 010001 010010 010011
           100100 101000 101100 100001 100010 100011
           110100 111000 111100 110001 110010 110011
           000001 000010 000011
           000101 000110 000111
           001001 001010 001011
           001101 001110 001111
- encode a sequence with 3-byte
        => 010101 010110 010111 011001 011010 011011 011101 011110 011111
           100101 100110 100111 101001 101010 101011 101101 101110 101111
           110101 110110 110111 111001 111010 111011 111101 111110
*/
