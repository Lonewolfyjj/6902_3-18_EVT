#! /bin/bash

export LC_ALL=C.UTF-8
export LANG=C.UTF-8

usage() {
	echo "usage: ./mkimage.sh"
}

CUR_DIR=$(pwd)
TOOLS=$CUR_DIR/../tools
IMAGE=$(pwd)/Image

cp -r $CUR_DIR/rtthread.bin $IMAGE/rtthread.bin
$TOOLS/mkimage -f $IMAGE/amp.its -E -p 0xe00 $IMAGE/amp.img

echo 'Image: amp.img is ready.'
