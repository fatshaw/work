result is for experiment of paper
test_for_flow is for the p2p project test

last test is on 26/03/2011:
parameters are:
chunksize=100
type=grid
sleep interval=1
training size=10000
k,v,r=3,len=100000
portrange 1024-65535 or port 80
fifo size=65100
fcapture interval=10
(it seems that interval=10 is much better than interval=30 which i still don't know its reason)
data:test_for_flow


last test is on 21/04/2011:
parameters are:
chunksize=1000,training size=10000,k,v,r=3,len=100000,fcapture interval=10,
gec sleep interval=5
all pcaps are replayed together and each file is replayed only once,instead of many times in the previous test which doesnot have a good result.



update on 2012.3.17
remove all test data(store in paper folders)
pcap:pcap captured by wireshark
script:scripts to start programms