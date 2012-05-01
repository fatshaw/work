#only need env_posix.c on this machine
rm -f /usr/home/waitman/downloads/mongo-c-driver/src/env_win32.c
rm -f /usr/home/waitman/downloads/mongo-c-driver/src/env_standard.c
gcc46 --std=c99 -Wall \
        /usr/home/waitman/downloads/mongo-c-driver/src/*.c \
        init.c -I/usr/home/waitman/downloads/mongo-c-driver/src/ \
        -I/usr/local/include -L/usr/local/lib -o init

