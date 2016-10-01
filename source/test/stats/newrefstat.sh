#!/usr/bin/env bash
# Update the reference stat with the current value

CURRSTATS="curr_allocs.csv"
STATSTORE="gwork_allocs.csv"
STATINFO="gwork_allocs.nfo"

# get details on the current commit
INFO=$(git show --quiet --pretty=fuller $(git rev-parse --verify HEAD))
echo "$INFO"

cat $CURRSTATS > $STATSTORE
echo "$INFO" > $STATINFO

