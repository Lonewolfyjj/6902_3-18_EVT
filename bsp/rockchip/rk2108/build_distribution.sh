#!/bin/bash

# how to use: ./build_distribuition.sh DEVICE_MODEL MAIN_VERSION

# 0. prepare sth
FILE='hl_config.h'
DEVICE_MODEL=$1
FOUND_MODEL=0
MAIN_VERSION='not_set'

echo "start prepare depencis"

line_num=1  
while read line; do  
    #Reading each line  
    result=$(echo $line | grep "if $DEVICE_MODEL")
    if [[ "$result" != "" ]]
    then
        result=$(echo $line | grep "A6902_VERSION")

        while [ "$result" == "" ]; do
            line=$(sed -n "$line_num"p $FILE)
            result=$(echo $line | grep "A6902_VERSION")
            if [[ "$result" != "" ]]
            then
                echo "found model version in line content : $line"
                FOUND_MODEL=1
                MAIN_VERSION=$(echo ${line##*' '})
                MAIN_VERSION=$(echo ${MAIN_VERSION%\"})
                MAIN_VERSION=$(echo ${MAIN_VERSION#\"})
                echo "build version is : " $MAIN_VERSION
                break;
            fi
            line_num=$((line_num+1))  
        done
        
    fi
    line_num=$((line_num+1))  
done < $FILE

if [ $FOUND_MODEL != 1 ]
then
    echo "model $DEVICE_MODEL not found error"
    exit
fi

echo "building" ${DEVICE_MODEL} ${MAIN_VERSION} "into ${DEVICE_MODEL}"_"${MAIN_VERSION}"

# 1. set hl_config.h content to the right version

# 1.1 read file length
echo "start configure the file"
line_num=1  
while read line; do  
    #Reading each line  
    result=$(echo $line | grep "//")
    if [[ "$result" != "" ]]
    then
        echo "finish set model"
        break;
    fi

    echo "Line No. $line_num : $line"  
    result=$(echo $line | grep "$DEVICE_MODEL")
    if [[ "$result" != "" ]]
    then
        echo "set model $DEVICE_MODEL enable"
        # 1.2 set model enable
        line=$(echo ${line%[0-1]})
        line=$(echo $line" 1")
        sed -i "$line_num d" "$FILE"
        sed -i "$line_num i $line" "$FILE"

    else
        echo "set model $DEVICE_MODEL disable"
        # 1.2 set model disable
        line=$(echo ${line%[0-1]})
        line=$(echo $line" 0")
        sed -i "$line_num d" "$FILE"
        sed -i "$line_num i $line" "$FILE"
    fi
    line_num=$((line_num+1))  
done < $FILE


# 2. copy the build resources
echo "start copy the depend src"

# 2.1 decide which device type (tx/rx) to build

result=$(echo $DEVICE_MODEL | grep "RX")
if [ "$result" != "" ]
then
    # RX
    SRC_DIR='audio_hollyland_a6902_rx'
else
    # TX
    SRC_DIR='audio_hollyland_a6902_tx'
fi

cp board/$SRC_DIR/rtconfig.txt rtconfig.h

# 3. run build.sh

# 3.1 cleanup the old build
if [ -f "Image/Firmware.img" ]; then
    rm -v Image/Firmware.img
fi

if [ -f "Image/rtthread.img" ]; then
    rm -v Image/rtthread.img
fi

if [ -f "Image/dist/${DEVICE_MODEL}_${MAIN_VERSION}.tar.gz" ]; then
    rm -v ${DEVICE_MODEL}"_"${MAIN_VERSION}".tar.gz"
fi

if [ -d "Image/dist/${DEVICE_MODEL}_${MAIN_VERSION}" ]; then
    rm -rvf Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}
fi

# 3.2 run build script
./build.sh

# 4. set file into the right version name "A6902_DEVICE_MODEL_MAIN_VERSION"
if [ -f "Image/rtthread.img" -a -f "Image/Firmware.img" -a -f "Image/rk2108_loader.bin" ]; then
    mkdir Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION} -p
    cp Image/rtthread.img Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}/
    cp Image/Firmware.img Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}/
    cp Image/rk2108_loader.bin Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}/
    tar -czvf Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}.tar.gz Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}
    rm -rvf Image/dist/${DEVICE_MODEL}"_"${MAIN_VERSION}
else
    echo "error of build, no output file"
fi