time ./cirTest -f testall.do2 > usr.ref 2>&1 
time ./ref/cirTest -f testall.do2 > ref.ref 2>&1
#vim ref.ref
vimdiff usr.ref ref.ref
