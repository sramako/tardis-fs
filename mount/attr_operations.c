#include "include/libtfs.h"
#include "mount.h"

int do_getattr(char *path, struct stat *st)
{
    assertd(path != NULL);
    START("do_getattr");
    LOGNULL("[getattr] Called\n");
    LOGNULL("\tAttributes of %s requested\n", path);
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);
    if (strlen(path) == 1 && strcmp(strdup(path), strdup("/")) == 0)
    {
        st->st_mode = S_IFDIR | 0777;
        st->st_nlink = 2;
        st->st_size = 1024;
        return 0;
    }
    int inode = getinode(strdup(path));
    LOGNULL("\tAttributes of %s[inode : %d] requested\n", path, inode);
    LOGNULL("ISD = %d\n", isd(inode));
    if (isd(inode) == 1)
    {
        LOGNULL("DIRECTORY\n");
        st->st_mode = S_IFDIR | 0777;
        st->st_nlink = 2;
        LOGNULL("RETURNING 0\n");
        return 0;
    }
    else if (isd(inode) == 0)
    {
        LOGNULL("REGULAR\n");
        st->st_mode = S_IFREG | 0777;
        st->st_nlink = 1;
        st->st_size = 1024;
        LOGNULL("RETURNING 0\n");
        return 0;
    }
    else if (isd(inode) == -1)
    {
        LOGNULL("REGULAR\n");
        st->st_mode = S_IFREG | 0777;
        st->st_nlink = 1;
        st->st_size = 1024;
        LOGNULL("RETURNING NOENT\n");
        return -ENOENT;
    }
    int i = 0, j = 0;
    for (i = 0; i < file_count; i++)
    {
        char fpath[PATH_LENGTH];
        fpath[0] = '/';
        strcat(fpath, get_path(i));
        char *fpath2 = fpath;
        if (strcmp(path, fpath2) == 0)
        {
            LOGNULL("RETURNING 0\n");
            return 0;
        }
        for (j = 0; j < 10; j++)
        {
            fpath[j] = '\0';
        }
    }
    LOGNULL("RETURNING NOENT\n");
    return -ENOENT;
}

int do_access(char *path, int mask)
{
    START("do_access");
    LOGNULL("do_access passthrough\n");
    return 0;
}

int do_setxattr(char *path, size_t size)
{
    START("do_setxattr");
    size = 1024;
    LOGNULL("do_setattr passthrough\n");
    return 0;
}

int do_utimens(const char* path, const struct timespec t)
{
    return 0;
}
