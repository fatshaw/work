#include <sys/statfs.h>
#include <stdio.h>
#include <error.h>
#include <sys/vfs.h>
#ifndef EXT3_SUPER_MAGIC
#define EXT3_SUPER_MAGIC 0xEF53
#endif
#define GB 1024*1024*1024
#define MB 1024*1024

int main()
{
        long long blocks,bfree,total,free;
        struct statfs64 fs;
        if(statfs64("/",&fs) < 0 )
        {
                perror("statfs");
                return -1;
        }
        printf("%ld\n",fs.f_type);
        printf("%ld\n",fs.f_bsize);
        printf("%ld\n",fs.f_blocks);
        printf("%ld\n",fs.f_bfree);
        printf("%ld\n",fs.f_bavail);
        printf("%ld\n",fs.f_files);
        printf("%d\n",fs.f_ffree);
        printf("%ld\n",fs.f_fsid);
        printf("%ld\n",fs.f_namelen);
        blocks = fs.f_blocks;
        bfree = fs.f_bfree;
        total = fs.f_bsize * blocks;
        free = fs.f_bsize * bfree;
        if(fs.f_type == EXT3_SUPER_MAGIC)
        {
                printf("total size = %lldGB\n",total/(GB));
                printf("total free size = %lldGB\n",free/(GB));
        }

}
