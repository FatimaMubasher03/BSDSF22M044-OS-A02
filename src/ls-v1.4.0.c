/*
 * Programming Assignment 02: lsv1.4.0
 * Add column display (default), long listing (-l), horizontal display (-x),
 * and alphabetical sort to ls.
 *
 * Usage:
 *   $ lsv1.4.0
 *   $ lsv1.4.0 -l
 *   $ lsv1.4.0 -x
 *   $ lsv1.4.0 /home /etc
 *   $ lsv1.4.0 -l /etc /home
 *   $ lsv1.4.0 -x /etc
 *
 * Notes:
 * - Default (no -l or -x): shows column layout, “down then across” in alphabetical order.
 * - -l: shows full long listing (permissions, owner, group, size, date) in alphabetical order.
 * - -x: shows horizontal layout (left to right, wraps to next line if needed) in alphabetical order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <sys/ioctl.h>

extern int errno;

void do_ls(const char *dir, int mode);
static void mode_to_string(mode_t mode, char *str);
static void print_long(const char *dir, const char *name, int width_links, int width_user, int width_group, int width_size);
static void print_vertical(char **names, int count, int maxlen);
static void print_horizontal(char **names, int count, int maxlen);

/* ────────────── Comparison function for qsort ────────────── */
static int cmpstring(const void *a, const void *b)
{
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
}

enum display_mode { DEFAULT, LONG, HORIZONTAL };

int main(int argc, char const *argv[])
{
    int opt;
    enum display_mode mode = DEFAULT;

    while ((opt = getopt(argc, (char *const *)argv, "lx")) != -1)
    {
        switch (opt)
        {
            case 'l':
                mode = LONG;
                break;
            case 'x':
                mode = HORIZONTAL;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-x] [file...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind == argc)
    {
        do_ls(".", mode);
    }
    else
    {
        for (int i = optind; i < argc; i++)
        {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], mode);
            puts("");
        }
    }

    return 0;
}

/* ────────────── do_ls ────────────── */
void do_ls(const char *dir, int mode)
{
    struct dirent *entry;
    char **names = NULL;
    int count = 0, capacity = 0;
    int maxlen = 0;

    DIR *dp = opendir(dir);
    if (!dp) { perror("opendir"); return; }

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        if (count == capacity)
        {
            capacity = capacity == 0 ? 32 : capacity * 2;
            names = realloc(names, capacity * sizeof(char *));
            if (!names) { perror("realloc"); closedir(dp); return; }
        }

        names[count] = strdup(entry->d_name);
        if (!names[count]) { perror("strdup"); closedir(dp); return; }

        int len = strlen(entry->d_name);
        if (len > maxlen)
            maxlen = len;

        count++;
    }

    closedir(dp);

    if (count == 0)
    {
        free(names);
        return;
    }

    /* ────────────── Sort filenames alphabetically ────────────── */
    qsort(names, count, sizeof(char *), cmpstring);

    switch (mode)
    {
        case LONG:
        {
            long long total_blocks = 0;
            int max_links = 0, max_user = 0, max_group = 0, max_size = 0;

            for (int i = 0; i < count; i++)
            {
                char path[PATH_MAX];
                snprintf(path, sizeof(path), "%s/%s", dir, names[i]);

                struct stat st;
                if (lstat(path, &st) == -1)
                {
                    fprintf(stderr, "lstat error on %s: %s\n", path, strerror(errno));
                    continue;
                }

                total_blocks += st.st_blocks;

                char buf[64];
                int n = snprintf(buf, sizeof(buf), "%lu", (unsigned long)st.st_nlink);
                if (n > max_links) max_links = n;

                struct passwd *pw = getpwuid(st.st_uid);
                n = pw ? strlen(pw->pw_name) : snprintf(buf, sizeof(buf), "%u", st.st_uid);
                if (n > max_user) max_user = n;

                struct group *gr = getgrgid(st.st_gid);
                n = gr ? strlen(gr->gr_name) : snprintf(buf, sizeof(buf), "%u", st.st_gid);
                if (n > max_group) max_group = n;

                n = snprintf(buf, sizeof(buf), "%lld", (long long)st.st_size);
                if (n > max_size) max_size = n;
            }

            printf("total %lld\n", total_blocks / 2);

            for (int i = 0; i < count; i++)
                print_long(dir, names[i], max_links, max_user, max_group, max_size);

            break;
        }
        case HORIZONTAL:
            print_horizontal(names, count, maxlen);
            break;
        case DEFAULT:
        default:
            print_vertical(names, count, maxlen);
            break;
    }

    for (int i = 0; i < count; i++)
        free(names[i]);
    free(names);
}

