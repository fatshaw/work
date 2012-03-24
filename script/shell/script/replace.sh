#!/bin/bash
if true 
then
opcode=(DFS MFS FMS FMA FMSS FNMA FMAS FNMAS FNMS FNMSS FSEL FRES FRSQRE SQRT)
len=${#opcode[*]}
for((i=0;i<$len;++i))
do
	a=${opcode[$i]}
	sed -i "s/\(class.*\|[^a-z][, \t] *\)\<$a\>/\1${a}_R,$a/g" amer.schinfo; #cannot add p behind g,otherwise it print the replacement twice
	sed -i "s/\(.*update_cr.*\)}/\1,${a}_R}/g" amer.schinfo; #cannot add p behind g.
done

opcode=(FSEL FRES FRSQRE SQRT)
len=${#opcode[*]}
for((i=0;i<$len;++i))
do
	a=${opcode[$i]}
	b=`echo $a| tr "[A-Z]" "[a-z]"`
	sed -i "s/insn.*\(\<$a\>\).*\(exec.*\)/!&\nclass ${b}_f {\1,\1_R} \2/g" amer.schinfo
done

fi

opcode=(LFSCR LFSB1 LFSB0)
len=${#opcode[*]}
for((i=0;i<$len;++i))
do
	a=${opcode[$i]}
	b=`echo $a| tr "[A-Z]" "[a-z]"`
	sed -i "s/\(insn.*\)\(\<$a\>\)\(.*\)/&\n\1\2_R\3/g" amer.schinfo
    sed -i "s/\(class.*\|[^a-z][, \t] *\)\<$a\>/\1${a}_R,$a/g" amer.schinfo
done
