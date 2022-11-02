#! /bin/bash

rm -f *.out
for inp in *.dat
do
    num=${inp/.dat/}
    echo 'Test' $num
    out=${inp/.dat/.out}
    ans=${inp/.dat/.ans}
    ./main < $inp > $out || exit 1
    diff -u $out $ans || exit 1
done

