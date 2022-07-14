GRAPH=$1
VTX=`cat $GRAPH | grep "^p edge " | awk '{print $3}'`
BOUND=3
if [ $VTX -gt $BOUND ]
then
  BOUND=15
fi
if [ $VTX -gt $BOUND ]
then
  BOUND=105
fi
while [ $VTX -gt $BOUND ]
do
  BOUND=$(($BOUND * 2))
done

echo "c vertices: "$VTX" cycle:"$BOUND

./run-w-cycle.sh $GRAPH $BOUND
