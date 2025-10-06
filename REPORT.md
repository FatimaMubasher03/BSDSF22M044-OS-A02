# REPORT.md – Programming Assignment 02: LS Project

**Student:** Fatima Mubasher  
**Instructor:** Dr. Muhammad Arif Butt, PhD  
**Course:** Operating System  

---

## Project Overview

This project implements a custom version of the `ls` command in C with multiple features developed iteratively:

| Version  | Feature                 | Description                                                                 |
|----------|------------------------|-----------------------------------------------------------------------------|
| v1.1.0   | Long Listing (-l)       | Display file metadata including permissions, owner, group, size, and modification time. |
| v1.2.0   | Column Display          | Default display in columns (“down then across”) adjusted to terminal width. |
| v1.3.0   | Horizontal Display (-x) | Left-to-right horizontal display wrapping to the next line as needed.       |
| v1.4.0   | Alphabetical Sort       | Sort directory entries alphabetically before displaying.                     |
| v1.5.0   | Colorized Output        | Print filenames in color based on file type using ANSI escape codes.        |
| v1.6.0   | Recursive Listing (-R)  | Recursively list subdirectories using the `-R` option.                      |

---

## Feature-wise Implementation & Report Questions

### v1.1.0 – Long Listing (-l)

**Implementation:**

- Used `lstat()` to get file metadata: `st_mode`, `st_uid`, `st_gid`, `st_size`, `st_mtime`.  
- Converted `st_mode` to string representation of permissions using `mode_to_string()`.  
- Resolved UID/GID to user and group names using `getpwuid()` and `getgrgid()`.  

**Report Questions:**

**How is `stat()` used to get file metadata?**  
`lstat()` fills a `struct stat` with all necessary info: file type, permissions, links, size, and timestamps.

**How are permissions displayed?**  
Using bitwise operations on `st_mode` to map bits to `rwx` characters.

---

### v1.2.0 – Column Display

**Implementation:**

- Determined terminal width using `ioctl()` and `TIOCGWINSZ`.  
- Calculated column width based on longest filename + spacing.  
- Displayed files “down then across” by calculating rows and columns.  

**Report Questions:**

**How does terminal width affect column layout?**  
The number of columns is `term_width / col_width`. Too few columns wrap entries vertically.

**Difference from horizontal display:**  
Vertical layout fills a column top-to-bottom, then moves right. Horizontal fills left-to-right and wraps.

---

### v1.3.0 – Horizontal Display (-x)

**Implementation:**

- Similar to column display but fills rows left-to-right.  
- Tracked current horizontal position to wrap lines when exceeding terminal width.  

**Report Questions:**

**Why is tracking horizontal position important?**  
Ensures proper wrapping and avoids line overflow.

**How is it different from default vertical columns?**  
Vertical layout prioritizes column-first; horizontal prioritizes row-first.

---

### v1.4.0 – Alphabetical Sort

**Implementation:**

- Read all directory entries into a dynamic array (`char **names`).  
- Sorted using `qsort()` with a comparison function:

Report Questions:

Why read all entries before sorting?
Sorting requires all items in memory to compare; streaming entries prevents sorting.

Drawbacks for huge directories:
High memory usage; slow malloc/realloc; potential fragmentation.

Purpose of qsort() comparison function:
qsort sorts arbitrary data; const void * allows generic pointers. Cast to char ** for string comparison.

---

v1.5.0 – Colorized Output

Implementation:

Used lstat() to determine file type.

Applied ANSI escape codes:

File Type	Color / Style	ANSI Code
Directory	Blue	\033[0;34m
Executable	Green	\033[0;32m
Tarballs (.tar/.gz/.zip)	Red	\033[0;31m
Symbolic links	Pink	\033[0;35m
Special files (device, socket)	Reverse video	\033[7m

Reset color after each filename: \033[0m.

Report Questions:

How do ANSI codes work?
Special character sequences sent to the terminal; modify foreground/background color and style.

Example green:

printf("\033[0;32m%s\033[0m", filename);


Which bits determine executable?
Check st_mode & (S_IXUSR | S_IXGRP | S_IXOTH) for owner, group, or other execute permissions.

---

v1.6.0 – Recursive Listing (-R)

Implementation:

Added -R option in getopt() loop.

Modified do_ls() to:

Print directory header (dirname:)

Read & sort entries

Display entries

For each directory (excluding . and ..), construct full path and recursively call do_ls()

Report Questions:

What is a base case?
The recursion stops when a directory contains no subdirectories or entries.

Why is full path essential?
Without full path, do_ls("subdir") may fail if the current working directory changes; relative path might not resolve correctly.

Memory Management

Dynamic arrays used: names = malloc()/realloc().

Each string duplicated: strdup().

Freed after use:

for (int i = 0; i < count; i++) free(names[i]);
free(names);

Git Workflow

Feature-per-branch approach:

feature-long-listing-v1.1.0
feature-column-display-v1.2.0
feature-horizontal-display-v1.3.0
feature-alphabetical-sort-v1.4.0
feature-colorized-output-v1.5.0
feature-recursive-listing-v1.6.0


Each branch committed individually.

Merged sequentially into main.

Tags created per version: v1.1.0 → v1.6.0.


