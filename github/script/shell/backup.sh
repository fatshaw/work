backup="nfs_backup"
for name in `ls`
do
        if [ -d "$name" ];then
                rm -f /home/"$name"/"$backup"
                ln -s /mnt/nfs/"$name" /home/"$name"/"$backup"
        fi
done
