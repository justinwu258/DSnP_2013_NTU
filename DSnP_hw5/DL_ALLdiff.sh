./do.all ./adtTest.dlist  &>   J_DL_doAll
./do.all ./ref/adtTest.dlist &> H_DL_doAll
diff J_DL_doAll H_DL_doAll

