#include <argp.h>
#include <stdbool.h>

const char *argp_program_version = "programname programversion";
const char *argp_program_bug_address = "<your@email.address>";
static char doc[] = "Your program description.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = { 
    { "writeExample", 'x', 0, 0, "Write example.dob file"},
    { "readExample", 'y', 0, 0, "Read and parser example.dob file."},
    { "write", 'w', "example.dob", 0, "Write example file."},
    { "read", 'r', "example.dob", 0, "Read and parse file."},
    { 0 } 
};

struct arguments {
	enum { READ_EXAMPLE, WRITE_EXAMPLE, READ_FILE, WRITE_FILE } mode;
	char *filename;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
    case 'x': 
	arguments->mode = WRITE_EXAMPLE; 
	break;
    case 'y': 
	arguments->mode = READ_EXAMPLE; 
	break;
    case 'w': 
	arguments->mode = WRITE_FILE; 
	arguments->filename = arg;
	break;
    case 'r': 
	arguments->mode = READ_FILE;
	arguments->filename = arg;
	break;
    case ARGP_KEY_ARG: return 0;
    default: return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };


int main(int argc, char *argv[])
{
    struct arguments arguments;

    arguments.mode = WRITE_EXAMPLE;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

	switch(arguments.mode) {
		case WRITE_FILE:
			printf("Write file: %s\n", arguments.filename);
			break;
		case READ_FILE:
			printf("Read file: %s\n", arguments.filename);
			break;
	}
}

