#!/bin/bash
#
# Function that replaces notifications sent from one
# script run
#
# - it saves notification id in global variable $NOTIFY_ID

function notify {
    argument=""
    last_id=`echo $NOTIFY_ID | sed 's/[^0-9]//g'`
        
    if [ -n "$last_id" ]; then
        argument="-r $last_id"
        NOTIFY_ID=$last_id        
    fi
    
    NOTIFY_ID=`notify-desktop $argument "$@"`
}

#
# Function that replaces notifications sent from one script
# accross multiple runs of this script
# 
# - it saves notification id in temporary file

function notify_ {
    this=`basename $0`
    tmp_file=/tmp/${this//./-}.txt
    argument=""
    
    if [ -e $tmp_file ]; then
        last_id=`cat $tmp_file | sed 's/[^0-9]//g'`
        
        if [ -n "$last_id" ]; then
            argument="-r $last_id"
        fi
    fi
    
    notify-desktop $argument "$@" > $tmp_file
}


#
# You can use both functions the same way as notify-desktop
#
# notify -i up "Show icon" "Up icon is shown in notification"
# notify_ "Testing notification"
# 
