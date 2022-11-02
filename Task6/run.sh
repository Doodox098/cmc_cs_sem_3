#! /bin/bash

rm -f tests/*.out
for inp in tests/*.dat
do
    num=${inp/.dat/}
    echo 'Test' $num
    out=${inp/.dat/.out}
    ans=${inp/.dat/.ans}
    ./2 < $inp > $out
    diff -u $out $ans
done

