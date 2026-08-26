// qemu-vault is an unofficial QEMU tool for saving and launching VMs by name.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char *home = getenv("HOME");

    if (home == NULL) {
        printf("HOME not found, can't save VMs!\n");
        return 1;
    }

    char config_dir[512];
    char config_file[512];

    snprintf(config_dir, sizeof(config_dir),
            "%s/.config/qemu-vault/", home);
    snprintf(config_file, sizeof(config_file),
            "%s/.config/qemu-vault/config", home);

    if (argc < 2) {
    printf("Usage: qemu-vault <command>\nFor help type: qemu-vault --help\n");
    return 1;
    }

    if (strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        printf("qemu-vault - Unofficial QEMU Virtual Machine Manager\n");
        printf("====================================================\n");
        printf("Usage:\n");
        printf("  qemu-vault <vm>                                      Run a saved VM\n");
        printf("  qemu-vault --help    | -h                            Show this screen\n");
        printf("  qemu-vault --list    | -l                            Show VM list\n");
        printf("  qemu-vault --add     | -a <name> <qemu parameters>   Save a VM\n");
        printf("  qemu-vault --version | -v                            Show the version\n");
        printf("  qemu-vault --remove  | -r <name>                     Remove a VM\n");
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0 ||
        strcmp(argv[1], "-v") == 0) {
        printf("qemu-vault: 1.0\n");
        return 0;
    }

    else if (strcmp(argv[1], "-r") == 0 ||
         strcmp(argv[1], "--remove") == 0) {

    if (argc < 3) {
        printf("Usage: qemu-vault -r <vm>\n");
        return 1;
    }

    FILE *config = fopen(config_file, "r");

    if (config == NULL) {
        printf("No config file found\n");
        return 1;
    }

    char temp_file[512];

    snprintf(temp_file, sizeof(temp_file),
             "%s.tmp", config_file);

    FILE *temp = fopen(temp_file, "w");

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
        remove(temp_file);
        printf("VM not found: %s\ncheck the name!\n", argv[2]);
        return 1;
    }

    remove(config_file);
    rename(temp_file, config_file);

    printf("VM removed: %s\n", argv[2]);

    return 0;
}

    else if (strcmp(argv[1], "-l") == 0 ||
         strcmp(argv[1], "--list") == 0) {

    FILE *config = fopen(config_file, "r");

    if (config == NULL) {
        printf("No saved VMs found!\n");
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
        mkdir(config_dir, 0755);
        FILE *config = fopen(config_file, "a");
        if (config == NULL) {
            printf("No config file found\n");
            return 1;
        }
        fprintf(config, "%s|%s\n", argv[2], argv[3]);

        fclose(config);

        printf("VM '%s' added!\n", argv[2]);
        return 0;
    }
    else {
    FILE *config = fopen(config_file, "r");

    if (config == NULL) {
        printf("No config file found\n");
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
        char *command = separator + 1;

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(vm_name, argv[1]) == 0) {
            printf("Starting '%s'...\n", vm_name);
            system(command);

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
