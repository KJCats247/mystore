#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct FileEntry {
  char* name;
  char* path;
};

struct FileEntry* files = NULL;
size_t file_count = 0;

void store_add_files(const char* name, const char* path) {
  struct FileEntry* tmp = realloc(files, (file_count + 1) * sizeof(struct FileEntry));
  
  if (tmp != NULL) {
    files = tmp;

    size_t name_len = strlen(name);
    files[file_count].name = malloc(name_len + 1);
    if (files[file_count].name != NULL) {
      strcpy(files[file_count].name, name);
    }

    size_t path_len = strlen(path);
    files[file_count].path = malloc(path_len + 1);
    if (files[file_count].path != NULL) {
      strcpy(files[file_count].path, path);
    }

    file_count = file_count + 1;
  }
}

const char* store_get_path(const char* name) {
  for (size_t i = 0; i < file_count; i++) {
    if (strcmp(files[i].name, name) == 0) {
      return files[i].path;
    }
  }
  return NULL;
}

void store_cleanup() {
  for (size_t i = 0; i < file_count; i++) {
    free(files[i].name);
    free(files[i].path);
  }
  free(files);
  
  files = NULL;
  file_count = 0;
}

void save_to_disk() {
  char path[512];
  snprintf(path, sizeof(path), "%s/.mystore.db", getenv("HOME"));
  FILE* f = fopen(path, "w");
  if (f == NULL) return;

  for (size_t i = 0; i < file_count; i++) {
    fprintf(f, "%s %s\n", files[i].name, files[i].path);
  }
  fclose(f);
}

void load_from_disk() {
  FILE* f = fopen("/home/user/.mystore.db", "r");
  if (f == NULL) {
    printf("a\n");
    return;
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);

  if (size == 0) {
    fclose(f);
    return;
  }

  rewind(f);

  char n[256], p[256];
  while (fscanf(f, "%255s %255s", n, p) == 2) {
    store_add_files(n, p);
  }

  fclose(f);
}

int main(int argc, char* argv[]) {
  load_from_disk();

  if (argc == 3 && strcmp(argv[1], "--path") == 0) {
    const char* found_path = store_get_path(argv[2]);
    if (found_path != NULL) {
      printf("%s\n", found_path);
    } else {
      fprintf(stderr, "Fehler: Name '%s' nicht gefunden.\n", argv[2]);
    }
  } 
  else if (argc == 4 && strcmp(argv[1], "--add") == 0) {
    store_add_files(argv[2], argv[3]);
    save_to_disk();
    printf("Erfolgreich hinzugefuegt: %s -> %s\n", argv[2], argv[3]);
  } 
  else {
    printf("Verwendung:\n");
    printf("  --add <name> <pfad>   Eintrag speichern\n");
    printf("  --path <name>         Pfad suchen\n");
  }

  store_cleanup();
  
  return 0;
}
