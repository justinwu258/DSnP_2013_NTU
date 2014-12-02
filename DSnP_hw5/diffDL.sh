./J_dlistTest.sh &> J_diffDL
./H_dlistTest.sh &> H_diffDL
diff J_diffDL H_diffDL

