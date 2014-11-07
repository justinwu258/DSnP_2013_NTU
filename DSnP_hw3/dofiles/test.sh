./gnCalc -f do6 &> J_test
./gnCalc-ref -f do6 &> H_test
./gnCalc -f do4 &> J_test4
./gnCalc-ref -f do4 &> H_test4
./gnCalc -f do7 &> J_test7
./gnCalc-ref -f do7 &> H_test7
diff J_test4 H_test4 &> diff4.log
diff J_test  H_test &> diff.log
diff J_test7 H_test7 &> diff7.log
mv J_* ../Justin_testCase
mv H_* ../Justin_testCase
mv diff* ../Justin_testCase
 
