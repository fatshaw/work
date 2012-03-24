import os
f=open("/etc/samba/smb.conf")
path=""
paths=[]
for line in f:
	ind=line.find("path=")
	if ind >=0 :
		path=line[ind+len("path="):]
	ind = line.find("vfs objects")
	if ind >=0:
		if line.find("shadow_copy") > 0:
			path=path.lstrip(' ')
			path=path.rstrip("/\n")
			paths.append(path)

fp=open("/mnt/system/auditLog/download/samba/shadow_tmp.sh","w")
fp.write("#!/bin/sh\n\
#DEVNAME=/dev/2d5918be_vg\n\
#SHARE_PATH=/mnt/pools/A\n\
#VOLUME=lv16aa4935\n\
#MOUNT_POINT=A0\n\
#xfs_freeze -f $SHARE_PATH/$MOUNT_POINT\n\
#lvcreate -L10M -s -n $DATE $DEVNAME/$VOLUME\n\
#xfs_freeze -u $SHARE_PATH/$MOUNT_POINT\n\
#mkdir -p $SHARE_PATH/$SNAPSHOT\n\
#mount $DEVNAME/$DATE $SHARE_PATH/$SNAPSHOT -onouuid,ro\n\
PREFIX=@GMT-\n\
DATE=`date -u +%Y.%m.%d-%H.%M.%S`\n\
SNAPSHOT=$PREFIX$DATE\n"
)

volumes=dict()
for path in paths:
	ind=path.rfind("/")
	mount_point=path[0:ind]
	fd=os.popen("mount -l | grep -E \""+mount_point+" \" | awk '{print $1}'","r")
	str=fd.read(1024)
	devname="/dev/"+str[str.rfind("/")+1:str.find("-")]
	volume=devname+"/"+str[str.find('-')+1:-1]
	folder=path[ind+1:]
	if volume not in volumes:
		volumes[volume]=[mount_point]
		volumes[volume].append(folder)
	else:
		volumes[volume].append(mount_point)
		volumes[volume].append(folder)

for dev,path in volumes.items():
	if len(path) <=1:
		continue
	folder=path.pop()
	mount_point=path.pop()
	devname=dev[0:dev.rfind('/')]
	poolpath=mount_point+"/../"
	fp.write("xfs_freeze -f "+mount_point+"\n")
	fp.write("lvcreate -L10M -s -n $DATE "+dev+"\n")
	fp.write("xfs_freeze -u "+mount_point+"\n")
	fp.write("mkdir -p "+poolpath+"$SNAPSHOT\n")
	fp.write("mount "+devname+"/$DATE "+poolpath+"$SNAPSHOT -onouuid,ro\n")
	while 1 :
		fp.write("mkdir -p "+mount_point+"/"+folder+"/$SNAPSHOT\n")
		fp.write("mount --bind "+poolpath+"$SNAPSHOT/"+folder+" "+mount_point+"/"+folder+"/$SNAPSHOT\n")
		if len(path)>=2:
			folder=path.pop()
			mount_point=path.pop()
		else:
			break
fp.close()
