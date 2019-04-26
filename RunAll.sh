for f in datasets/*.txt
do
  echo ===running on===$f
  time $1 $2 $f
done
