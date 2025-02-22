#include <stddef.h>

#include <libc/string.h>
#include <libc/sysfunc.h>

static command_help_t help_msg[] = { 
    {"<help> echo",     "Display a specified string."},
};

int main(int argc, char *argv[])
{
    int i;
    char msg[3] = {0};

    for(i = 1; i < argc; i++) {
        sys_write(STDOUT, argv[i], strlen(argv[i]));
        if(i + 1 < argc){
            sys_write(STDOUT, " ", 1);
        } else {
            sys_write(STDOUT, "\n", 1);
        }
    }

    return 0;
}

