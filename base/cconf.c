#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define WRITE_LINE(s) \
  if (fprintf(file, "%s\n", s) <= 0) { printf("[E] write '%s' to file failed\n", s); exit(1); } \
  printf("%s\n", s)

typedef union {
  unsigned char c[sizeof(int)];
  unsigned int  i;
} ByteOrderTest;

int main() {
  FILE* file = fopen("cconf.h", "wb");
  if (file == 0) {
    printf("[E] open cconf.h to write failed\n");
    return 1;
  }
  WRITE_LINE("#ifndef BASE_CCONF_H\n#define BASE_CCONF_H");
  WRITE_LINE("#define _CRT_SECURE_NO_WARNINGS");

  // byte-order
  ByteOrderTest data;
  data.i = 0xABCDEF;
  if (sizeof(unsigned int) < 4) {
    printf("[E] the size of char type is incorrect\n");
    exit(1);
  }
  if (data.c[0] == 0xEF && data.c[1] == 0xCD && data.c[2] == 0xAB && data.c[3] == 0x00) {
    WRITE_LINE("#define IS_LITTLE_ENDIAN 1\n");
  }
  else {
    if (data.c[0] != 0x00) {
        printf("[E] byte order test failed\n");
        exit(1);
    }
    WRITE_LINE("#define IS_LITTLE_ENDIAN 0\n");
  }

  // t_char t_byte t_int8 t_bool - 8-bit
  if (sizeof(unsigned char) == 1 && sizeof(signed char) == 1) {
    WRITE_LINE("typedef unsigned char t_char;");
    WRITE_LINE("typedef unsigned char t_byte;");
    WRITE_LINE("typedef signed char t_int8;");
    WRITE_LINE("typedef signed char t_bool;\n");
    WRITE_LINE("#undef true");
    WRITE_LINE("#undef false");
    WRITE_LINE("#define true 1");
    WRITE_LINE("#define false 0\n");
  }
  else {
    printf("[E] the size of char type is incorrect\n");
    exit(1);
  }

  // u_half t_half - 16-bit
  if (sizeof(unsigned short) == 2 && sizeof(short) == 2) {
    WRITE_LINE("typedef unsigned short u_half;");
    WRITE_LINE("typedef short t_half;");
  }
  else if (sizeof(unsigned int) == 2 && sizeof(int) == 2) {
    WRITE_LINE("typedef unsigned int u_half;");
    WRITE_LINE("typedef int t_half;");
  }
  else {
    printf("[E] no 16-bit integer type found\n");
    exit(1);
  }

  // u_full t_full - 32-bit
  if (sizeof(unsigned int) == 4 && sizeof(int) == 4) {
    WRITE_LINE("typedef unsigned int u_full;");
    WRITE_LINE("typedef int t_full;");
  }
  else if (sizeof(unsigned long) == 4 && sizeof(long) == 4) {
    WRITE_LINE("typedef unsigned long u_full;");
    WRITE_LINE("typedef long t_full;");
  }
  else {
    printf("[E] no 32-bit integer type found\n");
    exit(1);
  }

  // u_long t_long - 64-bit
  if (sizeof(unsigned long) == 8 && sizeof(long) == 8) {
    WRITE_LINE("typedef unsigned long u_long;");
    WRITE_LINE("typedef long t_long;");
  }
  else if (sizeof(unsigned long long) == 8 && sizeof(long long) == 8) {
    WRITE_LINE("typedef unsigned long long u_long;");
    WRITE_LINE("typedef long long t_long;");
  }
  else {
    printf("[E] no 64-bit integer type found\n");
    exit(1);
  }

  WRITE_LINE("\nstatic const t_char t_char_max = 0xFF;");
  WRITE_LINE("static const t_char t_char_min = 0x00;");
  WRITE_LINE("static const t_byte t_byte_max = 0xFF;");
  WRITE_LINE("static const t_byte t_byte_min = 0x00;");
  WRITE_LINE("static const t_int8 t_int8_max = 0x7F;");
  WRITE_LINE("static const t_int8 t_int8_min = 0x80;");
  WRITE_LINE("static const u_half u_half_max = 0xFFFF;");
  WRITE_LINE("static const u_half u_half_min = 0x0000;");
  WRITE_LINE("static const t_half t_half_max = 0x7FFF;");
  WRITE_LINE("static const t_half t_half_min = 0x8000;");
  WRITE_LINE("static const u_full u_full_max = 0xFFFFFFFF;");
  WRITE_LINE("static const u_full u_full_min = 0x00000000;");
  WRITE_LINE("static const t_full t_full_max = 0x7FFFFFFF;");
  WRITE_LINE("static const t_full t_full_min = 0x80000000;");
  WRITE_LINE("static const u_long u_long_max = 0xFFFFFFFFFFFFFFFF;");
  WRITE_LINE("static const u_long u_long_min = 0x0000000000000000;");
  WRITE_LINE("static const t_long t_long_max = 0x7FFFFFFFFFFFFFFF;");
  WRITE_LINE("static const t_long t_long_min = 0x8000000000000000;\n");

  // t_int u_int - at least 64-bit
  WRITE_LINE("typedef u_long u_int;");
  WRITE_LINE("typedef t_long t_int;");
  WRITE_LINE("static const u_int u_int_max = u_long_max;");
  WRITE_LINE("static const u_int u_int_min = u_long_min;");
  WRITE_LINE("static const t_int t_int_max = t_long_max;");
  WRITE_LINE("static const t_int t_int_min = t_long_min;\n");

  // t_ptr u_ptr - machine word size integer
  if (sizeof(void*) == 4) {
    WRITE_LINE("typedef u_full u_ptr;");
    WRITE_LINE("typedef t_full t_ptr;");
    WRITE_LINE("static const u_ptr u_ptr_max = u_full_max;");
    WRITE_LINE("static const u_ptr u_ptr_min = u_full_min;");
    WRITE_LINE("static const t_ptr t_ptr_max = t_full_max;");
    WRITE_LINE("static const t_ptr t_ptr_min = t_full_min;");
  }
  else if (sizeof(void*) == 8) {
    WRITE_LINE("typedef u_long u_ptr;");
    WRITE_LINE("typedef t_long t_ptr;");
    WRITE_LINE("static const u_ptr u_ptr_max = u_long_max;");
    WRITE_LINE("static const u_ptr u_ptr_min = u_long_min;");
    WRITE_LINE("static const t_ptr t_ptr_max = t_long_max;");
    WRITE_LINE("static const t_ptr t_ptr_min = t_long_min;");
  }
  else {
    printf("[E] invalid pointer type size\n");
    exit(1);
  }

  WRITE_LINE("#endif /* BASE_CCONF_H */\n");
  fclose(file);
  return 0;
}
