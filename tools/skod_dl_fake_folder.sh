#!/usr/bin/env bash
#
# skod_download_test.sh -
#
# This script will fake files to test skod download
#
# Copyright (c) 2016 by Hypsurus <hypsurus@mail.ru>
#
#

# Will create 10 folders.
for((i=0;i<=10;i++)) {
	mkdir "test${i}"
	cd "test${i}"
	echo "Hello this is test${i}." > "test$i.txt"
}
