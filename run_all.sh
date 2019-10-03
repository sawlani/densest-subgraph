for f in datasets/*.txt
do
    echo ===running on===$f
    ./rps 40 < $f
    ./exact 1000 < $f
done

