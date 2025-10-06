# Custom `ls` Implementation in C

## Overview

This project implements a **multi-feature `ls` command** in C with the following capabilities:

- **Long listing (`-l`)**
- **Column display (default)**
- **Horizontal display (`-x`)**
- **Alphabetical sort**
- **Colorized output**
- **Recursive listing (`-R`)**

---

## Installation

### 1. Clone the repository

```bash
git clone https://github.com/username/ls-project.git
cd ls-project
2. Build using make
bash
Copy code
make
3. Run the program
bash
Copy code
./lsv1.6.0 [options] [directories]
Usage
Option	Description
None	Default column display (down then across)
-l	Long listing (permissions, owner, group, size, date)
-x	Horizontal layout
-R	Recursive listing
Combined options	e.g., ./lsv1.6.0 -lxR

Features
Dynamic Memory: Handles directories of varying sizes.

Column & Horizontal Display: Adjusts output according to terminal width.

Alphabetical Sorting: Entries are sorted using qsort().

Colorized Output: Filenames printed in different colors using ANSI escape codes.

Recursive Listing: Prints directory hierarchy recursively with headers.

Git Workflow
Each feature is developed on a separate branch.

Sequential merges into the main branch after feature completion.

Version tagging and release creation:

mathematica
Copy code
v1.1.0 → Long Listing
v1.2.0 → Column Display
v1.3.0 → Horizontal Display
v1.4.0 → Alphabetical Sort
v1.5.0 → Colorized Output
v1.6.0 → Recursive Listing
Build & Run Example
bash
Copy code
make
./lsv1.6.0 -lR /home/user/projects
This command displays all files and directories recursively in long listing format, sorted alphabetically, with color coding.
