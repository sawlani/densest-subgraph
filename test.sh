echo testing $1
./exact 1000 <$1
./rps 40 $1
