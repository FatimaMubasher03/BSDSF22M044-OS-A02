🧩 Feature-1: Project Setup and Initial Build — v1.0.0

Concepts Covered:
Git/GitHub, Project Scaffolding, Basic Makefile Usage

Implementation Summary:
The base version ls-v1.0.0 was implemented using directory traversal functions. The project structure was created according to the assignment specifications, including src, bin, obj, and man directories, and compiled successfully using a Makefile. The executable lists all files in a directory except hidden files.

System Calls and Functions Used:

opendir() – Opens a directory stream.

readdir() – Reads entries from a directory.

closedir() – Closes the directory stream.

errno – Used for error handling.

Key Behavior:
If no argument is passed, it lists the current directory (.).
If one or more paths are provided, it lists the contents of each directory.

Example Usage:

$ ./bin/ls
$ ./bin/ls /home
$ ./bin/ls /home/kali /etc


Example Output:

Directory listing of /etc :
passwd
shadow
hosts
...


Difference from Real ls:
This version only prints file names and does not show file permissions, sizes, owners, or timestamps.

⚙️ Feature-2: Long Listing Format (-l) — v1.1.0

Concepts Covered:
File Metadata, System Calls (stat, lstat), User/Group Resolution (getpwuid, getgrgid), Time Formatting, Command-Line Argument Parsing

Implementation Summary:
Added support for the -l option to display detailed file information. When -l is passed, the program prints each file’s permissions, number of links, owner, group, size, modification time, and name — closely resembling the standard Unix ls -l output.

System Calls and Library Functions Used:

opendir(), readdir(), closedir() — directory traversal

lstat() — retrieve file metadata without following symbolic links

getpwuid() — get username from user ID

getgrgid() — get group name from group ID

strftime() / ctime() — format modification timestamp

Internal Logic Overview:

Command-line argument -l detected using getopt().

When -l is present → call do_ls_long() for long format.

Otherwise → fallback to simple display via do_ls().

For each file:

Use lstat() to get metadata (struct stat).

Extract file type and permission bits from st_mode.

Resolve user/group names.

Format and print output in aligned columns.

Key Difference Between v1.0.0 and v1.1.0

Version	Behavior
v1.0.0	Lists only file names (simple format).
v1.1.0	Adds support for -l, displaying file details in long format.

Example Usage:

$ ./bin/ls -l
$ ./bin/ls -l /etc


Example Output (Sample):

-rw-r--r--  1 kali kali   2048 Oct  6 14:25 Makefile
drwxr-xr-x  2 kali kali   4096 Oct  6 14:30 src
-rwxr-xr-x  1 kali kali  12000 Oct  6 14:35 bin/ls

🧠 Report Questions
1. What is the crucial difference between stat() and lstat()? When is lstat() more appropriate to use?

stat() retrieves information about the target file, following symbolic links.

lstat() retrieves information about the link itself, not the target file.
In the context of ls, lstat() is more appropriate because it allows symbolic links to be displayed as links, rather than showing metadata of the files they point to.

2. How can bitwise operators and macros be used to extract file type and permission information from st_mode?

The st_mode field in struct stat is a bitmask containing file type and permission bits.
Bitwise & operations and predefined macros are used to extract this information.

Examples:

if (st.st_mode & S_IFDIR)
    printf("This is a directory\n");

if (st.st_mode & S_IRUSR)
    printf("User has read permission\n");


Common Macros:

S_IFDIR → Directory

S_IFREG → Regular file

S_IRUSR, S_IWUSR, S_IXUSR → Owner permissions

S_IRGRP, S_IROTH → Group/Other permissions

✅ Version Control Summary

Branching and Workflow:

Created feature branch:
feature-long-listing-v1.1.0

Implemented and tested new functionality.

Merged into main after verification.

Tag and Release:

Tag created:
v1.1.0

Release created on GitHub titled:
Version 1.1.0 — Complete Long Listing Format

Uploaded compiled binary (ls) under release assets.
