time ./fraig -f testall_noI.do2 > usr.ref 2>&1 
time ./ref/fraig -f testall_noI.do2 > ref.ref 2>&1
#vim ref.ref
vimdiff usr.ref ref.ref
