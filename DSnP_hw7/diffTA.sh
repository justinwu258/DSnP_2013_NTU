./taskMgr -f doTA1  &> J_TA1.log
./ref/taskMgr -f doTA1 &> H_TA1.log
diff J_TA1.log H_TA1.log
echo "doTA1 done"
./taskMgr -f dofiles/do3  &> J_do3.log
./ref/taskMgr -f dofiles/do3 &> H_do3.log
diff J_do3.log H_do3.log
echo "do3 done"
