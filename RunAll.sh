for f in datasets/*.txt
do
    echo ===running on===$f
    time ./dsp 20 $f "${f/%txt/out}"
done
