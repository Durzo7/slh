GRAPH=$1
CYCLE=$2
SOLVE=~/cadical/build/cadical
TIMEOUT=3600

rm sol.log
./hcp-encode $GRAPH $CYCLE | timeout $TIMEOUT $SOLVE -w sol.log | grep "c total process"
head -n 1 $GRAPH
cat sol.log | grep "SATISFIABLE"
cat sol.log | grep "s SATISFIABLE" > result
if [ -s result ]
then
  ./hcp-decode $GRAPH sol.log
else
  echo "c TIMEOUT"
fi
echo
