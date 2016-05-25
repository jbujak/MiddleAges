#! /bin/bash

readonly PLAYER_1=1
readonly PLAYER_2=2

readonly GUI_EXE="sredniowiecze_gui_with_libs.sh"

readonly GUI_INPUT=3
readonly GUI_OUTPUT=4

readonly WIN=0
readonly DRAW=1
readonly LOSE=2

readonly OK=0
readonly ERROR=1

PLAYER_IN[$PLAYER_1]=5
PLAYER_IN[$PLAYER_2]=6
PLAYER_OUT[$PLAYER_1]=7
PLAYER_OUT[$PLAYER_2]=8

readonly MINIMAL_START_DISTANCE=8

readonly AI="AI"
readonly HUMAN="HUMAN"

SIZE=10
TURNS=100
DELAY=1
X_1="0"
Y_1="0"
X_2="0"
Y_2="0"
AI_1=""
AI_2=""
GUI_FLAGS=""
PLAYER=()

CURRENT_PLAYER=$PLAYER_1
NEXT_PLAYER=$PLAYER_2

main() {
	process_params "$@"
	if [ $? -ne 0 ]
	then
		exit $ERROR
	fi

	prepare_pipes

	./$GUI_EXE $GUI_FLAGS <&$GUI_INPUT >&$GUI_OUTPUT &
	local gui_pid=$!

	local ai1_pid=""
	if [ "$AI_1" != "" ]
	then
		"$AI_1" <&${PLAYER_IN[$PLAYER_1]} >&${PLAYER_OUT[$PLAYER_1]} &
		ai1_pid=$!
	fi

	local ai2_pid=""
	if [ "$AI_2" != "" ]
	then
		"$AI_2" <&${PLAYER_IN[$PLAYER_2]} >&${PLAYER_OUT[$PLAYER_2]} &
		ai2_pid=$!
	fi

	local game_pid=""
	game &
	game_pid=$!

	local ai1_checked=false
	local ai2_checked=false
	local gui_checked=false

	while true
	do
		# Waiting for AI and GUI to exit
		if [ "$AI_1" != "" ] && ! $ai1_checked && ! kill -0 $ai1_pid >/dev/null 2>&1
		then
			ai1_checked=true
			wait $ai1_pid
			result=$?

			if [ $result != $WIN ] && [ $result != $DRAW ] && [ $result != $LOSE ]
			then
				exit $ERROR
			fi
		fi

		if [ "$AI_2" != "" ] && ! $ai2_checked && ! kill -0 $ai2_pid >/dev/null 2>&1
		then
			ai2_checked=true
			wait $ai2_pid
			result=$?

			if [ $result != $WIN ] && [ $result != $DRAW ] && [ $result != $LOSE ]
			then
				exit $ERROR
			fi
		fi

		if ! kill -0 $gui_pid >/dev/null 2>&1 && ! $gui_checked
		then
			gui_checked=true
			wait $gui_pid
			result=$?

			kill $ai1_pid $ai2_pid $game_pid >/dev/null 2>&1

			if [ "$result" != "$OK" ]
			then
				kill $ai_1 $ai_2 >/dev/null 2>&1
				exit $ERROR
			fi
			exit $OK
		fi
		sleep 0.1
	done

	exit $OK
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
			PLAYER[$PLAYER_1]=$AI
		fi

		if [ "$current_param" = "-ai2" ]
		then
			AI_2="$next_param"
			PLAYER[$PLAYER_2]=$AI
		fi
	done

	if [ "$X_1" = "0" ]
	then
		generate_position X_1 Y_1 $X_2 $Y_2
	fi

	if [ "$X_2" = "0" ]
	then
		generate_position X_2 Y_2 $X_1 $Y_1
	fi

	if [ "$AI_1" = "" ]
	then
		GUI_FLAGS="$GUI_FLAGS -human1"
		PLAYER[$PLAYER_1]=$HUMAN
	fi

	if [ "$AI_2" = "" ]
	then
		GUI_FLAGS="$GUI_FLAGS -human2"
		PLAYER[$PLAYER_2]=$HUMAN
	fi

}

