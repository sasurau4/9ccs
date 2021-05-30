#include "9ccs.h"

/**
 * Globals
 * */
char *source;
char *filename;
Token *token;
Program *program;
Map *lvars;
Map *gvars;
Map *funcs;
Vector *found_strs;
int static_count;

char *read_file(char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        error("Cannot open %s: %s", path, strerror(errno));
    }

    if (fseek(fp, 0, SEEK_END) == -1) {
        error("%s: fseek: %s", path, strerror(errno));
    }
    size_t size = ftell(fp);
    if (fseek(fp, 0, SEEK_SET) == -1) {
        error("%s: fseek: %s", path, strerror(errno));
    }

    char *buf = calloc(1, size + 2);
    fread(buf, size, 1, fp);

    if (size == 0 || buf[size - 1] != '\n') {
        buf[size++] = '\n';
    }
    buf[size] = '\0';
    fclose(fp);
    return buf;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        return 1;
    }
    static_count = 0;

    filename = argv[1];
    source = read_file(filename);
    token = tokenize(source);
    found_strs = new_vec();
    program = parse();

    gen_program(program);
    return 0;
}