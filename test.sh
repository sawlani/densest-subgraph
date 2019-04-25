echo testing $1
time ./exactDSP-cpp/densest.exe <$1
time ./blah 20 $1
