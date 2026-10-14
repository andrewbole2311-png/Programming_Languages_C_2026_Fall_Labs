// week5_task4_struct_binary_io.c   (BONUS)
// Name: BOLE  Surname: ANDREA  Student ID: 260ADB177
//
// Task 4: Save and load a struct in binary format with fwrite()/fread()
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task4_struct_binary_io

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
  char name[MAX_NAME_LEN];
  int age;
  float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls
// them directly)
int save_student_binary(const Student* s,
                        const char* filename);  // 0 on success, -1 on error
int load_student_binary(const char* filename,
                        Student* out);  // 0 on success, -1 on error
long file_size(const char* filename);   // bytes, -1 on error

int main(void) {
  Student s1 = {
      0};  // zero-initialised so unused name bytes are not garbage on disk
  strcpy(s1.name, "Alice");
  s1.age = 21;
  s1.gpa = 3.75f;

  Student loaded = {0};
  const char* bin_file = "student.bin";
  const char* txt_file = "student.txt";

  printf("Saving student in binary format...\n");
  // save binary
  if (save_student_binary(&s1, bin_file) != 0) {
    perror("Failed to save binary file");
    return EXIT_FAILURE;
  }

  // save text version for size comparison
  FILE* fp_txt = fopen(txt_file, "w");
  if (fp_txt == NULL) {
    perror("Failed to open text file for writing");
    return EXIT_FAILURE;
  }
  fprintf(fp_txt, "%s %d %.2f\n", s1.name, s1.age, s1.gpa);
  fclose(fp_txt);

  printf("Loading student from binary file...\n");
  // load binary
  if (load_student_binary(bin_file, &loaded) != 0) {
    perror("Failed to load binary file");
    return EXIT_FAILURE;
  }

  // print loaded record and file size comparisons
  printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age,
         loaded.gpa);
  printf("Text file size: %ld bytes\n", file_size(txt_file));
  printf("Binary file size: %ld bytes\n", file_size(bin_file));

  return EXIT_SUCCESS;
}

// Dump raw struct bytes directly into binary file
int save_student_binary(const Student* s, const char* filename) {
  if (s == NULL || filename == NULL) {
    return -1;
  }

  FILE* fp = fopen(filename, "wb");
  if (fp == NULL) {
    return -1;
  }

  // write exactly 1 struct chunk
  size_t written = fwrite(s, sizeof(Student), 1, fp);
  fclose(fp);

  return (written == 1) ? 0 : -1;
}

// Read raw struct bytes straight from binary file
int load_student_binary(const char* filename, Student* out) {
  if (out == NULL || filename == NULL) {
    return -1;
  }

  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    return -1;
  }

  // read exactly 1 struct chunk
  size_t read_count = fread(out, sizeof(Student), 1, fp);
  fclose(fp);

  return (read_count == 1) ? 0 : -1;
}

// Seek to the end of the file and get the offset in bytes
long file_size(const char* filename) {
  if (filename == NULL) {
    return -1;
  }

  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    return -1;
  }

  // go all the way to the end
  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return -1;
  }

  // where are we? that is our byte count
  long size = ftell(fp);
  fclose(fp);

  return size;
}
