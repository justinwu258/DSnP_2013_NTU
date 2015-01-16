./fraig -f doStrashSim10 &> J_StrashSim10.log
./ref/fraig -f doStrashSim10 &> H_StrashSim10.log
diff J_StrashSim10.log H_StrashSim10.log #&> diffStrashSim10L.log
echo "diff doStrashSim10 done"
