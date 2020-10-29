#!/bin/bash

for f in *.svg
do
    scour -i $f --enable-viewboxing --enable-id-stripping --enable-comment-stripping --shorten-ids --indent=none --create-groups --strip-xml-prolog --remove-metadata --remove-titles --strip-xml-prolog --remove-descriptive-elements
done
