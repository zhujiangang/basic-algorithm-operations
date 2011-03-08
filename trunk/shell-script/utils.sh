#!/bin/bash
#    Utilities for some useful common functions


debug=0  # 0 (low=default), 1 (some debug info), 2 (function info)


addzero ()
{
# $1 is the number to be processed, $2 is the least length required
  if [ ${#1} -lt $2 ] ; then
    diff=$(($2 - ${#1}))
#    diff=`expr $2 - ${#1}`
    for fr in `seq 1 ${diff}`; do
      echo -n "0"
    done
  fi
  echo $1
}

truncate ()
{
	local length="${#1}"
	local limit="$2"
	if [ $length -gt $limit ] ; then
		local pos=$(( $length - $limit ))
		echo "...${1:$pos}"
	else
		echo "$1"
	fi
}

readfile ()
{
local input_txtfile="$1"
total_lines=`wc -l "$input_txtfile" | awk '{print $1}'`
#total_lines=$(( $total_lines + 1 ))
line=1

#while [ $line -ne $total_lines -a $total_lines -ne 0 ];
while read thisline  ## read $input_txtfile
do
  # Search for included files:
#  thisline=`sed -n "$line"p "$input_txtfile"`
	mygrep=`echo "$thisline"` | grep
#  mygrep=`echo "$thisline" | grep -i ^include`
  mygrep_kb=`echo "$thisline" | grep -i :kenburns:`
  echo "$mygrep"
  echo "$mygrep_kb"
  line=$(( $line + 1 ))
done  < "$input_txtfile"
}

test ()
{
# 1. test addzero
  addzero     9 5
  addzero    10 5
  addzero   199 5
  addzero  9999 5
  addzero 12345 5
  addzero 123456 5
  junk=`addzero   199 5`
  echo "junk=$junk"
# 2. test truncate
	truncate 0123456789 3
# 3. test readfile
	readfile /C/diske/workfolder/test/input/input_file2.txt
}

test
