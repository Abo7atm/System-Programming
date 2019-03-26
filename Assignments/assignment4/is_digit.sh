re='^[0-9]+$'
read -p "please enter a number: " number
if [[ $number =~ $re ]]; then
   echo "$number is all digits"
else
    echo "$number is not all digits"
fi
exit