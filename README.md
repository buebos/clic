# Clic: Cli parser for C

## Introduction

This project aims to provide a simple mostly declarative API for parsing CLI a input. Using memebers from the same structs declared you will be able to retrieve the values assigned to the commands, flags or parameters.

## Declaring the CLI

This is the base struct where you can register all the commands or global flags within your app.

```c
Cli cli = {
    .name = "sample",
    .epilogue = "The sample clic app",

    .cmds_len = 1,
    .cmds = (CliCommand *[]){
        &Hello,
    },

    .flags_len = 0,
    .flags = NULL,
};
```

## Declaring a command

Commands are the main components of the CLI. They can be nested within other commands forming a tree like structure. When the parser finishes, commands will have a linked list like structure following the chain until the command tail or the target command.

```c
CliCommand Hello = {
    .id = "hello",
    .run = NULL,

    .aliases_len = 0,
    .aliases = NULL,

    .cmds_len = 0,
    .cmds = NULL,

    .params_len = 1,
    .params = (CliParam[]){
        {
            .id = "hello.name",
            .is_list = false,
            .is_nullable = true,
            .type = CLI_STR,

            .value_constraints_len = 0,
            .value_constraints = NULL,
        },
    },

    .flags_len = 0,
    .flags = NULL,
};
```

You can optionally assign a run pointer to the command. This should be used if you wish to handle a polymorphic flow for your program when running the command. It should be a pointer to the command main function you wish to run.

```c
int main(int argc, char** argv) {
    ...

    CliCommand *target = cli_get_cmd(&cli);

    return ((AppCommandRun)target->run)(&app);
}
```

Or you could also handle it with a conditional flow.

```c
int main(int argc, char** argv) {
    ...

    CliCommand *target = cli_get_cmd(&cli);

    if(strcmp(target->id, "build") == 0) {
        printf("[INFO]: Building...");
        ...
    } else if(strcmp(target->id, "start") == 0) {
        ...
    }

    return 0;
}
```
