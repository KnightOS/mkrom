#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 0x100

struct input_file {
	char *file;
	unsigned int offset;
	struct input_file *next;
};

struct {
	char *rom_file;
	FILE *rom;
	unsigned int length;
	struct input_file *input;
	struct input_file *last_input;
} context;

void show_help() {
	printf(
		"mkrom - Writes individual files into a ROM image\n"
		"\n"
		"Usage: mkrom OUTPUT LENGTH FILE:ADDRESS...\n"
		"See `man 1 mkrom` for details.\n"
		"\n"
		"LENGTH is the target size of the output file OUTPUT\n"
		"FILE:ADDRESS pairs specify input files and their\n"
		"respective offset in the output file.\n"
		"\n"
		"Examples:\n"
		"\tCreate a 16KiB ROM image from two input files\n"
		"\t\tmkrom example.rom 0x4000 temp1.bin:0x0 temp2.bin:0x200\n"
	);
}

void parse_context(int argc, char **argv) {
	context.rom_file = NULL;
	context.input = NULL;
	context.length = 0;
	const char *errorMessage = "Invalid usage - see `mkrom --help`\n";
	int i;
	for (i = 1; i < argc; i++) {
		if (*argv[i] == '-') {
			if (strcmp(argv[i], "--help") == 0) {
				show_help();
				exit(0);
			} else {
				fputs(errorMessage, stderr);
				exit(1);
			}
		} else {
			if (context.rom_file == NULL) {
				context.rom_file = argv[i];
			} else if (context.length == 0) {
				if (sscanf(argv[i], "0x%x", &context.length) != 1 &&
					sscanf(argv[i], "%u", &context.length) != 1) {
					fputs(errorMessage, stderr);
					exit(1);
				}
			} else {
				char *colon = strrchr(argv[i], ':');
				unsigned int offset;
				if (colon == NULL ||
					(sscanf(colon+1, "0x%x", &offset) != 1 &&
					sscanf(colon+1, "%u", &offset) != 1)) {
					fputs(errorMessage, stderr);
					exit(1);
				}
				*colon = '\0';
				struct input_file *input = malloc(sizeof(struct input_file));
				input->offset = offset;
				input->file = argv[i];
				input->next = NULL;
				if (context.input == NULL) {
					context.input = input;
					context.last_input = input;
				}
				else {
					context.last_input->next = input;
					context.last_input = input;
				}
			}
		}
	}
	if (context.length == 0 || context.rom_file == NULL ) {
		fputs(errorMessage, stderr);
		exit(1);
	}
}

void write_file_to_image(struct input_file *input) {
	FILE *input_file;
	size_t amount_read;
	char buffer[BLOCK_SIZE];

	input_file = fopen(input->file, "rb");
	if (input_file == NULL) {
		fprintf(stderr, "Unable to open %s\n", input->file);
		exit(1);
	}

	fseek(context.rom, input->offset, SEEK_SET);
	while ((amount_read = fread(buffer, 1, BLOCK_SIZE, input_file)) > 0) {
		fwrite(buffer, amount_read, 1, context.rom);
	}

	fclose(input_file);
}

int main(int argc, char **argv) {
	struct input_file *input;
	parse_context(argc, argv);

	context.rom = fopen(context.rom_file, "wb");
	if (context.rom == NULL) {
		fprintf(stderr, "Unable to open %s\n", context.rom_file);
		exit(1);
	}
	while (context.length--) {
		fputc(0xff, context.rom);
	}
		
	input = context.input;
	while (input) {
		write_file_to_image(input);
		input = input->next;
	}

	fclose(context.rom);

	return 0;
}
