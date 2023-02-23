#!/binbash
echo "Testing TestArrayIndex. . ."
    ./obj/mcc test/cases/testArrayIndex.mC #> all.out
#   diff -s all.out ./test/exp/testArrayIndex.exp

echo "Testing TestFuncCallMatch. . ."
    ./obj/mcc test/cases/testFuncCallMatch.mC #> all.out
#   diff -s all.out ./test/exp/testFuncCallMatch.exp

echo "Testing TestMulDeclFunc. . ."
    ./obj/mcc test/cases/testMulDeclFunc.mC #> all.out
#   diff -s all.out ./test/exp/testMulDeclFunc.exp

echo "Testing TestMulDeclVars. . ."
    ./obj/mcc test/cases/testMulDeclVars.mC #> all.out
#   diff -s all.out ./test/exp/testMulDeclVars.exp

echo "Testing TestTypeMisMatch. . ."
    ./obj/mcc test/cases/testTypeMismatch.mC #> all.out
#   diff -s all.out ./test/exp/testTypeMismatch.exp

echo "Testing TestunDeclFunc. . ."
    ./obj/mcc test/cases/testUnDeclFunc.mC #> all.out
#   diff -s all.out ./test/exp/testUnDeclFunc.exp

echo "Testing testUnDeclVars. . ."
    ./obj/mcc test/cases/testUnDeclVars.mC #> all.out
#   diff -s all.out ./test/exp/testUnDeclVars.exp