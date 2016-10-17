// Save binary bytes as printable text
// Printable characters' range is from [0|010000|0] to [0|111111|0] (0x20 to 0x7E).
// Binary byte has following forms (?? can be 01, 10, and 11):
// ---
// 00[00XXXX] => [0|010001|1] [0|01XXXX|0]
// ??[00XXXX] => [0|010011|1] [0|??XXXX|0]
//
// 00[??XXXX] =>              [0|??XXXX|0]
// 01[??XXXX] => [0|010000|1] [0|??XXXX|0]
// 10[??XXXX] => [0|011000|1] [0|??XXXX|0] [0|??XXXX|0]
// 11[??XXXX] => [0|011011|1] [0|??XXXX|0] [0|??XXXX|0] [0|??XXXX|0]
//               [0|111111|1] is not a printable character
//
// 00         => [0|010001|1] [0|01XXXX|0]
// ??         => [0|010011|1] [0|??XXXX|0]
// 00 00      => [0|100001|1] [0|01XXXX|0] [0|01XXXX|0]
// 00 ??      => [0|100011|1] [0|01XXXX|0] [0|??XXXX|0]
// ?? 00      => [0|110001|1] [0|??XXXX|0] [0|01XXXX|0]
// ?? ??      => [0|110011|1] [0|??XXXX|0] [0|??XXXX|0]
// ---

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_BYTES 1024*1024
unsigned char bf[MAX_BYTES+4] = {0};

int readFile(FILE* file, int* bytes) {
  size_t sz = 0;
  sz = fread(bf, 1, MAX_BYTES, file);
  *bytes = (int)sz;
  return (sz != MAX_BYTES ? EOF : 0);
}

int readBytes(int i, int len, unsigned char* ch) {
  int count = 0;
  unsigned char curChar = 0;
  if (i >= len) {
    return 0;
  }
  curChar = bf[i++];
  ch[count++] = curChar;
  if ((curChar & 0x30) != 0 && (curChar & 0xC0) == 0) {
    return 1;
  }
  if (i >= len) {
    return 1;
  }
  curChar = bf[i++];
  ch[count++] = curChar;
  if ((curChar & 0x30) != 0 && (curChar & 0xC0) == 0) {
    return 1;
  }
  if ((ch[0] & 0x30) == 0 && (curChar & 0x30) != 0 || (curChar & 0x30) == 0 && (ch[0] & 0x30) != 0) {
    return 1;
  }
  if ((ch[0] & 0x30) == 0 && (curChar & 0x30) == 0) {
    return 2;
  }
  if (i >= len) {
    return 2;
  }
  curChar = bf[i++];
  ch[count++] = curChar;
  if ((curChar & 0x30) == 0 || (curChar & 0xC0) == 0) {
    return 2;
  }
  if ((ch[0] & 0xC0) == 0xC0 && (ch[1] & 0xC0) == 0xC0 && (curChar & 0xC0) == 0xC0) {
    return 2;
  }
  return 3;
}

