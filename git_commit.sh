#!/bin/bash
#if argument 1 is not set or nul use default.
pram1VAR=${0:-"A small update"}
git add .
git status
git commit -m $pram1VAR
git push