generate_position() {
	# $1, $2 - variables in which generated position is stored
	# $3, $4 - the other pair of X, Y

	# calculated result
	local posx=""
	local posy=""

	# the other pair of X, Y
	local otherx=$3
	local othery=$4

	if [ "$otherx" = "0" ]
	then
		# otherx and othery not set
		otherx=1
		othery=1
		
	fi

	# bounds for X and Y
	local lower_bound=1
	local lower_bound_y=1
	local upper_bound_x=$(($SIZE - 3))
	local upper_bound_y=$SIZE

	if [ $(($otherx + $MINIMAL_START_DISTANCE)) -le $upper_bound_x ]
	then
		# X can be greater than otherx
		posx=$(random $(($otherx + $MINIMAL_START_DISTANCE)) $upper_bound_x)
	elif [ $(($otherx - $MINIMAL_START_DISTANCE)) -ge $lower_bound ]
	then
		# X can be less than otherx
		posx=$(random $lower_bound $(($otherx - $MINIMAL_START_DISTANCE)) )
	fi

	if [ $(($othery + $MINIMAL_START_DISTANCE)) -le $upper_bound_y ]
	then
		# Y can be greater than othery
		posy=$(random $(($othery + $MINIMAL_START_DISTANCE)) $upper_bound_y)

		if [ -n $posx ]
		then
			# X could not be generated, but Y could
			posx=$(random 1 $upper_bound_x)
		fi
	elif [ $(($othery - $MINIMAL_START_DISTANCE)) -ge $lower_bound_y ]
	then
		# Y can be less than othery
		posy=$(random $lower_bound_y $(($othery - $MINIMAL_START_DISTANCE)) )

		if [ -n $posx ]
		then
			# X could not be generated, but Y could
			posx=$(random 1 $upper_bound_x)
		fi
	elif [ $posx ]
	then
		# Y could not be generated, but X could
		posy=$(random 1 $upper_bound_x)
	else
		# Neither X nor Y was generated - error
		exit $ERROR
	fi

	# save result to input variables
	eval $1=$posx
	eval $2=$posy

}

random() {
	# Returns random number from range [$1, $2].
	echo "$(( ( ($RANDOM << 15) | $RANDOM) % ($2 - $1 + 1) + $1 ))"
}

prepare_pipes() {
	# Create GUI input and output pipes
	local pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec $GUI_INPUT<>$pipe"
	rm $pipe

	local pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec $GUI_OUTPUT<>$pipe"
	rm $pipe

	# Create and redirect AI_1 input and output pipes
	local pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec ${PLAYER_IN[$PLAYER_1]}<>$pipe"
	rm $pipe

	local pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec ${PLAYER_OUT[$PLAYER_1]}<>$pipe"
	rm $pipe

	if [ "$AI_1" = "" ]
	then
		eval "exec $GUI_OUTPUT>&${PLAYER_OUT[$PLAYER_1]}"
		eval "exec ${PLAYER_IN[$PLAYER_1]}>/dev/null"
	fi

	# Create AI_2 input and output pipes
	local pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec ${PLAYER_IN[$PLAYER_2]}<>$pipe"
	rm $pipe

	local pipe=$(mktemp -u)
	mkfifo $pipe
	eval "exec ${PLAYER_OUT[$PLAYER_2]}<>$pipe"
	rm $pipe

	if [ "$AI_2" = "" ]
	then
		eval "exec $GUI_OUTPUT>&${PLAYER_OUT[$PLAYER_2]}"
		eval "exec ${PLAYER_IN[$PLAYER_2]}>/dev/null"
	fi
}

game() {
	echo "INIT $SIZE $TURNS $PLAYER_1 $X_1 $Y_1 $X_2 $Y_2" >&$GUI_INPUT
	echo "INIT $SIZE $TURNS $PLAYER_1 $X_1 $Y_1 $X_2 $Y_2" >&${PLAYER_IN[$PLAYER_1]}

	echo "INIT $SIZE $TURNS $PLAYER_2 $X_1 $Y_1 $X_2 $Y_2" >&$GUI_INPUT
	echo "INIT $SIZE $TURNS $PLAYER_2 $X_1 $Y_1 $X_2 $Y_2" >&${PLAYER_IN[$PLAYER_2]}

	while true
	do
		local current_command=""
		while [ "$current_command" != "END_TURN" ]
		do
			read current_command <&${PLAYER_OUT[$CURRENT_PLAYER]}

			echo "$current_command" >&${PLAYER_IN[$NEXT_PLAYER]}
			echo "$current_command"
			if [ "${PLAYER[$CURRENT_PLAYER]}" = "$AI" ]
			then
				echo "$current_command" >&$GUI_INPUT
			fi
		done

		sleep $DELAY
		next_player
	done
}

next_player() {
	NEXT_PLAYER=$CURRENT_PLAYER

	if [ "$CURRENT_PLAYER" = "$PLAYER_1" ]
	then
		CURRENT_PLAYER=$PLAYER_2
	else
		CURRENT_PLAYER=$PLAYER_1
	fi
}

main "$@"
