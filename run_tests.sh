#! /usr/bin/bash
range=$1
if [ ! -d run_log ]; then
	mkdir run_log
fi
p=$pwd
cd build
for((i=1;i<=$range;i++))
do
	echo run test ${i}
	ctest -VV -R "database_*" -j | grep sec> ../run_log/"test_result_$i"
done
cd $p

