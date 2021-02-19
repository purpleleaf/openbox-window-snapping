#!/bin/bash

mouse='G604'
diff=40
titleSize=34

onRelease () {
	local width=$1
	local heigth=$2
	local widthHalf=$(expr $width / 2)
	local heigthHalf=$(expr $heigth / 2)
	local posx=$3
	local posy=$4
	
if [ $x -lt $(expr $diff + $posx) ] && [ $y -lt $(expr $diff + $posy) ] # top left
then
	$(xdotool windowmove $activeWindow $posx $posy)
	$(xdotool windowsize $activeWindow $widthHalf $(expr $heigthHalf - $titleSize))
	
elif [ $x -gt $(expr $width - $diff - 1 + $posx) ] && [ $y -lt $(expr $diff + $posy) ] # top right
then
	$(xdotool windowmove $activeWindow $(expr $posx + $width / 2) $posy)
	$(xdotool windowsize $activeWindow $widthHalf $(expr $heigthHalf - $titleSize))
elif [ $x -lt $(expr $diff + $posx) ] && [ $y -gt $(expr $heigth - $diff - 1 + $posy) ] # bottom left
then
	$(xdotool windowmove $activeWindow $posx $(expr $heigthHalf  + $posy))
	$(xdotool windowsize $activeWindow $widthHalf $(expr $heigthHalf - $titleSize))
elif [ $x -gt $(expr $width - $diff - 1 + $posx) ] && [ $y -gt $(expr $heigth - $diff - 1 + $posy) ] # bottom right
then
	$(xdotool windowmove $activeWindow $(expr $posx + $width / 2) $(expr $heigth / 2 + $posy))
	$(xdotool windowsize $activeWindow $widthHalf $(expr $heigthHalf - $titleSize))
elif [ $y -lt $(expr $diff + $posy) ] #top
then
	$(xdotool windowmove $activeWindow $posx $posy)
	$(wmctrl -r :ACTIVE: -b add,maximized_vert,maximized_horz)
elif [ $y -gt $(expr $heigth - $diff - 1 + $posy) ] #bottom
then
	$(xdotool windowmove $activeWindow $posx $(expr $heigthHalf + $posy))
	$(xdotool windowsize $activeWindow $width $(expr $heigthHalf - $titleSize))
elif [ $x -lt $(expr $diff + $posx) ] #left
then
	$(xdotool windowmove $activeWindow $posx $posy)
	$(xdotool windowsize $activeWindow $widthHalf $(expr $heigth - $titleSize))
elif [ $x -gt $(expr $width - $diff - 1 + $posx) ] # right
then
	$(xdotool windowmove $activeWindow $(expr $widthHalf + $posx) $posy)
	$(xdotool windowsize $activeWindow $widthHalf $(expr $heigth - $titleSize))
else
	if [ "$CURRENT_STATE" = "VERT" ]
	then
		$(xdotool windowsize $activeWindow $(expr $width / 3) $(expr $heigth / 3))
	fi
fi
}

isDisplay() {
	local __resultvar=$5
	if [ $x -ge $3 ] && [ $x -le $(expr $1 + $3) ] && [ $y -ge $4 ] && [ $y -le $(expr $2 + $4) ]
	then
		local myresult=1
	else
		local myresult=0
	fi
	eval $__resultvar="'$myresult'"
}

activeWindow=$(xdotool getactivewindow)
MOUSE_ID=$(xinput --list | grep $mouse | grep pointer | grep -o 'id=[0-9]\+' | grep -o '[0-9]\+')
MOUSE_STATE=$(xinput --query-state $MOUSE_ID | grep 'button\[1\]' | grep -o 'up\|down')
CURRENT_STATE=$(xprop -id $activeWindow | grep "_NET_WM_STATE(ATOM)" | grep -o 'VERT\|')

if [ "$CURRENT_STATE" = "VERT" ]
then
	$(wmctrl -r :ACTIVE: -b remove,maximized_vert,maximized_horz)
fi


while [ "$MOUSE_STATE" = "down" ]
do
	MOUSE_STATE=$(xinput --query-state $MOUSE_ID | grep 'button\[1\]' | grep -o 'up\|down')
done

x=$(xinput --query-state $MOUSE_ID | grep 'valuator\[0\]' | grep -o '=[0-9]\+'| grep -o '[0-9]\+')
y=$(xinput --query-state $MOUSE_ID | grep 'valuator\[1\]' | grep -o '=[0-9]\+'| grep -o '[0-9]\+')



displays=$(xrandr -q | grep ' connected' | grep -o 'DP-[0-9]\+\|HDMI-[0-9]\+\|DVI-[0-9]\+')
for i in $displays
do
	coords=$(xrandr -q | grep $i | grep -o '[0-9]\+x[0-9]\++[0-9]\++[0-9]\+' | grep -o '[0-9]\+')
	j=0
	for g in $coords
	do
		coordsAr[$j]=$g
		j=$((j+1))
	done
	isDisplay ${coordsAr[@]} return
	if [ $return = 1 ]
	then
		break
	fi
done

onRelease ${coordsAr[@]}

