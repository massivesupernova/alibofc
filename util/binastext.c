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
- binary byte has `0000` type can only be encoded separately using 2 characters
- 3 continuous binary bytes of `11` type cannot be encoded together, because the first character will be 0x7F (it is not printable)

So middle 6 bits `MMMMMM` in the first character [0|MMMMMM|1] has following possibles.
Six bits can encode 64 numbers, following has total 63 numbers without `111111`.
- encode a sequence with 1-byte
        [01]0000 [10]0000 [11]0000 [0000]00 [0001]00 [0010]00 [0011]00
- encode a sequence with 2-byte
        [01][01]00 [01][10]00 [01][11]00 [01][0001] [01][0010] [01][0011]
        [10][01]00 [10][10]00 [10][11]00 [10][0001] [10][0010] [10][0011]
        [11][01]00 [11][10]00 [11][11]00 [11][0001] [11][0010] [11][0011]
        [0000][01] [0000][10] [0000][11]
        [0001][01] [0001][10] [0001][11]
        [0010][01] [0010][10] [0010][11]
        [0011][01] [0011][10] [0011][11]
- encode a sequence with 3-byte
        [01][01][01] [01][01][10] [01][01][11] [01][10][01] [01][10][10] [01][10][11] [01][11][01] [01][11][10] [01][11][11]
        [10][01][01] [10][01][10] [10][01][11] [10][10][01] [10][10][10] [10][10][11] [10][11][01] [10][11][10] [10][11][11]
        [11][01][01] [11][01][10] [11][01][11] [11][10][01] [11][10][10] [11][10][11] [11][11][01] [11][11][10]
*/

#include <stdio.h>

static const int MAX_BYTES = 1024*1024;
static unsigned char bf[MAX_BYTES+4] = {0};

int readFile(FILE* file, int* bytes) {
  size_t sz = 0;
  sz = fread(bf, 1, MAX_BYTES, file);
  *bytes = (int)sz;
  if (sz != MAX_BYTES) {
    return EOF;
  }
  return 0;
}

int readBytes(int i, int len, unsigned char* ch) {
  int count = 0;
  unsigned char curChar = 0;
  if (i >= len) {
    return 0;
  }
  curChar = bf[i++];
  ch[count++] = curChar;
  if ((curChar & 0x30) == 0 || (curChar & 0xC0) == 0) {
    return 1;
  }
  if (i >= len) {
    return 1;
  }
  curChar = bf[i++];
  ch[count++] = curChar;
  if ((curChar & 0x30) == 0 || (curChar & 0xC0) == 0) {
    return 1;
  }
  if (i >= len) {
    return 2;
  }
  curChar = bf[i++];
  ch[count++] = curChar;
  if ((curChar & 0x30) == 0 || (curChar & 0xC0) == 0 || (curChar & 0xC0) == 0xC0) {
    return 2;
  }
  return 3;
}

int writeFormatedFile(FILE* outfile, int nBytes) {
  int i = 0;
  int encodeLen = 0;
  unsigned char ch[5] = {0};
  for (; (encodeLen = readBytes(i, nBytes, ch)) > 0; i += encodeLen) {
    unsigned char destStr[5] = {0};
    int count = 0;
    if (encodeLen == 1) {
      if ((ch[0] & 0x30) == 0) {    // 00[00XXXX] => [0|010010|1][0|01XXXX|0]
        if ((ch[0] & 0xC0) == 0) {  // ??[00XXXX] => [0|010011|1][0|??XXXX|0]
          destStr[0] = 0x25;
          destStr[1] = (0x20 | (ch[0] << 1));
        }
        else {
          destStr[0] = 0x27;
          destStr[1] = (((ch[0] & 0xC0) >> 1) | ((ch[0] & 0x0F) << 1));
        }
        count = 2;
      }
      else if ((ch[0] & 0xC0) == 0) {
        destStr[0] = (ch[0] << 1);  // 00[??XXXX] => [0|??XXXX|0]
        count = 1;
      }
      else {                        // ??[??XXXX] => [0|??0000|1][0|??XXXX|0]
        destStr[0] = ((ch[0] & 0xC0) >> 1) | 0x01;
        destStr[1] = ((ch[0] & 0x3F) << 1);
        count = 2;
      }
    }
    else if (encodeLen == 2) {
      // ??[??XXXX]??[??XXXX] => [0|????00|1][0|??XXXX|0][0|??XXXX|0]
      count = 3;
    }
    else if (encodeLen == 3) {
      // ??[??XXXX]??[??XXXX] => [0|??????|1][0|??XXXX|0][0|??XXXX|0][0|??XXXX|0]
      count = 4;
    }
    if (fwrite(destStr, 1, count, outfile) != count) {
      return 0;
    }
  }
  return 1;
}

int main(int argc, char** argv) {
  if (argc < 2 || argv == 0) {
    printf("[E] Invalid command line parameters. \nUsage: binastext <target_bin_file> <out_file_name>\n");
    return 1;
  }
  char* filePathString = argv[1];
  char* outFileName = argv[2];
  if (filePathString == 0 || filePathString[0] == 0 || outFileName == 0 || outFileName[0] == 0) {
    printf("[E] Invalid input file path.\n");
    return 1;
  }
  File* infile = fopen(filePathString, "rb");
  if (infile == 0) {
    printf("[E] Open file to read failed %s: %s.\n", filePathString, strerror(errno));
    return 1;
  }
  FILE* outfile = fopen(outFileName, "wb");
  if (outfile == 0) {
    printf("[E] Open file to write failed %s: %s.\n", outFileName, strerror(errno));
    return 1;
  }
  int nBytes = 0;
  int result = 0;
  while ((result = readFile(infile, &nBytes)) != EOF) {
    if (writeFormatedFile(outfile, nBytes) == 0) {
      break;
    }
  }
  if (result == EOF) {
    writeFormatedFile(outfile, nBytes);
  }
  fclose(infile);
  fclose(outfile);
  return 0;
}
