./fraig -f doOpt &> J_Opt.txt
./ref/fraig -f doOpt &> H_Opt.txt
diff J_Opt.txt H_Opt.txt > diffOptL.log
#vi diffOptL.log
echo "diff doOpt done"
./fraig -f doOpt01to07 &> J_Opt1to7.txt
./ref/fraig -f doOpt01to07 &> H_Opt1to7.txt
diff J_Opt1to7.txt H_Opt1to7.txt > diffOptL1to7.log
vi diffOptL1to7.log
echo "diff doOpt01to07 done"
