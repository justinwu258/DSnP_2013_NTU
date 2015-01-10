./Jdo.sh &> J_test1.log
./Hdo.sh &> H_test1.log
time ./J_cirGate.sh &> J_testGate.log
time ./H_cirGate.sh &> H_testGate.log
./J_Wr.sh &> J_testWr.log
./H_Wr.sh &> H_testWr.log
diff J_test1.log H_test1.log
diff J_testGate.log H_testGate.log
diff J_testWr.log H_testWr.log
