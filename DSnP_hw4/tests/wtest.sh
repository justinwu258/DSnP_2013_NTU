../memTest -f do1 &> J1.log
../memTest -f do2 &> J2.log
../memTest -f do4 &> J4.log
../memTest -f do5 &> J5.log
../ref/memTest -f do1 &> H1.log
../ref/memTest -f do2 &> H2.log
../ref/memTest -f do4 &> H4.log
../ref/memTest -f do5 &> H5.log
diff J1.log H1.log 
diff J2.log H2.log 
diff J4.log H4.log 
diff J5.log H5.log
