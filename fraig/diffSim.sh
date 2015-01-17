./fraig -f doSimulSim03 &> J_SimulSim03.log
./ref/fraig -f doSimulSim03 &> H_SimulSim03.log
diff J_SimulSim03.log H_SimulSim03.log &> myDiffSimulSim03.log
vi mydiffSimulSim03.log
echo "diff doSimulSim03 done"
./fraig -f doSimulSim10 &> J_SimulSim10.log
./ref/fraig -f doSimulSim10 &> H_SimulSim10.log
#diff J_SimulSim10.log H_SimulSim10.log &> myDiffSimulSim10.log
#vi mydiffSimulSim10.log
vimdiff J_SimulSim10.log H_SimulSim10.log #&> myDiffSimulSim10.log
echo "diff doSimulSim10 done"
#./fraig -f doSimulSim09 &> J_SimulSim09.log
#./ref/fraig -f doSimulSim09 &> H_SimulSim09.log
#diff J_SimulSim09.log H_SimulSim09.log &> myDiffSimulSim09.log
#vi myDiffSimulSim09.log
#echo "diff doSimulSim09 done"
#
#./fraig -f doStrashSim10 &> J_StrashSim10.log
#./ref/fraig -f doStrashSim10 &> H_StrashSim10.log
#diff J_StrashSim10.log H_StrashSim10.log #&> diffStrashSim10L.log
#echo "diff doStrashSim10 done"
