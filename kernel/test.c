/**-----------------------------------------------------------------------------

 @file    file.c
 @brief   Implementation of file test functions
 @details
 @verbatim

  The file test functions in this file can be called in kmain() function.

 @endverbatim

 **-----------------------------------------------------------------------------
 */
#include <fs/vfs.h>
#include <fs/fat32.h>

#include <base/klog.h>

#include <test.h>

void file_test(void);

void dir_test(void)
{
    char* fn1 = "/";

    kprintf("List all files in \"%s\":\n", fn1);

    vfs_handle_t f1 = vfs_open(fn1, VFS_MODE_READWRITE);
    if (f1 != VFS_INVALID_HANDLE) {
        klogi("Open %s(%d) successed\n", fn1, f1);
        vfs_refresh(f1);

        vfs_dirent_t de = {0};
        while (true) {
            int64_t ret = vfs_getdent(f1, &de);
            if (ret <= 0) break;
            kprintf("%04d-%02d-%02d %02d:%02d \e[36m%5s\e[0m %s\n",
                  1900 + de.tm.year, de.tm.mon + 1, de.tm.mday,
                  de.tm.hour, de.tm.min,
                  de.type == VFS_NODE_FOLDER ? "<DIR>" : "",
                  de.name);
        }
        vfs_close(f1);
    } else {
        klogi("Open %s(%d) failed\n", fn1, f1);
    }
    file_test();
}

void file_test(void)
{
    char* fn1 = "/assets/desktop.bmp";
    vfs_handle_t f1 = vfs_open(fn1, VFS_MODE_READ);
    if (f1 != VFS_INVALID_HANDLE) {
        klogi("Successfully open %s with length %d\n", fn1, vfs_tell(f1));
        vfs_close(f1);
    } else {
        kloge("Open %s failed\n", fn1);
    }

    char* fn2 = "/HELLOWLD.TXT";
    vfs_handle_t f2 = vfs_open(fn2, VFS_MODE_READWRITE);
    if (f2 != VFS_INVALID_HANDLE) {
        char buff_read[1024] = {0};
        size_t readlen = vfs_read(f2, sizeof(buff_read) - 1, buff_read);
        klogi("Originally read %d bytes from %s(%d)\n%s\n", readlen, fn2, f2, buff_read);

        char buff_write[1024] = "(1) This is a test -- END";
        vfs_write(f2, strlen(buff_write), buff_write);
        readlen = vfs_read(f2, sizeof(buff_read) - 1, buff_read);
        klogi("Read %d bytes from %s(%d)\n%s\n", readlen, fn2, f2, buff_read);

        vfs_close(f2);
    } else {
        kloge("Open %s(%d) failed\n", fn2, f2);
    }

    vfs_handle_t f21 = vfs_open(fn2, VFS_MODE_READWRITE);
    if (f21 != VFS_INVALID_HANDLE) {
        char buff_read[1800] = {0};
        char buff_write[1800] = "(2) This is a test";

        size_t m = strlen(buff_write);
        for (; m < 120; m++) {
            buff_write[m] = 'A';
        }
        buff_write[m] = 'B';

        vfs_seek(f21, 10, SEEK_SET);
        vfs_write(f21, strlen(buff_write), buff_write);
        vfs_seek(f21, 0, SEEK_SET);
        size_t readlen = vfs_read(f21, sizeof(buff_read) - 1, buff_read);
        klogi("Read %d bytes from %s(%d)\n%s\n", readlen, fn2, f21, buff_read);
        vfs_close(f21);
    } else {
        kloge("Open %s(%d) failed\n", fn2, f21);
    }
}

