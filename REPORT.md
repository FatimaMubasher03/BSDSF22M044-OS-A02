# Feature-2: Long Listing Format (-l) — v1.1.0

**Concepts Covered:**  
File Metadata, System Calls (`stat`, `lstat`), User/Group Resolution (`getpwuid`, `getgrgid`), Time Formatting, Command-Line Argument Parsing.

**Implementation Summary:**  
Added support for the `-l` option to display detailed file information. When `-l` is passed, the program prints each file’s permissions, number of links, owner, group, size, modification time, and name — similar to the standard Unix `ls -l`.

**1. System Calls Used**
- `opendir()`, `readdir()`, `closedir()` — directory traversal  
- `lstat()` — file metadata retrieval  
- `getpwuid()` / `getgrgid()` — resolve user and group names  
- `strftime()` — time formatting  

**2. Key Difference Between v1.0.0 and v1.1.0**
| Version | Behavior |
|----------|-----------|
| v1.0.0 | Lists only file names (simple format). |
| v1.1.0 | Adds support for `-l`, displaying file details in long format. |

**Example Output:**
