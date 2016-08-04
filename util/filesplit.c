#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {
  if (argc < 2 || argv == 0) {
    printf("[E] Invalid command line parameters.\nUsage: filesplit <large_file_path>\n");
    return 1;
  }
  char* filepath_str = argv[1];
  if (filepath_str == 0 || filepath_str[0] == 0) {
    printf("[E] Invalid input file path.\n");
    return 1;
  }
  FILE* infile = fopen(filepath_str, "rb");
  if (infile == 0) {
    printf("[E] Open file to read failed %s: %s.\n", filepath_str, strerror(errno));
    return 1;
  }
  FILE* outfile = 0;
  char outname[128] = {0};
  long fileno = 1001;
  int logcount = 0;
  unsigned long maxfilebytes = (unsigned long)(1024*1024) * (unsigned long)4000;
  for (;;) {
    if (sprintf(outname, "./part.%ld", fileno++) < 11) {
      printf("[E] Compose output file name failed.\n");
      goto close_infile_and_return_fail;
    }
    unsigned long i = 0;
    unsigned long prev_mb = 0;
    unsigned long mb = 0;
    unsigned long kb = 0;
    unsigned long b = 0;
    int ch = 0;
    for (i = 0; i < maxfilebytes; ++i) {
      if ((ch = fgetc(infile)) == EOF) {
        break;
      }
      if (i == 0) {
        if ((outfile = fopen(outname, "wb")) == 0) {
          printf("[E] Open file to write failed %s: %s.\n", outname, strerror(errno));
          goto close_infile_and_return_fail;
        }
        printf("%s:\n", outname);
      }
      if (fputc(ch, outfile) == EOF) {
        printf("[E] Write file %s failed, already written %ldB, current fail char %02x: %s.\n", outname, i, ch, strerror(errno));
        fclose(outfile);
        goto close_infile_and_return_fail;
      }
      prev_mb = mb;
      mb = (i + 1) / (1024 * 1024);
      if (mb != prev_mb && (mb % 100) == 0) {
        printf("Written 100MB... %d\n", ++logcount);
      }
    }
    if (outfile) {
      fclose(outfile);
      outfile = 0;
    }
    if (ch == EOF) {
      // current "i" (added 1 and fgetc EOF) is the total written bytes
      i = i - mb * 1024 * 1024; // remain bytes for kb
      kb = i / 1024;            // calc kb
      i = i - kb * 1024;        // remain bytes for b
      b = i % 1024;             // calc b
      if (mb != 0 || kb != 0 || b != 0) {
        printf("Written %ldMB %ldKB %ldB... %d\n", mb % 100, kb, b, ++logcount);
      }
      if (logcount == 0) {
        printf("Completed, nothing written.\n");
      }
      else {
        printf("Written completed.\n");
      }
      break;
    }
  }
  fclose(infile);
  return 0;
close_infile_and_return_fail:
  fclose(infile);
  return 1;
}

