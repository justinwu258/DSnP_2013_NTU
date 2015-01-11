./fraig -f doOpt &> J_Opt.txt
./ref/fraig -f doOpt &> H_Opt.txt
diff J_Opt.txt H_Opt.txt > diffOptL.log
vi diffOptL.log
echo "diff doOpt done"
