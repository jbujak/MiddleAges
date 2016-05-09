#! /bin/bash
set -euo pipefail
IFS=$'\n\t'

readonly MINIMAL_START_DISTANCE=8

SIZE=10
TURNS=100
DELAY=1
X_1="UNDEFINED"
Y_1="UNDEFINED"
X_2="UNDEFINED"
Y_2="UNDEFINED"
AI_1=""
AI_2=""

main() {
	process_params "$@"
	prepare_pipes

	cat <&$GUI_INPUT >&$GUI_OUTPUT &
	pid=$!

	echo $'abc\ndef' >&$GUI_INPUT
	read a <&$GUI_OUTPUT
	echo $a
	read a <&$GUI_OUTPUT
	echo $a

	kill $pid
	wait -n $pid 2>/dev/null
}

prepare_pipes() {
	# Create gui input pipe
	PIPE=$(mktemp -u)
	mkfifo $PIPE
	# attach it to file descriptor 3
	exec 3<>$PIPE
	readonly GUI_INPUT=3

	# unlink (delete) the named pipe
	rm $PIPE

	# Create gui output pipe
	PIPE=$(mktemp -u)
	mkfifo $PIPE
	# attach it to file descriptor 4
	exec 4<>$PIPE
	readonly GUI_OUTPUT=4

	# unlink (delete) the named pipe
	rm $PIPE
}

process_params() {
	for (( i=1; i<="$#"; i+=2 ))
	do
		local current_param="${@:$i:1}"
		local next_param="${@:$(($i+1)):1}"


		if [ "$current_param" = "-n" ]
		then
			SIZE=$next_param
		fi

		if [ "$current_param" = "-k" ]
		then
			TURNS=$next_param
		fi

		if [ "$current_param" = "-s" ]
		then
			DELAY=$next_param
		fi

		if [ "$current_param" = "-p1" ]
		then
			X_1=`echo $next_param | cut -d , -f 1`
			Y_1=`echo $next_param | cut -d , -f 2`
		fi

		if [ "$current_param" = "-p2" ]
		then
			X_2=`echo $next_param | cut -d , -f 1`
			Y_2=`echo $next_param | cut -d , -f 2`
		fi

		if [ "$current_param" = "-ai1" ]
		then
			AI_1="$next_param"
		fi

		if [ "$current_param" = "-ai2" ]
		then
			AI_2="$next_param"
		fi
	done

	if [ "$X_1" = "UNDEFINED" ]
	then
		position_1_generate
	fi

	if [ "$X_2" = "UNDEFINED" ]
	then
		position_2_generate
	fi
}

position_1_generate() {
	# X_1 is drawn from range [1, $SIZE - 3].
	X_1=$(random 1 $(($SIZE - 3)))

	if [ $(($RANDOM%2)) = 0 ]
	then
		# Y_1 is drawn from range [1, $SIZE - $MINIMAL_START_DISTANCE]..
		Y_1=$(random 1 $(($SIZE - $MINIMAL_START_DISTANCE)))
	else
		# Y is drawn from range [1 + $MINIMAL_START_DISTANCE, $SIZE].
		Y_1=$(random $((1 + $MINIMAL_START_DISTANCE)) $SIZE)
	fi
}

position_2_generate() {
	# X_2 is drawn from range [1, $SIZE - 3].
	X_2=$(random 1 $(($SIZE - 1)))

	if [ $(abs $(($X_1 - $X_2)) ) -lt $MINIMAL_START_DISTANCE ] 
	then
		# Y_2 is drawn to satisfy condition |$Y_1 - $Y_2| > $MINIMAL_START_DISTANCE.
		if [ $Y_1 -gt $MINIMAL_START_DISTANCE ]
		then
			Y_2=$(random 1 $(($Y_1 - $MINIMAL_START_DISTANCE)))
		else
			Y_2=$(random $(($Y_1 + $MINIMAL_START_DISTANCE)) $SIZE)
		fi
	else
		# Y_2 is drawn from range [1, $SIZE].
		Y_2=$(( ( ($RANDOM << 15) | $RANDOM ) % ($SIZE) + 1))
	fi
}

random() {
	# Returns random number from range [$1, $2].
	echo "$(( ( ($RANDOM << 15) | $RANDOM) % ($2 - $1 + 1) + $1 ))"
}

abs() {
	if [ "$1" -ge 0 ]
	then
		echo "$1"
	else
		echo "$((-$1))"
	fi
}

main "$@"
