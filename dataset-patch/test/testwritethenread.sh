#!/bin/sh

#
# Test various flavours of WRITE then READ
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
    if [ "${dsorg}" != "SEQ" ]; then
      # read the actual directory
      ./basicread "${hlq}" "${relds}" >>${actual_output}
      rc=$?
      if [ $rc -gt $maxrc ]; then
        maxrc=$rc
      fi
      fullname="${relds}(MEM)"
    else 
      fullname="${relds}"
    fi

    # 
    # Read the fullname that is empty - make sure it says 0 
    # bytes.
    # Call basicwrite to write 3 lines - the number of bytes
    # written varies whether it is length-prefixed or not.
    # Read the fullname back.
    #
    ./basicread "${hlq}" "${fullname}" >>${actual_output}
    rc=$?
    if [ $rc -gt $maxrc ]; then
      maxrc=$rc
    fi
    ./basicwrite "${hlq}" "${fullname}" >>${actual_output}
    rc=$?
    if [ $rc -gt $maxrc ]; then
      maxrc=$rc
    fi
    ./basicread "${hlq}" "${fullname}" >>${actual_output}
    rc=$?
    if [ $rc -gt $maxrc ]; then
      maxrc=$rc
    fi
  done
done

if [ $maxrc -gt 0 ]; then
  echo "One or more tests failed" >&2
fi
exit $maxrc