int writeFile(FILE* outfile, int totalBytes, int* writtenBytes) {
  int i = 0, readLen = 0;
  unsigned char ch[5] = {0};
  *writtenBytes = 0;
  for (; (readLen = readBytes(i, totalBytes, ch)) > 0; i += readLen) {
    unsigned char destStr[5] = {0};
    int destLen = 0;
    if (readLen == 1) {
      if ((ch[0] & 0x30) == 0) {
        if ((ch[0] & 0xC0) == 0) {
          // 00[00XXXX] => [0|010001|1] [0|01XXXX|0]
          destStr[0] = 0x23;
          destStr[1] = (0x20 | (ch[0] << 1));
        }
        else {
          // ??[00XXXX] => [0|010011|1] [0|??XXXX|0]
          destStr[0] = 0x27;
          destStr[1] = (((ch[0] & 0xC0) >> 1) | ((ch[0] & 0x0F) << 1));
        }
        destLen = 2;
      }
      else if ((ch[0] & 0xC0) == 0) {
        // 00[??XXXX] => [0|??XXXX|0]
        destStr[0] = (ch[0] << 1);
        destLen = 1;
      }
      else {
        // ??[??XXXX] => [0|??0000|1] [0|??XXXX|0]
        destStr[0] = ((ch[0] & 0xC0) >> 1) | 0x01;
        destStr[1] = ((ch[0] & 0x3F) << 1);
        destLen = 2;
      }
    }
    else if (readLen == 2) {
      if ((ch[0] & 0x30) == 0 && (ch[1] & 0x30) == 0) {
        if ((ch[0] & 0xC0) == 0) {
          if ((ch[1] & 0xC0) == 0) {
            // 00[00XXXX] 00[00XXXX] => [0|100001|1] [0|01XXXX|0] [0|01XXXX|0]
            destStr[0] = 0x43;
            destStr[1] = (0x20 | (ch[0] << 1));
            destStr[2] = (0x20 | (ch[1] << 1));
          }
          else {
            // 00[00XXXX] ??[00XXXX] => [0|100011|1] [0|01XXXX|0] [0|??XXXX|0]
            destStr[0] = 0x47;
            destStr[1] = (0x20 | (ch[0] << 1));
            destStr[2] = ((ch[1] & 0xC0) >> 1) | ((ch[1] & 0x0F) << 1);
          }
        }
        else {
          if ((ch[1] & 0xC0) == 0) {
            // ??[00XXXX] 00[00XXXX] => [0|110001|1] [0|??XXXX|0] [0|01XXXX|0]
            destStr[0] = 0x63;
            destStr[1] = ((ch[0] & 0xC0) >> 1) | ((ch[0] & 0x0F) << 1);
            destStr[2] = (0x20 | (ch[1] << 1));
          }
          else {
            // ??[00XXXX] ??[00XXXX] => [0|110011|1] [0|??XXXX|0] [0|??XXXX|0]
            destStr[0] = 0x67;
            destStr[1] = ((ch[0] & 0xC0) >> 1) | ((ch[0] & 0x0F) << 1);
            destStr[2] = ((ch[1] & 0xC0) >> 1) | ((ch[1] & 0x0F) << 1);
          }
        }
      }
      else {
        // ??[??XXXX] ??[??XXXX] => [0|????00|1] [0|??XXXX|0] [0|??XXXX|0]
        destStr[0] = (((ch[0] & 0xC0) | ((ch[1] & 0xC0) >> 2)) >> 1) | 0x01;
        destStr[1] = ((ch[0] & 0x3F) << 1);
        destStr[2] = ((ch[1] & 0x3F) << 1);
      }
      destLen = 3;
    }
    else if (readLen == 3) {
      // ??[??XXXX] ??[??XXXX] ??[??XXXX] => [0|??????|1] [0|??XXXX|0] [0|??XXXX|0] [0|??XXXX|0]
      destStr[0] = (((ch[0] & 0xC0) | ((ch[1] & 0xC0) >> 2) | ((ch[2] & 0xC0) >> 4)) >> 1) | 0x01;
      destStr[1] = ((ch[0] & 0x3F) << 1);
      destStr[2] = ((ch[1] & 0x3F) << 1);
      destStr[3] = ((ch[2] & 0x3F) << 1);
      destLen = 4;
    }
    if (destLen == 0) {
      printf("[E] Write to dest string failed.\n");
      return 0;
    }
    if (fwrite(destStr, 1, destLen, outfile) == destLen) {
      *writtenBytes += destLen;      
    }
    else {
      printf("[E] Write dest string (%s) to file failed.\n", destStr);
      return 0;
    }
  }
  return 1;
}

int main(int argc, char** argv) {
  if (argc < 2 || argv == 0) {
    printf("[E] Invalid command line parameters.\nUsage: binastext <target_bin_file> <out_text_file>\n");
    return 1;
  }
  char* binFileName = argv[1];
  char* txtFileName = argv[2];
  if (binFileName == 0 || binFileName[0] == 0 || txtFileName == 0 || txtFileName[0] == 0) {
    printf("[E] Invalid input file name(s).\n");
    return 1;
  }
  FILE* infile = fopen(binFileName, "rb");
  if (infile == 0) {
    printf("[E] Open file to read failed %s: %s.\n", binFileName, strerror(errno));
    return 1;
  }
  FILE* outfile = fopen(txtFileName, "wb");
  if (outfile == 0) {
    printf("[E] Open file to write failed %s: %s.\n", txtFileName, strerror(errno));
    return 1;
  }
  int readResult = 0, readBytes = 0, writtenBytes = 0, writeResult = 0;
  long long binFileBytes = 0;
  long long txtFileBytes = 0;
  while ((readResult = readFile(infile, &readBytes)) != EOF) {
    binFileBytes += readBytes;
    writeResult = writeFile(outfile, readBytes, &writtenBytes);
    txtFileBytes += writtenBytes;
    if (writeResult == 0) {
      break;
    }
  }
  if (readResult == EOF && readBytes > 0) {
    binFileBytes += readBytes;
    writeFile(outfile, readBytes, &writtenBytes);
    txtFileBytes += writtenBytes;
  }
  printf("Write complete: from %s (%lldB) to %s (%lldB).\n", binFileName, binFileBytes, txtFileName, txtFileBytes);  
  fclose(infile);
  fclose(outfile);
  return 0;
}
