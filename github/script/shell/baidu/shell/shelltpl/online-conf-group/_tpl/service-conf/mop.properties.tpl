#MOPKeeper mop ===================================
mop.prodbmop.MOPName=prodbmop
mop.prodbmop.MORepoName=pmMORepo
mop.prodbmop.PDRepoName=pmPDRepo
mop.prodbmop.MODCacheSize=5
mop.prodbmop.PDCacheSize=5
mop.prodbmop.writeAgentTryLockIntervalMillSeconds=10
mop.prodbmop.writeAgentLockPingIntervalMillSeconds=10000

#MOPKeeper mop for testProDB ===================================
mop.prodbmopT.MOPName=prodbmopT
mop.prodbmopT.MORepoName=pmMORepoT
mop.prodbmopT.PDRepoName=pmPDRepoT
mop.prodbmopT.MODCacheSize=5
mop.prodbmopT.PDCacheSize=5
mop.prodbmopT.writeAgentTryLockIntervalMillSeconds=10
mop.prodbmopT.writeAgentLockPingIntervalMillSeconds=10000

#JsonDiff related config
jsondiff.backend=jackson

#MOP padding option
mop.paddingWithPow2=false
#1KW 100G
mop.padding.MORepo.paddingbytes=10240
#1KW 100G
mop.padding.PDRepo.paddingbytes=10240

# multi dmq sync options
mop.dmqsync.batchsize=10000

# parallelWriteAgent option
mop.parallelWriteAgent.dispatch.poolsize=5
mop.parallelWriteAgent.worker.poolsize=10
mop.parallelWriteAgent.retrytime=6

# parallelReadAgent option
mop.parallelReadAgent.receive.poolsize=5
mop.parallelReadAgent.receive.pooltotalsize=100
mop.requestbase.defaultBatchSize=10000
mop.requestbase.worker.poolsize=10
mop.requestbase.worker.singlethreadfetchsize=1000
mop.requestbase.worker.fetchpoolsize=10

# downstream option
mop.downstream.fetch.poolsize=10
mop.downstream.fetch.singlethreadfetchsize=1000
mop.downstream.fetch.fetchsize=5000
mop.downstream.process.firsthalf.poolsize=10
mop.downstream.process.secondhalf.poolsize=5

# MOP rollback setting
mop.rollback.limit=100000

# MOP Validation setting
mop.validation.anyfeedid=-1
mop.validation.debug=false

#MongoDB bug status
mop.mongodb.bugs.arraysort.fixed=false

#for debug
mop.updateMO=4
  # options: rawData, jsonDiff, jzDiff
mop.diff.method=jzDiff,rawData
  # it's to compare JsonDiff/JzDiff's size
mop.diff.comparesize=false
  # it's to store src in Diff, just for debug
mop.diff.storesrc=false

# cleanup options
mop.cleanup.batchsize=500

# stable version watcher
mop.stableversionwatcher.checkIntervalMilliSeconds=200
mop.stableversionwatcher.serverTags=primary,secondary,remote
mop.stableversion.delaylimit=100000

# backup utility
mop.backup.MOQueryBatchSize=5000
mop.backup.PDQueryBatchSize=5000
