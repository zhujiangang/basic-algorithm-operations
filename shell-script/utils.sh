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
  echo "{${FUNCNAME[0]}} input_txtfile=$input_txtfile"
  
  local total_lines=`wc -l "$input_txtfile" | awk '{print $1}'`
    
  local line=1
  local sline=`addzero ${line} 6`
  local thisline=
  echo -n "read line: ${sline}"
	while [ $line -ne $total_lines -a $total_lines -ne 0 ];
	#while read thisline  ## read $input_txtfile
	do		
	  # Search for included files:
	  thisline=`sed -n "$line"p "$input_txtfile"`	  
	  line=$(( $line + 1 ))
	  sline=`addzero ${line} 6`
	  echo -en "\b\b\b\b\b\b"
	  echo -n "${sline}"
	done  < "$input_txtfile"
	echo ; echo ; echo
}

readfile1 ()
{
  local input_txtfile="$1"
  echo "{${FUNCNAME[0]}} input_txtfile=$input_txtfile"
  
  local total_lines=`wc -l "$input_txtfile" | awk '{print $1}'`
    
  local line=0
  local sline=`addzero ${line} 6`
  local thisline=
	#while [ $line -ne $total_lines -a $total_lines -ne 0 ];
	while read thisline  ## read $input_txtfile
	do		
	  # Search for included files:
	  thisline=`sed -n "$line"p "$input_txtfile"`
	  echo -ne "\r                                                                                   \r"
	  echo -ne "thisline=${thisline}"
	  
	  start_with=`echo "$thisline" | grep -i ^#`
		if [ -n "$start_with" ] ; then
			echo " ==> $start_with"
			sleep 1s
		fi	  
	  line=$(( $line + 1 ))
	done  < "$input_txtfile"
	echo ; echo ; echo
}

full_path ()   # from Shea Martin, modified by scott dylewski
{
	if [ -f "$1" ]; then
	  DIR="$(dirname "$1")"
	  DIR=$(cd "$DIR" && pwd)
	  FILE="$(basename "$1")"
	  RSLT="$?"
	  echo "$DIR/$FILE"
	  return "$RSLT"
	elif [ -d "$1" ]; then
	  DIR="$1"
		FILE="$( cd "$DIR" && pwd)"
		RSLT=$?
	  echo "$FILE"
		return $RSLT
	else
    echo "unknown file: $1"
    return 1
	fi
}

trim0 ()
{
	local l_length="${#1}"
	local l_index=0
	local l_it=''
	if [ $l_length -le 1 ] ; then
		echo $1
	else
	  for (( l_index=0 ; l_index<l_length ; l_index++ )) ; do
	  	l_it="${1:${l_index}:1}"
	  	if [ $l_it -ne "0" ] ; then
	  		echo "${1:${l_index}}"
	  		break
	    fi
	  done
	fi
}


testfor ()  # testfor
{
	echo -n $@
	#if the number is started with "0", it will be considered as a Octal number.
	#[ -n "$2" ] && local start="$( printf %4.0f $2 )" || local start="1" 
	#local end="$( printf %4.0f $1 )"
	#correct code
	[ -n "$2" ] && local start=`trim0 $2` || local start="1" 
	local end=`trim0 $1`
	echo ": start=$start, end=$end"
	#for (( i_tmp=start ; i_tmp<=end ; i_tmp++ )) ; do
	for i_tmp in `seq $start 1 $end`; do
		echo -n "$i_tmp "
	done
	echo 
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
	#readfile  "$0"	
	#readfile1 "$0"
# 4. test full_path
	full_path ..
	
	trim0
	trim0 0
	trim0 9
	trim0 000000345677
	trim0 000000000012334
	trim0 123456
	trim0 1234560000
	
	testfor 0029
	testfor 0047
	testfor 0119 1
	testfor 0150 121
	testfor 0009
	testfor 0030 1
	testfor 0150 31
	testfor 0285 1
	testfor 0315 286
	testfor 0080 051
}

test
