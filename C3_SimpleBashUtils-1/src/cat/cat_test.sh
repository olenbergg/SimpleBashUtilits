#!/bin/bash
#1
./s21_cat test.txt >> s21_cat.txt
cat test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#2 flag -b
./s21_cat -b test.txt >> s21_cat.txt
cat -b test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#3 flag -e
./s21_cat -e test.txt >> s21_cat.txt
cat -e test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#4 flag -n
./s21_cat -n test.txt >> s21_cat.txt
cat -n test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#5 flag -s
./s21_cat -s test.txt >> s21_cat.txt
cat -s test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#6 flag -t
./s21_cat -t test.txt >> s21_cat.txt
cat -t test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#7 flags -ne
./s21_cat -ne test.txt >> s21_cat.txt
cat -ne test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#8 flags -bet
./s21_cat -be test.txt >> s21_cat.txt
cat -b -e test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#9 flags -set
./s21_cat -set test.txt >> s21_cat.txt
cat -set test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#10 flags -sv
./s21_cat -sv test.txt >> s21_cat.txt
cat -sv test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#11 flags -sn
./s21_cat -sn test.txt >> s21_cat.txt
cat -sn test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#12 flags -sb
./s21_cat -sb test.txt >> s21_cat.txt
cat -sb test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#13 flags -nb
./s21_cat -nb test.txt >> s21_cat.txt
cat -nb test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#14 flags -v
./s21_cat -v test.txt >> s21_cat.txt
cat -v test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#15 flags -ne
./s21_cat -n test.txt test.txt >> s21_cat.txt
cat -n test.txt test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
#16 flags -nsv
./s21_cat -nsv test.txt >> s21_cat.txt
cat -nsv test.txt >> cat.txt
diff -s s21_cat.txt cat.txt
rm s21_cat.txt cat.txt
