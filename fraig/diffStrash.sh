#./fraig -f doStrash &> J_Strash.log
#./ref/fraig -f doStrash &> H_Strash.log
#diff J_Strash.log H_Strash.log #&> diffStrashL.log
##vi diffStrashL.log
#echo "diff doStrash done"
./fraig -f doStrash01 &> J_Strash01.log
./ref/fraig -f doStrash01 &> H_Strash01.log
diff J_Strash01.log H_Strash01.log #&> diffStrash01L.log
#vi diffStrash01L.log
echo "diff doStrash done"
./fraig -f doStrash02 &> J_Strash02.log
./ref/fraig -f doStrash02 &> H_Strash02.log
diff J_Strash02.log H_Strash02.log #&> diffStrash02L.log
#vi diffStrash02L.log
echo "diff doStrash02 done"
./fraig -f doStrash03 &> J_Strash03.log
./ref/fraig -f doStrash03 &> H_Strash03.log
diff J_Strash03.log H_Strash03.log #&> diffStrash03L.log
#vi diffStrash03L.log
echo "diff doStrash03 done"
./fraig -f doStrash04 &> J_Strash04.log
./ref/fraig -f doStrash04 &> H_Strash04.log
diff J_Strash04.log H_Strash04.log #&> diffStrash04L.log
#vi diffStrash04L.log
echo "diff doStrash04 done"
./fraig -f doStrash07 &> J_Strash07.log
./ref/fraig -f doStrash07 &> H_Strash07.log
diff J_Strash07.log H_Strash07.log #&> diffStrash07L.log
#vi diffStrash07L.log
echo "diff doStrash07 done"
./fraig -f doStrash08 &> J_Strash08.log
./ref/fraig -f doStrash08 &> H_Strash08.log
diff J_Strash08.log H_Strash08.log #&> diffStrash08L.log
#vi diffStrash08L.log
echo "diff doStrash08 done"
./fraig -f doStrash09 &> J_Strash09.log
./ref/fraig -f doStrash09 &> H_Strash09.log
diff J_Strash09.log H_Strash09.log #&> diffStrash09L.log
#vi diffStrash09L.log
echo "diff doStrash09 done"
./fraig -f doStrash10 &> J_Strash10.log
./ref/fraig -f doStrash10 &> H_Strash10.log
diff J_Strash10.log H_Strash10.log #&> diffStrash10L.log
#vi diffStrash10L.log
echo "diff doStrash10 done"
./fraig -f doStrash05 &> J_Strash05.log
./ref/fraig -f doStrash05 &> H_Strash05.log
diff J_Strash05.log H_Strash05.log #&> diffStrash05L.log
#vi diffStrash05L.log
echo "diff doStrash05 done"
./fraig -f doStrash06 &> J_Strash06.log
./ref/fraig -f doStrash06 &> H_Strash06.log
diff J_Strash06.log H_Strash06.log #&> diffStrash06L.log
#vi diffStrash06L.log
echo "diff doStrash06 done"
./fraig -f doStrashC7552 &> J_StrashC7552.log
./ref/fraig -f doStrashC7552 &> H_StrashC7552.log
diff J_StrashC7552.log H_StrashC7552.log #&> diffStrash06L.log
#vi diffStrash06L.log
echo "diff doStrashC7552 done"
#./ref/fraig -f doStrash05 &> H_Strash05.log
#./ref/fraig -f doDiffStrash05 &> H_DiffStrash05.log
#diff H_Strash05.log H_DiffStrash05.log &> diffDiffStrash05L.log
#vi diffDiffStrash05L.log
#echo "diff doDiffStrash05 done"
./fraig -f doMultiStrashOptimizeSweep &> J_doMultiStrashOptimizeSweep.log
./ref/fraig -f doMultiStrashOptimizeSweep &> H_doMultiStrashOptimizeSweep.log
diff J_doMultiStrashOptimizeSweep.log H_doMultiStrashOptimizeSweep.log &> diffdoMultiStrashOptimizeSweep.log
vi diffdoMultiStrashOptimizeSweep.log
echo "diff doMultiStrashOptimizeSweep done"
