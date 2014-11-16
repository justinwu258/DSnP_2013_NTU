../memTest -f do2 > J2.log
../memTest -f do5 > J5.log
../ref/memTest -f do2 > H2.log
../ref/memTest -f do5 > H5.log
diff J2.log H2.log
diff J5.log H5.log
