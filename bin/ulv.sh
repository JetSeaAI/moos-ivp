#!/bin/bash

#-------------------------------------------------------                        
#  Part 1: handle command-line arguments                          
#-------------------------------------------------------                        

MOOS_PORT="9321"
ALOG_FILES=""
for ARGI; do
#    echo "Argument: ${ARGI}"
#    echo "Suffix: ${ARGI##*.}"

    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
        printf "%s [SWITCHES] [ALOG Files]  \n" $0
        printf "  --help, -h         \n"
        printf "  --moos_port=PORT   \n"
        exit 0;
    elif [ "${ARGI##*.}" = "alog" ]; then
#        echo "Found an alog file: $ARGI"
        ALOG_FILES="$ALOG_FILES $ARGI" 
    elif [ "${ARGI:0:12}" = "--moos_port=" ] ; then
        MOOS_PORT="${ARGI#--moos_port=*}"
    else
        printf "Bad Argument: %s \n" $ARGI
        exit 0
    fi
done

echo "List of alog files: $ALOG_FILES"  
echo "MOOSDB Port Number: $MOOS_PORT"  

MOOSDB --moos_port=$MOOS_PORT &
uLogView --moos_port=$MOOS_PORT $ALOG_FILES &
uLogViewIPF --moos_port=$MOOS_PORT $ALOG_FILES &
uLogViewHelm --moos_port=$MOOS_PORT $ALOG_FILES

printf "Killing all processes ... \n"
kill %1 %2 %3
printf "Done killing processes ... \n"
