#!/bin/bash

if ! [ -d "outputs" ]; then
    eval mkdir "outputs"
else
    eval rm -r "outputs"
    eval mkdir "outputs"
fi

line=""
path={}
line_num=1
if ! [ -e results ]; then
    eval make check
fi
while read line
    do 
        path[$line_num]=${line:5}
        printf "[%d] " $line_num
        echo ${path[`expr $line_num`]}
        let "line_num = $line_num + 1"
    done < results
    

#echo $line_num
tc_num=112
repeat_cnt=100
i=0
failed_cnt=0
console=0

run_test()
{
    local tc_num=$tc_num
    local repeat_cnt=$repeat_cnt
    local path=${path[`expr $tc_num`]}
    local result= "FAIL"
    local fail_cnt=0
    local i=1
    if ! [ -d 'outputs/'${path##t*/} ]; then
        eval mkdir 'outputs/'${path##t*/}
    fi
    while [ "$i" -le $repeat_cnt ]
        do
            if [ -e $path'.output' ]; then
                eval rm $path'.output'
            fi
            printf "[%4d]%-40s" $i ${path##t*/}' running...'
            
	        if [ $console == 0 ]; then
	    	    eval make $path'.result' > 'outputs/'${path##t*/}'/result_'$i'.txt'
		        if [ -e 'outputs/'${path##t*/}'/result_'$i'.txt' ]; then
		            read result < $path'.result'
		            if [ $result != "PASS" ]; then
			            echo -e "[ \033[31m$result\033[0m ]"
			            let "fail_cnt = $fail_cnt + 1"
			            eval mv 'outputs/'${path##t*/}'/result_'$i'.txt' 'outputs/'${path##t*/}'/result_'$i'fail.txt'
	                else
			            echo -e "[ \033[32m$result\033[0m ]"
			            eval mv 'outputs/'${path##t*/}'/result_'$i'.txt' 'outputs/'${path##t*/}'/result_'$i'pass.txt'
		            fi
		        fi
            else
                printf "\n"
		        eval make $path'.result'
		        if [ -e $path'.result' ]; then
		            read result < $path'.result'
		            if [ $result != "PASS" ]; then
                        echo -e "[ \033[31m$result\033[0m ]"
			            let "fail_cnt = $fail_cnt + 1"
		            else
		    	        echo -e "[ \033[32m$result\033[0m ]"
		            fi
		        fi
	        fi
            let "i = $i + 1"
        done
    if [ $fail_cnt -gt 0 ]; then
        let "failed_cnt = $failed_cnt + 1"
        echo "$fail_cnt of $repeat_cnt tests failed in ${path##t*/} "
    fi
    return
}


total=`expr $line_num - 1` 
if ! [ $tc_num == 0 ]; then
    run_test
else
    tc_num=1
    while [ $tc_num -le $total ]
    do
        run_test
        let "tc_num = $tc_num + 1"
    done

    if [ $failed_cnt -gt 0 ]; then
        echo "$failed_cnt of $total tests failed. "
    else
        echo "All $total tests passed. "
    fi


fi
