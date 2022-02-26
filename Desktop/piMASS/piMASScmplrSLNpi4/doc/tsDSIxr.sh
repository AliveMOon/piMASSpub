#!/bin/bash

rpiTSid=$(xinput list | grep raspberrypi-ts | awk '{print $4}' | cut -c 4-)

DSIxr=$(xrandr --listmonitors | grep DSI )
DSIw=$( echo "$DSIxr" | awk -F'[ |/|x|+]' '{ print $5 }')
DSIh=$( echo "$DSIxr" | awk -F'[ |/|x|+]' '{ print $7 }')
DSIl=$( echo "$DSIxr" | awk -F'[ |/|x|+]' '{ print $9 }')
DSIu=$( echo "$DSIxr" | awk -F'[ |/|x|+]' '{ print $10 }')

HDMIxr=$(xrandr --listmonitors | grep HDMI )
sz=${#HDMIxr}
if [ $sz -eq 0 ]; then 
HDMIxr=$DSIxr 
fi
echo $DSIxr
echo $HDMIxr

HDMIw=$( echo "$HDMIxr" | awk -F'[ |/|x|+]' '{ print $5 }')
HDMIh=$( echo "$HDMIxr" | awk -F'[ |/|x|+]' '{ print $7 }')
HDMIl=$( echo "$HDMIxr" | awk -F'[ |/|x|+]' '{ print $9 }')
HDMIu=$( echo "$HDMIxr" | awk -F'[ |/|x|+]' '{ print $10 }')

DSIlw=$(bc <<<"scale=4;$DSIl+$DSIw" )
HDMIlw=$(bc <<<"scale=4;$HDMIl+$HDMIw" )
DpHlw=$(bc <<<"scale=4;$DSIlw/$HDMIlw" )
echo $DSIlw '/' $HDMIlw '=' $DpHlw
DpHw=$(bc <<<"scale=4;$DSIw/$HDMIw" )
echo $DSIw '/' $HDMIw '=' $DpHw

DSIuh=$(bc <<<"scale=4;$DSIu+$DSIh" )
HDMIuh=$(bc <<<"scale=4;$HDMIu+$HDMIh" )
DpHuh=$(bc <<<"scale=4;$DSIu/$DSIuh" )
echo $DSIuh '/' $HDMIuh '=' $DpHuh
DpHh=$(bc <<<"scale=4;$DSIh/$DSIuh" )
echo $DSIh '/' $HDMIh '=' $DpHh

xinput set-prop $rpiTSid 'Coordinate Transformation Matrix' 0 -$DpHw $DpHlw	$DpHh 0 $DpHuh   0 0 1

 