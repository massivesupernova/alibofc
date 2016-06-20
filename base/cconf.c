#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define OUTPUT_STRING(s) if (fprintf(file, "%s", s) <= 0) { printf("write '%s' to file failed\n", s); exit(1); }

int main() {
  FILE* file = fopen("cconf.h", "wb");
  if (file == 0) {
    printf("open cconf.h to write failed\n");
    return 1;
  }
  OUTPUT_STRING("#ifndef BASE_CCONF_H\n#define BASE_CCONF_H\n")
  OUTPUT_STRING("#define _CRT_SECURE_NO_WARNINGS\n\n")

  // byte order
  int value = 0xABCD;
  if ((value & 0xFF) == 0xAB) {
    OUTPUT_STRING("#define IS_LITTLE_ENDIAN 1\n")
  }
  else {
    if ((value & 0xFF) != 0xCD) {
        printf("byte order test failed\n");
        exit(1);
    }
    OUTPUT_STRING("#define IS_LITTLE_ENDIAN 0\n")
  }

  // basic type
  if (sizeof(unsigned char) == 1 && sizeof(signed char) == 1) {
    OUTPUT_STRING("typedef unsigned char t_bool;")
    OUTPUT_STRING("typedef unsigned char t_char;")
  }
  else {
    printf("the size of char type is incorrect\n");
    exit(1);
  }

  OUTPUT_STRING("#endif /* BASE_CCONF_H */\n")
  fclose(file);
  return 0;
}
