#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef struct Table_t {
  char* line;
  int offset;
  int length;
} Table;

int is_number(char* inp) {
  int res = 1;
  for (int i = 0; i < strlen(inp); i++) {
    if (!isdigit(inp[0])) {
      res = 0;
      break;
    }
  }
  return res;
}

void print_table(Table* my_table, int size) {
  for (int i = 0; i < size; i++) {
    if (table[i].line != NULL)
      printf("%d) [offset: %d length: %d] %s\n", i + 1, table[i].offset,
                                                        table[i].length,
                                                        table[i].line);
    else
    printf("%d) [offset: %d length: %d]\n", i + 1, table[i].offset,
                                                      table[i].length);
  }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("Wrong arguments count!\n");
        return 1;
    }

    char* name = argv[1];
    int file = open(name, O_RDONLY);
    if (file == -1) {
        perror("Error with opening file.\n");
        return 1;
    }

    Table* table = NULL;

    char buf[2];
    int pos = 0;
    int line_count = 1;
    table = (Table*)malloc(sizeof(Table));
    while (read(file, buf, 1) > 0) {
        if (buf[0] == '\n') {
          if (table == NULL) {
            table = (Table*)malloc((++line_count) * sizeof(Table));
          }
          else {
            table = (Table*)realloc(table, (++line_count) * sizeof(Table));
          }
          table[line_count - 1].line = NULL;
          table[line_count - 1].offset = pos + 1;
          table[line_count - 1].length = 0;
        }
        else {
          table[line_count - 1].length++;
        }
        pos++;
    }
    line_count--;

    int max_len = 0;
    for (int i = 0; i < line_count; i++) {
      if (table[i].length > 0) {
        table[i].line = (char*)malloc(table[i].length + 1);
        lseek(file, table[i].offset, 0);
        read(file, table[i].line, table[i].length);
        table[i].line[table[i].length] = '\0';
      }
    }

    print_table(table, line_count);

    char num[1024];
    int idx = 0;
    while (scanf("%s", num)) {
      if (strcmp(num, "") == 0) {
        printf("error\n");
      }
      if (!is_number(num)) {
        perror("Wrong line number!\n");
        continue;
      }
      idx = atoi(num);
      if (idx == 0)
        break;
      if (idx > line_count || idx < 0) {
        perror("Wrong line number!\n");
        continue;
      }
      if (table[idx - 1].length > 0)
        printf("%s\n", table[idx - 1].line);
      else
        printf("\n");
    }

    for (int i = 0; i < line_count; i++) {
        if (table[i].line != NULL)
          free(table[i].line);
    }
    free(table);

    close(file);

    return 0;
}
