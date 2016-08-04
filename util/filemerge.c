#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {
  if (argc < 2 || argv == 0) {
    printf("[E] Invalid command line parameters.\nUsage: filemerge <split_files_folder>\n");
    return 1;
  }
  char* dest_folder = argv[1];
  if (dest_folder == 0 || dest_folder[0] == 0) {
    printf("[E] Invalid split files folder.\n");
    return 1;
  }
  int len = 0;
  while (dest_folder[len++] != 0) {
    if (len > 4000) {
      printf("[E] Specified folder path is too long.\n");
      return 1;
    }
  }
  char split_file_name[4096] = {0};
  for (len = 0; dest_folder[len] != 0; ++len) {
    split_file_name[len] = dest_folder[len];
  }
  FILE* outfile = 0;
  char outname[] = "./merged.file";
  if ((outfile = fopen(outname, "wb")) == 0) {
    printf("[E] Open file to write failed: %s.\n", outname);
    return 1;
  }
  long fileno = 1001;
  FILE* split_file = 0;
  int ch = 0;
  long long totalbytes = 0;
  for (;;) {
    sprintf(split_file_name + len, "part.%ld", fileno++);
    if ((split_file = fopen(split_file_name, "rb")) == 0) {
      if (totalbytes == 0) {
        printf("[E] Open to read the 1st split file failed %s: %s.\n", split_file_name, strerror(errno));
      }
      else {
        printf("[W] The last one split file part.%ld is read.\n", fileno-2);
      }
      break;
    }
    long long bytes = 0;
    while ((ch = fgetc(split_file)) != EOF) {
       if (fputc(ch, outfile) == EOF) {
         printf("[E] Write target file %s failed, already read %lldB and extra %lldB from %s, current fail byte %02x: %s.\n",
             outname, totalbytes, bytes, split_file_name + len, ch, strerror(errno));
         fclose(split_file);
         goto close_outfile_and_return_fail;
       }
       bytes += 1;
    }
    totalbytes += bytes;
    printf("Read %s %lldB\t-> %lldB\n", split_file_name + len, bytes, totalbytes);
    fclose(split_file);
    split_file = 0;
  }
  if (totalbytes == 0) {
    printf("Completed, and nothing read.\n");
  }
  else {
    printf("Completed, target file %s %lldB.\n", outname, totalbytes);
  }
  fclose(outfile);
  return 0;
close_outfile_and_return_fail:
  fclose(outfile);
  return 1;
}
