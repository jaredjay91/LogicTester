#include "logicToMath.C"
#include "evaluate.C"


string replaceP(string mathFormula="(~(P->~P)&&~(Q->~Q))->~(P->~Q)", string strP="P", int pval=1) {
  while (mathFormula.find(strP)>=0) {
    int posP = mathFormula.find(strP);
    if (posP<0) break;
    mathFormula = mathFormula.substr(0,posP)+intToString(pval)+mathFormula.substr(posP+strP.length());
  }
  return mathFormula;
}


void formulaTester(string Formula="(~(P->~P)&&~(Q->~Q))->~(P->~Q)", bool debugMode=false) {

  string mathFormula = logicToMath(Formula,false);

  //Replace P's and Q's with 0's and 1's and evaluate the truth values.
  cout << endl << "Evaluating the truth values..." << endl;
  for (int pval=0;pval<2;pval++) {
    for (int qval=0;qval<2;qval++) {
      cout << "P=" << pval << ", Q=" << qval << ": ";
      mathFormula = replaceP(mathFormula,"P",pval);
      mathFormula = replaceP(mathFormula,"Q",qval);
      cout << evaluate(mathFormula) << endl;
    }
  }
}
