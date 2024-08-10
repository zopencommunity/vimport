#!/bin/sh

#
# Test various flavours of READ
#

#
# First, create datasets of different formats, 
# and also loop across all dsorgs
#

hlq=$(hlq)
dsf="FB"
dsv="VB"
dsu="U"
dsfa="FBA"
dsva="VBA"

expected_output="./testread.expected"
actual_output="/tmp/testread.$$.actual"

rm -f "${actual_output}"
maxrc=0
for fmt in $dsf $dsv $dsu $dsfa $dsva; do
  for dsorg in PDS LIBRARY SEQ ; do
    relds="DIO.${fmt}.${dsorg}"
    ds="${hlq}.${relds}"
    drm -f "${ds}"
    dtouch -r"${fmt}" -t"${dsorg}" "${ds}"
    ./basicread "${hlq}" "${relds}" >>${actual_output}
    rc=$?
    if [ $rc -gt $maxrc ]; then
      maxrc=$rc
    fi
    if [ "${dsorg}" != "SEQ" ]; then
      ./basicread "${hlq}" "${relds}(MEM)" >>${actual_output}
      rc=$?
      if [ $rc -gt $maxrc ]; then
        maxrc=$rc
      fi
      ./basicread "${hlq}" "${relds}(MEM)" >>${actual_output}
      rc=$?
      if [ $rc -gt $maxrc ]; then
        maxrc=$rc
      fi
    fi
  done
done

if [ $maxrc -gt 0 ]; then
  echo "One or more tests failed" >&2
fi
exit $maxrc
