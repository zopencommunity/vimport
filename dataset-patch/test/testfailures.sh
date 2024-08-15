#
# Test failure conditions (or partial failure conditions)
#
expected_output="./testfailures.expected"
actual_output="/tmp/testfailures.$$.actual"

rm -f "${actual_output}"

maxrc=0
#
# should be able to read this member
#
./basicread sys1 'maclib(yregs)' >>"${actual_output}" 2>&1
rc=$?
if [ $rc -gt $maxrc ]; then
  maxrc=$rc
fi

#
# should NOT be able to write to this member
#
./basicwrite sys1 'MACLIB(yregs)' >>"${actual_output}" 2>&1
rc=$?
if [ $rc -eq 0 ]; then
  echo "Write to MACLIB(YREGS) should have failed" >&2
  maxrc=8
fi

#
# Check a bunch of invalid dataset names are caught
#

./basicread sys1 'maclib(noclose' >>"${actual_output}" 2>&1
rc1=$?

./basicread sys1 'maclib(noclose))' >>"${actual_output}" 2>&1
rc2=$?

./basicread sys1 'nametoolong' >>"${actual_output}" 2>&1
rc3=$?

./basicread 'hlq' 'llq' >>"${actual_output}" 2>&1
rc4=$?

./basicread sys1 'the.dataset.name.is.too.long.to.process.i.would.expect' >>"${actual_output}" 2>&1
rc5=$?

./basicread sys1 'funny.chars.x#@!%' >>"${actual_output}" 2>&1
rc6=$?
