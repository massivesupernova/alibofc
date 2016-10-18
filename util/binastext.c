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
  size_t sz = fread(bf, 1, MAX_BYTES, file);
  *bytes = (int)sz;
  return (sz != MAX_BYTES ? EOF : 0);
}

int readBytesFromFile(FILE* file, int bytes) {
  if (fread(bf, 1, bytes, file) != bytes) {
    return 0;
  }
  return 1;
}

int writeBytesToFile(FILE* file, unsigned char* p, int bytes) {
  if (fwrite(p, 1, bytes, file) != bytes) {
    printf("[E] Write decoded bytes to file failed: %s.\n", strerror(errno));
    return 0;
  }
  return 1;
}

int getBytesGroup(int i, int len, unsigned char* ch) {
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
  for (; (readLen = getBytesGroup(i, totalBytes, ch)) > 0; i += readLen) {
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

unsigned char decodeByte(unsigned char ch, unsigned type) {
  switch (type) {
  case 0x0000: // [0|01XXXX|0] => 00[00XXXX]
    return (ch >> 1) & 0x0F;
  case 0xFF00: // [0|??XXXX|0] => ??[00XXXX]
    return ((ch << 1) & 0xC0) | ((ch >> 1) & 0x0F);
  case 0x00FF: // [0|??XXXX|0] => 00[??XXXX]
  case 0xFFFF:
    return (ch >> 1) & 0x3F;
  default:
    break;
  }
  printf("[E] Invalid type in decodeByte %4x.\n", type);
  return 0xFF;
}

int isTypeByte(unsigned char ch) {
  return ((ch & 0x80) == 0) && ((ch & 0x01) == 1);
}

int isDataByte(unsigned char ch) {
  return ((ch & 0x80) == 0) && ((ch & 0x01) == 0);
}

int main(int argc, char** argv) {
  if (argc < 3 || argv == 0) {
    printf("[E] Invalid command line parameters.\nUsage: binastext <input_file> <output_file> [-d]\n");
    return 1;
  }
  char* inFileName = argv[1];
  char* outFileName = argv[2];
  if (inFileName == 0 || inFileName[0] == 0 || outFileName == 0 || outFileName[0] == 0) {
    printf("[E] Invalid input file name(s).\n");
    return 1;
  }
  FILE* infile = fopen(inFileName, "rb");
  if (infile == 0) {
    printf("[E] Open file to read failed %s: %s.\n", inFileName, strerror(errno));
    return 1;
  }
  FILE* outfile = fopen(outFileName, "wb");
  if (outfile == 0) {
    printf("[E] Open file to write failed %s: %s.\n", outFileName, strerror(errno));
    return 1;
  }
  long long inFileBytes = 0, outFileBytes = 0;
  if (argc > 3 && argv[3] != 0 && argv[3][0] == '-' && argv[3][1] == 'd') {
    for (;;) {
      unsigned char tp1 = 0, tp2 = 0, tp3 = 0;
      unsigned char destBuf[5] = {0};
      if (!readBytesFromFile(infile, 1)) {
        break;
      }
      inFileBytes += 1;
      if (isTypeByte(bf[0])) {
        tp1 = ((bf[0] << 1) & 0xC0) >> 6;
        tp2 = ((bf[0] << 1) & 0x30) >> 4;
        tp3 = ((bf[0] << 1) & 0x0C) >> 2;
        if (tp1 == 0) {
          printf("[E] Invalid encoded tp1 %2x.\n", tp1);
          break;
        }
        if (tp1 == 0x03 && tp1 == tp2 && tp2 == tp3) {
          printf("[E] Invalid encoded tp1==tp2==tp3=11.\n");
          break;
        }
        if (tp2 == 0x00 && tp1 != 0x00 && tp3 != 0x00) {
          if (tp3 == 0x02) {
            printf("[E] Invalid encoded tp3==10.\n");
            break;
          }
          if (tp1 == 0x01) {
            if (!readBytesFromFile(infile, 1)) {
              printf("[E] Invalid end of the file 0x01.\n");
              break;
            }
            inFileBytes += 1;
            if (!isDataByte(bf[0])) {
              printf("[E] Invalid data byte 0x01 %2x.\n", bf[0]);
              break;
            }
            if (tp3 == 0x01) {
              destBuf[0] = decodeByte(bf[0], 0x0000);
            }
            else {
              destBuf[0] = decodeByte(bf[0], 0xFF00);
            }
            if (!writeBytesToFile(outfile, destBuf, 1)) {
              break;
            }
            outFileBytes += 1;
          }
          else if (tp1 == 0x02) {
            if (!readBytesFromFile(infile, 2)) {
              printf("[E] Invalid end of the file 0x02.\n");
              break;
            }
            inFileBytes += 2;
            if (!isDataByte(bf[0]) || !isDataByte(bf[1])) {
              printf("[E] Invalid data byte 0x02 %2x %02x.\n", bf[0], bf[1]);
              break;
            }
            if (tp3 == 0x01) {
              destBuf[0] = decodeByte(bf[0], 0x0000);
              destBuf[1] = decodeByte(bf[1], 0x0000);
            }
            else {
              destBuf[0] = decodeByte(bf[0], 0x0000);
              destBuf[1] = decodeByte(bf[1], 0xFF00);
            }
            if (!writeBytesToFile(outfile, destBuf, 2)) {
              break;
            }
            outFileBytes += 2;
          }
          else {
            if (!readBytesFromFile(infile, 2)) {
              printf("[E] Invalid end of the file 0x03.\n");
              break;
            }
            inFileBytes += 2;
            if (!isDataByte(bf[0]) || !isDataByte(bf[1])) {
              printf("[E] Invalid data byte 0x03 %2x %2x.\n", bf[0], bf[1]);
              break;
            }
            if (tp3 == 0x01) {
              destBuf[0] = decodeByte(bf[0], 0xFF00);
              destBuf[1] = decodeByte(bf[1], 0x0000);
            }
            else {
              destBuf[0] = decodeByte(bf[0], 0xFF00);
              destBuf[1] = decodeByte(bf[1], 0xFF00);
            }
            if (!writeBytesToFile(outfile, destBuf, 2)) {
              break;
            }
            outFileBytes += 2;
          }
        }
        else {
          if (tp3 != 0x00) {
            if (tp2 == 0) {
              printf("[E] Invalid encoded tp2==00 tp3!=00.\n");
              break;
            }
            if (!readBytesFromFile(infile, 3)) {
              printf("[E] Invalid end of the file 0x04.\n");
              break;
            }
            inFileBytes += 3;
            if (!isDataByte(bf[0]) || !isDataByte(bf[1]) || !isDataByte(bf[2])) {
              printf("[E] Invalid data byte 0x04 %2x %2x %2x.\n", bf[0], bf[1], bf[2]);
              break;
            }
            destBuf[0] = (tp1 << 6) | decodeByte(bf[0], 0xFFFF);
            destBuf[1] = (tp2 << 6) | decodeByte(bf[1], 0xFFFF);
            destBuf[2] = (tp3 << 6) | decodeByte(bf[2], 0xFFFF);
            if (!writeBytesToFile(outfile, destBuf, 3)) {
              break;
            }
            outFileBytes += 3;
          }
          else if (tp2 != 0x00) {
            if (tp1 == 0) {
              printf("[E] Invalid encoded tp1==00 tp2!=00.\n");
              break;
            }
            if (!readBytesFromFile(infile, 2)) {
              printf("[E] Invalid end of the file 0x05.\n");
              break;
            }
            inFileBytes += 2;
            if (!isDataByte(bf[0]) || !isDataByte(bf[1])) {
              printf("[E] Invalid data byte 0x05 %2x %2x.\n", bf[0], bf[1]);
              break;
            }
            destBuf[0] = (tp1 << 6) | decodeByte(bf[0], 0xFFFF);
            destBuf[1] = (tp2 << 6) | decodeByte(bf[1], 0xFFFF);
            if (!writeBytesToFile(outfile, destBuf, 2)) {
              break;
            }
            outFileBytes += 2;
          }
          else {
            if (!readBytesFromFile(infile, 1)) {
              printf("[E] Invalid end of the file 0x06.\n");
              break;
            }
            inFileBytes += 1;
            if (!isDataByte(bf[0])) {
              printf("[E] Invalid data byte 0x06 %2x.\n", bf[0]);
              break;
            }
            destBuf[0] = (tp1 << 6) | decodeByte(bf[0], 0xFFFF);
            destBuf[1] = (tp2 << 6) | decodeByte(bf[1], 0xFFFF);
            if (!writeBytesToFile(outfile, destBuf, 1)) {
              break;
            }
            outFileBytes += 1;
          }
        }
      }
      else if (isDataByte(bf[0])) {
        destBuf[0] = decodeByte(bf[0], 0x00FF);
        if (!writeBytesToFile(outfile, destBuf, 1)) {
          break;
        }
        outFileBytes += 1;
      }
      else {
        printf("[E] Invalid encoded byte %2x.\n", bf[0]);
        break;
      }
    }
  }
  else {
    int readResult = 0, readBytes = 0, writtenBytes = 0, writeResult = 0;
    while ((readResult = readFile(infile, &readBytes)) != EOF) {
      inFileBytes += readBytes;
      writeResult = writeFile(outfile, readBytes, &writtenBytes);
      outFileBytes += writtenBytes;
      if (writeResult == 0) {
        break;
      }
    }
    if (readResult == EOF && readBytes > 0) {
      inFileBytes += readBytes;
      writeFile(outfile, readBytes, &writtenBytes);
      outFileBytes += writtenBytes;
    }
  }
  printf("Write complete: from %s (%lldB) to %s (%lldB).\n", inFileName, inFileBytes, outFileName, outFileBytes);
  fclose(infile);
  fclose(outfile);
  return 0;
}
