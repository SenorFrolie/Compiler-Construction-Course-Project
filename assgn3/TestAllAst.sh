#!/binbash
echo "Testing TestArrayIndex. . ."
    ./obj/mcc --ast test/cases/testArrayIndex.mC #> all.out
#   diff -s all.out ./test/exp/testArrayIndex.exp

echo "Testing TestFuncCallMatch. . ."
    ./obj/mcc --ast test/cases/testFuncCallMatch.mC #> all.out
#   diff -s all.out ./test/exp/testFuncCallMatch.exp

echo "Testing TestMulDeclFunc. . ."
    ./obj/mcc --ast test/cases/testMulDeclFunc.mC #> all.out
#   diff -s all.out ./test/exp/testMulDeclFunc.exp

echo "Testing TestMulDeclVars. . ."
    ./obj/mcc --ast test/cases/testMulDeclVars.mC #> all.out
#   diff -s all.out ./test/exp/testMulDeclVars.exp

echo "Testing TestTypeMisMatch. . ."
    ./obj/mcc --ast test/cases/testTypeMismatch.mC #> all.out
#   diff -s all.out ./test/exp/testTypeMismatch.exp

echo "Testing TestunDeclFunc. . ."
    ./obj/mcc --ast test/cases/testUnDeclFunc.mC #> all.out
#   diff -s all.out ./test/exp/testUnDeclFunc.exp

echo "Testing testUnDeclVars. . ."
    ./obj/mcc --ast test/cases/testUnDeclVars.mC #> all.out
#   diff -s all.out ./test/exp/testUnDeclVars.exp