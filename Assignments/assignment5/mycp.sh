#!/bin/bash

# checklist:
# no option:  Working
# -R:         Working
# -t:         Working
# -Rt:        Working 
# -Rv:        
# -Rtv:

# initial variables
numArg=0
tag_var=.arc
choice=0

# handling options
while getopts "Rtvh" arg;
do
  case $arg in
    R)
      numArg=$((numArg+1))
      choice=$((choice+1))
      ;;
    t)
      numArg=$((numArg+1))
      choice=$((choice+2))
      ;;
    v)
      numArg=$((numArg+1))
      choice=$((choice+4))
      ;;
    h)
      echo -e "usage: mycp [options] <source> <destination>\\n"
      echo "options:"
      echo -e "-v\\t verbose output to destination, no files will be copied"
      echo -e "-t\\t tag copied files with .arc"
      echo -e "-R\\t recursive copy"
      echo -e "-h\\t show this help message and exit"
      exit
      ;;
    *)
      exit 1
      ;;
  esac
done

# handle both "-Rvt" and "-R -v -t" formats

# case1: mycp -Rt dir1 dir2   --> numArg=2, $#=3 ----> if numArg > ($#-2)
# case2: mycp -R -t dir1 dir2 --> numArg=2, $#=4 ----> else

if (($numArg > 0)); then
  if (($numArg > (($# - 2)))); then
    shift $(($OPTIND - 1))
  else
    shift $(($OPTIND - $numArg))
  fi
fi

# working? -- yes
source_file=$1
destination=$2

###################################################
# functions

# 1-check_files: passed two arguments: source and dest
#                check if arg1 is file, and arg2 is either
#                dir or non existing file, propmts overwrite
#                if file exists. called by regular_copy
# check_files() {

# }

# 2-copy: if passed two arguments, do regular copy
#         if passed three, do copy with tag
regular_copy() {
  if [ -f "$1" ]; then
    if [ -f "$2" ]; then
      read -p ""$2" is not a directory, do you want to overwtire? (y)/(n) " a
      if [ "$a" == "n" ]; then
        echo "Did not copy. EXIT."
        exit
      else
        if [ $3 == "2" ]; then
          ehco $2
          cp "$1" "$2$tag_var"
        else 
          cp "$1" "$2"
        fi
      fi
    else
      if [ "$3" == "2" ]; then
        echo "now copyyyyy `basename $1` to $2/`basename $1`.arc"
        cp "$1" "$2`basename $1`.arc"
      elif [ "$3" == "3" ]; then
        # echo "recursive tag copy"
        echo "$2/`basename $1`.arc"
        cp "$1" "$2/`basename $1`.arc"
      else
        cp "$1" "$2"
      fi
    fi
  else
    echo "-- ERROR: `basename "$1"` is not a file"
    exit
  fi
}

# 3-tagging: passed two arguments: source and dest,
#            adds suffix .arc then call regular copy.
tagging() {
  regular_copy $1 $2 2
}

# verbose_copy: passed two arguments, print regular structure

verbose_copy() {
  for file in "$1"/*; do
    if [ -d "$file" ]; then
      # print directory name and add dashes to
      # the destination of the recursive call
      echo "$3`basename $file`" >> "$2/cp.verbose"
      verbose_copy "$file" "$2" "--$3"
    else
      echo "$3`basename $file`" >> "$2/cp.verbose"
    fi
  done 
}

verbose_copy_tag() {
  # .
  # ├── dir1
  # │   ├── dir11
  # │   │   └── file11
  # │   ├── dir12
  # │   │   └── file12
  # │   ├── file1
  # │   ├── file2
  # │   └── file3
  # └── dir2
  #
  # assuming file cp.verbose is supposed
  # to be at destination directory
  for file in "$1"/*; do
    if [ -d "$file" ]; then
      # print directory name and add dashes to
      # the destination of the recursive call
      echo "$3`basename $file`.arc" >> "$2/cp.verbose"
      verbose_copy_tag "$file" "$2" "--$3"
    else
      echo "$3`basename $file`.arc" >> "$2/cp.verbose"
    fi
  done 
}

# recursive_check: does the checks needed in recursive functions
recursive_check() {
  # check if both parameters are directories
  if ! [ -d "$1" ]; then
    echo "-- ERROR: $1 is not a directory"
    exit
  elif ! [ -d "$2" ]; then  
    echo "-- ERROR: $2 is not a directory"
    exit
  fi

  case $3 in
    1)
      if ! [ -d "$2/`basename $1`" ]; then
        mkdir "$2/`basename $1`"
      fi
      recursive $1 "$2/`basename $1`"
    ;;
    2)
      if ! [ -d "$2/`basename $1`" ]; then
        mkdir "$2/`basename $1`.arc"
      fi
      recursive_tag $1 "$2/`basename $1`.arc"
    ;;
    3)
      echo "`basename $2`" >> "$2/cp.verbose"
      verbose_copy "$1" "$2" "--"
    ;;
    4)
      echo "`basename $2`.arc" >> "$2/cp.verbose"
      verbose_copy_tag "$1" "$2" "--"
  esac
}


# recursive:
recursive() {
  for file in "$1"/*; do
    if [ -d "$file" ]; then
      # create a new directory and make it
      # the destination of the recursive call
      mkdir "`pwd`/$2/`basename $file`"
      recursive "$file" "$2/`basename $file`"
    else
      regular_copy $file $2
    fi
  done    
}

# recursive_tag:
recursive_tag() {
  for file in "$1"/*; do
    if [ -d "$file" ]; then
      # create a new directory and make it
      # the destination of the recursive call
      mkdir "`pwd`/$2/`basename $file`.arc"
      recursive_tag "$file" "$2/`basename $file`.arc"
    else
      regular_copy $file $2 3
    fi
  done  
}

###################################################

# bitwise opetations?
# -R -> 1
# -t -> 10
# -v -> 100

# !! -- update: no need for bitwise operations
# e.g. -Rt -> 1 or 10 -> ((1 | 10)) -> 11 -> go into case

# new idea: at option handling, calculate variable $choice
# which will be sum of the options as follows:
# -R = 1, -t = 2, -v = 4.
# then go into another switch to perform options.

################################
# case will be as follows:     #
# if choice == 0: no options   #
# if choice == 1: -R           #
# if choice == 2: -t           #
# if choice == 3: -Rt          #
# if choice == 4: -v           #
# if choice == 5: -Rv          #
# if choice == 6: -tv          #
# if choice == 7: -Rtv         #
################################

# copying process
# perform checks (e.g. file1 exists, file2 directory, etc...) in cases?
case $choice in
  0) regular_copy $source_file $destination 1
    exit
    ;;
  1) # -R
    # recursive_check checks the arguments
    # then calls the recursive function.
    recursive_check $source_file $destination 1
    ;;
  2) # -t
    tagging $source_file $destination
    ;;
  3) # -Rt
     # "2" will work as a check in the recursive function
    recursive_check $source_file $destination 2
    ;;
  4) # -v
    verbose_copy $source_file $destination
    ;;
  5) # -Rv
    recursive_check $source_file $destination 3
    ;;
  6) # -tv
    ;;
  7) # -Rtv
    recursive_check $source_file $destination 4
    ;;
esac
