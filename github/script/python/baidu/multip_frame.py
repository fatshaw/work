#£¡/home/work/bin/python2.6

import os
import sys
import logging
from multiprocessing import Pool

core_num =
split_num =

def grep(file):
    logging.info('starting %s' % file)
    ret = {}
    
    for line in open(file):
        do something
        ret[file] = ...

    return ret

def split(dir, file):
    if not os.path.exists(dir):
        os.path.makedir(dir)

    count = 0
    index = 0
    writer = open(os.path.join(dir, 'part-%d' % index), 'w')
    for line in open(file):    
        if count == split_num:
            writer.close()
            index += 1
            count = 0
            writer = open(os.path.join(dir, 'part-%d' % index), 'w')

        writer.write(line)
        writer.write('\n')
        count += 1
        
def main():
    if len(sys.argv) < 2:
        print 'Usage: %s filename' % sys.argv[0]
        return

    logging.basicConfig(filename=¡¯%s.log¡® % sys.argv[0])
    temp_dir = 'temp_%s_%s' % (sys.argv[0], sys.argv[1])
    split(temp_dir, sys.argv[1])

    results = []
    for file in os.listdir(temp_dir):
        res = pool.apply_async(grep, (os.path.join(temp_dir, file),))
        results.append(res)

    for res in results:
        ...

if __name__ == '__main__':
    main()
