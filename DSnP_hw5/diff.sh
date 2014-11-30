./J_arrayTest.sh &> J_diff
./H_arrayTest.sh &> H_diff
diff J_diff H_diff

