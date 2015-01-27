make
./taskMgr -f dofiles/do1 &> jDo1.txt
./ref/taskMgr -f dofiles/do1 &> hDo1.txt
diff jDo1.txt hDo1.txt
echo "----- Do1 done -----"
./taskMgr -f dofiles/do2 &> jDo2_2.txt
./ref/taskMgr -f dofiles/do2 &> hDo2_2.txt
diff jDo2_2.txt hDo2_2.txt &> diffDo2_2.txt
#vi jDo2.txt
echo "----- Do2 done -----"
./taskMgr -f dofiles/do3 &> jDo3.txt
./ref/taskMgr -f dofiles/do3 &> hDo3.txt
diff jDo3.txt hDo3.txt
#vi jDo3.txt
echo "----- Do3 done -----"

#vi hBigTaski.txt
#echo "----- JBigTaski done -----"
