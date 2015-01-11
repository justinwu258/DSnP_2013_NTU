./J_opt07.sh &> J_Opt07.txt
./H_opt07.sh &> H_Opt07.txt
diff J_Opt07.txt H_Opt07.txt
echo "diff doOpt07 done"
./fraig -f doSweep &> J_Sweep.txt
./ref/fraig -f doSweep &> H_Sweep.txt
diff J_Sweep.txt H_Sweep.txt > diffSweepL.log
vi diffSweepL.log
echo "diff doSweep done"
