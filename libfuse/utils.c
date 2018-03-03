#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "include/libtfs.h"

void display()
{
    START("display");
    int i;
    for (i = 0; i < file_count + 1; i++)
    {
        LOG("\n%s", files[i].path);
    }
}

bool valid_path(char* path)
{
    assert(path != NULL);
    assert(path[0] != '\0');

}

FILE *get_data_handle(blockno_t block, offset_t offset)
{
    START("*get_data_handle");
    numassert(offset >= 0 && offset < BLOCKSIZE, offset);
    numassert(block >= 0 && block < NUM_BLOCKS, block);
    assert(valid_path(backing_storage_path));
    FILE *handle = fopen(backing_storage_path, "rw+b");
    fseek(handle, (block * BLOCKSIZE) + offset, blocks_origin);
    return handle;

    END("*get_data_handle");
}

void sanity_check()
{
    assertd(backing_storage_path != NULL);
    numassert(file_count >= 0, file_count);
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        if (blocks[block].allocated == false)
        {
            numassert(blocks[block].next == -1, block);
        }
    }
    for(inode_t inode = 0; inode < file_count; inode++)
    {
        if(files[inode].used == true)
        {
            assertd(files[inode].path != NULL);
            numassert(files[inode].start_block >= 0 && files[inode].start_block <= NUM_BLOCKS, files[inode].start_block);
        }
    }
}

void dump_data()
{
    assertd(backing_storage_path != NULL);
    numassert(file_count >= 0, file_count);

    LOG("\n\n>>>>>>>>>>>>>>>>>>");
    LOG("\nFILE METADATA:");
    for(inode_t inode = 0; inode < file_count; inode++)
    {
        LOG("File %d:", inode);
        LOG1("used:%d", files[inode].used);
        LOG1("first:%d", files[inode].start_block);
        LOG1("path:%s", files[inode].path);
        LOG1("is_dir:%d", files[inode].is_dir);
    }

    LOG("\n\n>>>");

    LOG("\nBLOCK METADATA:");
    for (blockno_t block = 0; block < NUM_BLOCKS; block++)
    {
        LOG("Block %d:", block);
        LOG1("allocated:%d", blocks[block].allocated);
        LOG1("next:%d", blocks[block].next);

    }
    LOG("\n>>>>>>>>>>>>>>>>>>\n\n");
}

void init_logging()
{
    //log_file = fopen("log", 'a');
}