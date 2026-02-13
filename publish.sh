#!/bin/bash

git add .
in=$1
com=${in//-/ }
git commit -m $com
git push -u origin main
