/* qemu-vault v1.1 source code
 ensure you have QEMU installed, if not install it first
 this software is distant from the official QEMU project, and is not affiliated with it in any way!
 this software is licensed under the MIT license, see LICENSE for details */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifndef __linux__
#ifndef __unix__
#error "qemu-vault currently supports only UNIX-like systems" 
#endif
#endif

static int qemu_check(void)
{
    return system("command -v qemu-system-x86_64 >/dev/null 2>&1") == 0; // cheap check
}

static int vm_name_exists(const char *cfg_file, const char *name) 
{
    FILE *config = fopen(cfg_file, "r");
    char line[1024];

    if (config == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), config) != NULL) {
        char *separator = strchr(line, '|');

        if (separator != NULL) {
            *separator = '\0';
            if (strcmp(line, name) == 0) {
                fclose(config);
                return 1;
            }
        }
    }

    fclose(config);
    return 0;
}

int main(int argc, char *argv[])
{
    char *home = getenv("HOME");

    if (home == NULL) {
        printf("HOME not found, can't save VMs!\n");
        return 1;
    }

    char cfg_dir[512];
    char cfg_file[512];

    snprintf(cfg_dir, sizeof(cfg_dir),
            "%s/.config/qemu-vault/", home);
    snprintf(cfg_file, sizeof(cfg_file),
            "%s/.config/qemu-vault/config", home);

    if (argc < 2) {
    printf("Usage: qemu-vault <command>\nFor help type: qemu-vault --help\n");
    return 1;
    }

    if (strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        printf("qemu-vault\n");
        printf("==========\n"); 
        printf("Usage:\n");
        printf("  qemu-vault <vm>                                      Run a saved VM\n");
        printf("  qemu-vault --help    | -h                            Show this screen\n");
        printf("  qemu-vault --list    | -l                            Show VM list\n");
        printf("  qemu-vault --add     | -a <name> <qemu parameters>   Save a VM\n");
        printf("  qemu-vault --version | -v                            Show the version\n");
        printf("  qemu-vault --remove  | -r <name>                     Remove a VM\n");
        printf("Config file: '%s' can be edited to modify VM parameters\n", cfg_file);
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0 ||
        strcmp(argv[1], "-v") == 0) {
        printf("qemu-vault, version: 1.1, Copyright (c) 2026 furkanorth\n");
        printf("This software is licensed under the MIT license\n");
        printf("This software is distant from the official QEMU project, and is not affiliated with it in any way!\n");
        return 0;
    }

    else if (strcmp(argv[1], "-r") == 0 ||
         strcmp(argv[1], "--remove") == 0) {

    if (argc < 3) {
        printf("Usage: qemu-vault -r <vm>\n");
        return 1;
    }

    FILE *config = fopen(cfg_file, "r");

    if (config == NULL) {
        printf("No config file found, could be a path error\n");
        return 1;
    }

    char tmp_file[512];

    snprintf(tmp_file, sizeof(tmp_file),
             "%s.tmp", cfg_file);

    FILE *temp = fopen(tmp_file, "w");

    if (temp == NULL) {
        perror("No temp file found");
        fclose(config);
        return 1;
    }

    char line[1024];
    int found = 0;

    while (fgets(line, sizeof(line), config) != NULL) {

        char copy[1024];
        strcpy(copy, line);

        char *separator = strchr(copy, '|');

        if (separator == NULL) {
            fputs(line, temp);
            continue;
        }

        *separator = '\0';

        if (strcmp(copy, argv[2]) == 0) {
            found = 1;
            continue;
        }

        fputs(line, temp);
    }

    fclose(config);
    fclose(temp);

    if (!found) {
        remove(tmp_file);
        printf("VM not found: %s\ncheck the name!\n", argv[2]);
        return 1;
    }

    remove(cfg_file);
    rename(tmp_file, cfg_file);

    printf("VM removed: %s\nTo edit the config file : %s\n", argv[2], cfg_file);

    return 0;
}

    else if (strcmp(argv[1], "-l") == 0 ||
         strcmp(argv[1], "--list") == 0) {

    FILE *config = fopen(cfg_file, "r");

    if (config == NULL) {
        printf("Config file not created but can be created with -a flag\n");
        return 0;
    }

    char line[1024];

    printf("Available VMs:\n");

    while (fgets(line, sizeof(line), config) != NULL) {
        char *separator = strchr(line, '|');

        if (separator != NULL) {
            *separator = '\0';
            printf("  %s\n", line);
        }
    }

    fclose(config);
    return 0;
}

    else if (strcmp(argv[1], "--add") == 0 ||
             strcmp(argv[1], "-a") == 0) {
        if (argc < 4) {
        printf("Usage: qemu-vault -a <name> <qemu parameters>\n");
        return 1;
    }

        if (strchr(argv[2], '|') != NULL || strchr(argv[2], '\n') != NULL) {
            printf("VM name cannot contain '|' or a newline\n");
            return 1;
        }

        if (vm_name_exists(cfg_file, argv[2])) {
            printf("A VM named '%s' already exists\nPlease type another name\n", argv[2]);
            return 1;
        }

        mkdir(cfg_dir, 0755); 
        FILE *config = fopen(cfg_file, "a");
        if (config == NULL) {
            printf("No config file found\n");
            return 1;
        }
        fseek(config, 0, SEEK_END);
            if (ftell(config) == 0) {
                fprintf(config, "# qemu-vault config file\n# Format: <vm_name>|<qemu_parameters>\n# This file can be edited manually\n");
            }
        fprintf(config, "%s|%s\n", argv[2], argv[3]);

        fclose(config);

        printf("VM '%s' added!\nTo edit its parameters, modify the config file : %s\n", argv[2], cfg_file);
        return 0;
    }
    else {
    FILE *config = fopen(cfg_file, "r");

    if (config == NULL) {
        printf("No config file found, could be a path error\n");
        return 1;
    }

    char line[1024];
    int found = 0;

    while (fgets(line, sizeof(line), config) != NULL) {

        char *separator = strchr(line, '|');

        if (separator == NULL) {
            continue;
        }

        *separator = '\0';

        char *vm_name = line;
        char *qemu_command = separator + 1;

        qemu_command[strcspn(qemu_command, "\n")] = '\0';

        if (strcmp(vm_name, argv[1]) == 0) {
            if (!qemu_check()) {
                fprintf(stderr,
                        "QEMU not found, needs to be installed. if already installed, check the path\n");
                fclose(config);
                return 1;
            }

            printf("Starting '%s'...\n", vm_name);
            system(qemu_command);

            found = 1;
            break;
        }
    }

    fclose(config);

    if (!found) {
        printf("no VM found: %s\ncheck the name!\n", argv[1]);
        return 1;
    }

    return 0;
}
    return 0;
} 
