./run &> J_test
./runH &> H_test
diff J_test H_test > ydiff.log