/* ────────────── print_vertical ────────────── */
static void print_vertical(char **names, int count, int maxlen)
{
    struct winsize ws;
    int term_width = 80;
    if (isatty(STDOUT_FILENO) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        term_width = ws.ws_col;

    int spacing = 2;
    int col_width = maxlen + spacing;
    int ncols = term_width / col_width;
    if (ncols < 1) ncols = 1;

    int nrows = (count + ncols - 1) / ncols;

    for (int r = 0; r < nrows; r++)
    {
        for (int c = 0; c < ncols; c++)
        {
            int index = c * nrows + r;
            if (index >= count)
                break;

            printf("%-*s", col_width, names[index]);
        }
        printf("\n");
    }
}

/* ────────────── print_horizontal ────────────── */
static void print_horizontal(char **names, int count, int maxlen)
{
    struct winsize ws;
    int term_width = 80;
    if (isatty(STDOUT_FILENO) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        term_width = ws.ws_col;

    int spacing = 2;
    int col_width = maxlen + spacing;
    int x = 0;

    for (int i = 0; i < count; i++)
    {
        if (x + col_width > term_width)
        {
            printf("\n");
            x = 0;
        }
        printf("%-*s", col_width, names[i]);
        x += col_width;
    }
    if (x != 0)
        printf("\n");
}

/* ────────────── mode_to_string ────────────── */
static void mode_to_string(mode_t mode, char *str)
{
    str[0] = S_ISDIR(mode) ? 'd' :
             S_ISLNK(mode) ? 'l' :
             S_ISCHR(mode) ? 'c' :
             S_ISBLK(mode) ? 'b' :
             S_ISFIFO(mode)? 'p' :
             S_ISSOCK(mode)? 's' : '-';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';
}

/* ────────────── print_long ────────────── */
static void print_long(const char *dir, const char *name, int width_links, int width_user, int width_group, int width_size)
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dir, name);

    struct stat st;
    if (lstat(path, &st) == -1)
    {
        fprintf(stderr, "lstat error on %s: %s\n", path, strerror(errno));
        return;
    }

    char perms[11];
    mode_to_string(st.st_mode, perms);

    unsigned long nlinks = (unsigned long)st.st_nlink;

    char ownerbuf[64];
    struct passwd *pw = getpwuid(st.st_uid);
    snprintf(ownerbuf, sizeof(ownerbuf), "%s", pw ? pw->pw_name : "?");

    char groupbuf[64];
    struct group *gr = getgrgid(st.st_gid);
    snprintf(groupbuf, sizeof(groupbuf), "%s", gr ? gr->gr_name : "?");

    long long size = (long long)st.st_size;

    char timebuf[64];
    struct tm *tm = localtime(&st.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);

    char namebuf[PATH_MAX + 64];
    if (S_ISLNK(st.st_mode))
    {
        char target[PATH_MAX];
        ssize_t r = readlink(path, target, sizeof(target) - 1);
        if (r != -1)
        {
            target[r] = '\0';
            snprintf(namebuf, sizeof(namebuf), "%s -> %s", name, target);
        }
        else
        {
            snprintf(namebuf, sizeof(namebuf), "%s -> (unreadable)", name);
        }
    }
    else
    {
        snprintf(namebuf, sizeof(namebuf), "%s", name);
    }

    printf("%s %*lu %-*s %-*s %*lld %s %s\n",
           perms,
           width_links, nlinks,
           width_user, ownerbuf,
           width_group, groupbuf,
           width_size, size,
           timebuf,
           namebuf);
}
