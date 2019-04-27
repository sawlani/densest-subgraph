for f in datasets/*.txt
do
    echo ===running on===$f
    time ./dsp 40 $f "${f/%txt/out1}"
    time ./exactDSP-cpp/densest 250 $f "${f/%txt/out2}"
done

