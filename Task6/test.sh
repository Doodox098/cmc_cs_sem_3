#! /bin/bash

for inp in tests/*.dat
do
    out=${inp/.dat/.ans}
    time ./2 < $inp > $out
done

