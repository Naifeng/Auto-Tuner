# while IFS=, read -r p1 p2 p3
# do
#     echo "$p1, $p2, $p3"
# done < candidate_set.csv

rm -f profiling_result.csv
while IFS=, read -r array
do
    # echo "${array[0]},${array[1]},${array[2]}" >> profiling_result.csv 
    echo "1,${array[@]},1" >> profiling_result.csv
done < training_set.csv