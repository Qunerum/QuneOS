#include "kernel.h"
#include "../common/files.h"
#include "../common/utility.h"
#include "../common/terminal.h"
#include "../common/terminalCMDs.h"
#include "memory.h"

char* actualPath;

void logStart(char* msg, int state)
{
    if (state) { printc(msg, LIGHT_GREEN); } else { printc(msg, LIGHT_RED); }
    for (int i = 0; i < 69 - len(msg); i++) { printc(".", DARK_GRAY); }
    print(" [ ");
    if (state) { printc(" OK ", LIGHT_GREEN); } else { printc("ERR!", LIGHT_RED); }
    println(" ]");
}
void runCmd(char* input)
{
    char cmd[STR_LEN];
    char args[STR_LEN];
    split(input, ' ', cmd, args);
    for (int i = 0; i < cmd_count; i++) { if (is(cmd, cmds[i].cmd)) { cmds[i].handler(args); return; } }
    print("Unkown command!\n");
}
void createUser()
{
    print("Enter your name (max 32 characters): ");
    char* name = (char*)kmalloc(34);
    readLine(name, 32);
    print("Enter your password (max 32 characters): ");
    char* pass = (char*)kmalloc(33);
    readPass(pass, 32);
    println("Registering user...");
    if (!exists("sys")) { touch("sys"); editFile("sys", "0"); }

    char* usr = (char*)kmalloc(36);
    copyStr(usr, addStr("usr", readFile("sys")));

    makeDir(name);

    touch(usr);
    editFile(usr, addStr(addStr(name, "\n"), pass));

    editFile("sys", intToStr(strToInt(readFile("sys")) + 1));
}
void powerOff() {
    // QEMU
    outw(0x604, 0x2000);
    outw(0x4004, 0x3400);

    clear();
    printc("System Halted.\n", LIGHT_CYAN);
    print("You can now safely turn off your computer.");
    while(1) { __asm__("cli; hlt"); }
}
void kmain() {
    clear();
    init_memory();
    logStart("Kernel Memory", 1);
    logStart("Keyboard Driver", 1);
    logStart("VGA Text Mode", 1);
    actualPath = (char*)kmalloc(STR_LEN);
    copyStr(actualPath, "*");
    logStart("File System", 1);
    logStart("Applications (1/1):", 1);
    logStart("- Terminal", 1);
    printlne();

    println("/--------------------------------\\");
    print("|"); printb("  ", BLACK); printb("  ", BLUE); printb("  ", GREEN); printb("  ", CYAN);
    printb("  ", RED); printb("  ", MAGENTA); printb("  ", BROWN); printb("  ", LIGHT_GRAY);
    printb("  ", DARK_GRAY); printb("  ", LIGHT_BLUE); printb("  ", LIGHT_GREEN); printb("  ", LIGHT_CYAN);
    printb("  ", LIGHT_RED); printb("  ", LIGHT_MAGENTA); printb("  ", YELLOW); printb("  ", WHITE); print("|\n");
    println("\\--------------------------------/\n");
    printcb("[ SYSTEM LOADED ]\n\n", WHITE, GREEN);

    if (!exists("sys")) {
        println("Error! This appears to be your first run.");
        println("Please create your first user.");
        createUser();
        println("Registered!");
        printlne();
        clear();
    }
    int usrCount = strToInt(readFile("sys"));
    char* name = (char*)kmalloc(33);
    char* pass = (char*)kmalloc(33);
    char* id = (char*)kmalloc(36);
    for (int i = 0; i < usrCount; i++)
    {
        copyStr(id, addStr("usr", intToStr(i)));
        char* fl = readFile(id);
        split(fl, '\n', name, pass);
        printq("%d. %s\n", i, name);
        printq("Readed user '%s' with pass '%s'\n", name, pass);
    }
    int success = 0;
    char* u = (char*)kmalloc(17);
    char* enPass = (char*)kmalloc(33);
    while (!success)
    {
        println("\n(Enter to login user ID 0)");
        print("Login as: "); readLine(u, 16);
        int ui = 0;
        if (leni(u, ' ') >= 1) ui = strToInt(u);
        if (ui >= usrCount)
        {
            printc("Error! Cannot find a user with ID: ", LIGHT_RED); printcInt(ui, LIGHT_RED); printlne();
            println("Please choose again...");
        } else {
            copyStr(id, addStr("usr", intToStr(ui)));
            char* fl = readFile(id);
            split(fl, '\n', name, pass);
            int sucPass = 0;
            while (!sucPass)
            {
                print("Enter password: "); readPass(enPass, 32);
                if (is(enPass, pass)) { printc("Successfuly logged!\n", LIGHT_GREEN); changeDir(name); sucPass = 1; } else { printc("Wrong password! Try again.\n\n", LIGHT_RED); }
            }
            success = 1;
        }
    }
    kfree(u);
    kfree(id);
    kfree(enPass);
    kfree(name);
    kfree(pass);

    char cmd[CMD_MAX];
    while(1) {
        printc("'", GREEN); printc(actualPath, WHITE); printc("'", GREEN); print(": ");
        readLine(cmd, CMD_MAX);
        if (len(cmd) > 0) { add_to_history(cmd); }
        runCmd(cmd);
    }
}
