#include "intToString.C"

  // 1 = TRUE
  // 0 = FALSE

  // P = 0 or 1.
  // P*P = P

  //    || = OR.      P || Q = ~(~P||~Q) = 1-(1-P)*(1-Q)
  //    && = AND.     P && Q = P*Q
  //    -> = IMPLIES. P -> Q = 1-P*(1-Q)
  //    ~ = NOT.      ~P = 1-P


  // P || P 	= 1-(1-P)*(1-P)
  //		= 1-(1-2P+P*P)
  //		= 1-(1-2P+P)
  //		= 1-(1-P)
  //		= P

  // P ||~P 	= 1-(1-P)*P
  //		= 1-(P-P*P)
  //		= 1-(P-P)
  //		= 1

  // P&&(P->Q) -> Q  	= 1 - [P*(1-P+P*Q)]*(1-Q)
  //			= 1 - (P*(1-P)*(1-Q) + P*P*Q*(1-Q))
  //			= 1 - (0*(1-Q) + P*P*0)
  //			= 1

string logicToMath(string Formula="(~(P->~P)&&~(Q->~Q))->~(P->~Q)", bool debugMode=false) {

  const int formulaLength = Formula.length();

  cout << endl << "Input Formula = " << Formula << endl;
  //search through formula to find locations and nest levels of parentheses.
  cout << endl << "Finding parentheses..." << endl;
  int leftLocations[formulaLength];
  int rightLocations[formulaLength];
  int leftLevels[formulaLength];
  int rightLevels[formulaLength];
  int nestLevel = 0;
  int numleft = 0;
  int numright = 0;
  for (int i=0;i<formulaLength;i++) {

    if (Formula.substr(i,1)=="(") {
      if (debugMode) cout << "Found ( at i = " << i << " with nestLevel = " << nestLevel << endl;
      leftLocations[numleft] = i;
      leftLevels[numleft] = nestLevel;
      nestLevel -= 1;
      numleft++;
    }
    else if (Formula.substr(i,1)==")") {
      nestLevel += 1;
      if (debugMode) cout << "Found ) at i = " << i << " with nestLevel = " << nestLevel << endl;
      rightLocations[numright] = i;
      rightLevels[numright] = nestLevel;
      numright++;
    }
    else continue;

  }
  if (debugMode) {
    cout << "nestLevel = " << nestLevel << endl;
    cout << "numleft = " << numleft << endl;
    cout << "numright = " << numright << endl;
  }

  //Check that there is a ")" for every "("
  if (numleft-numright > 0) {
    cout << "ERROR: Missing \")\" in formula!" << endl;
    return NULL;
  }
  else if (numleft-numright < 0) {
    cout << "ERROR: Missing \"(\" in formula!" << endl;
    return NULL;
  }

  //match left and right parentheses.
  cout << endl << "Matching sets of parantheses..." << endl;
  const int numSubs = numleft+1; //+1 for the original formula.
  int pairs[numSubs][2];
  int nummatched = 1;
  for (int i=0;i<formulaLength;i++) {
    if (i>0 && leftLocations[i]==0) break;
    int j=0;
    if (debugMode) cout << "( at " << leftLocations[i] << " with nestLevel=" << leftLevels[i] << endl;
    pairs[nummatched][0] = leftLocations[i];
    while (j<numright) {
      //cout << "  j = " << j << endl;
      if (rightLocations[j]>leftLocations[i] && rightLevels[j]==leftLevels[i]) {
        if (debugMode) cout << "	matched to ) at " << rightLocations[j] << " with nestLevel=" << rightLevels[j] << endl;
        pairs[nummatched][1] = rightLocations[j];
        nummatched++;
        break;
      }
      else {
        //cout << "	not matched to ) at " << rightLocations[j] << " with nestLevel=" << rightLevels[j] << endl;
        j++;
      }
    }
  }

  //extract the contents enclosed by the parentheses.
  cout << endl << "Extracting subFormulas..." << endl;
  string subFormulas[numSubs];
  string subFormulaNames[numSubs];
  subFormulas[0] = Formula;
  subFormulaNames[0] = "@0";
  for (int i=1;i<numSubs;i++) {
    subFormulaNames[i] = "@"+intToString(i);
    //cout << "pairs[" << i << "] = (" << pairs[i][0] << "," << pairs[i][1] << ")" << endl;
    subFormulas[i] = Formula.substr(pairs[i][0],pairs[i][1]-pairs[i][0]+1);
    if (debugMode) cout << subFormulaNames[i] << " = " << subFormulas[i] << endl;
  }

  //Replace statements in inner parentheses with statement names
  for (int i=0;i<numSubs;i++) {
    int replaceIndex = numSubs-1-i;
    string replaceFormula = subFormulas[replaceIndex];//Start with the last subFormula and replace it in every other formula where it appears.
    if (debugMode) cout << endl;
    for (int j=0;j<replaceIndex;j++) {
      if (debugMode) cout << subFormulas[j] << " ---> ";
      int pos = subFormulas[j].find(replaceFormula);
      if (pos>=0) {
        subFormulas[j] = subFormulas[j].substr(0,pos)+subFormulaNames[replaceIndex]+subFormulas[j].substr(pos+replaceFormula.length());
      }
      //else cout << "X";
      if (debugMode) cout << subFormulas[j];
      if (debugMode) cout << ",  pos = " << pos << endl;
    }
  }

  //Print simplified formulas
  cout << endl << "Behold the simplified formulas: " << endl;
  for (int i=0;i<numSubs;i++) {
    cout << subFormulaNames[i] << " = " << subFormulas[i] << endl;
  }

  //Generate Math statements
  cout << endl << "Translating to algebraic formulas..." << endl;
  string subMathFormulas[numSubs];
  string subMathFormulaNames[numSubs];
  for (int i=0;i<numSubs;i++) {
    subMathFormulaNames[i] = "@"+intToString(i);
    subMathFormulas[i] = subFormulas[i];
  }

  //Change the '&&' to '*'
  for (int i=0;i<numSubs;i++) {
    int pos=1;
    while (pos>0) {
      //Search in the subMathFormula for a '~'
      pos = subMathFormulas[i].find("&&");//returns -1 if it doesn't find anything.
      if (pos>0) {
        if (debugMode) cout << "Found \'&&\' in @" << i << " at position " << pos << endl;
        if (debugMode) cout << subMathFormulas[i] << "  --->  ";
      }
      else {
        if (debugMode) cout << "Found no \'&&\' in @" << i << endl;
        continue;
      }
      //Replace the substring '&&' in the formula with '*'
      subMathFormulas[i] = subMathFormulas[i].substr(0,pos)+"*"+subMathFormulas[i].substr(pos+2);
      if (debugMode) cout << subMathFormulas[i] << endl;
    }
  }

  //Change the 'P->Q' to '(1-P*(1-Q))'
  for (int i=0;i<numSubs;i++) {
    int pos=1;
    while (pos>0) {
      //Search in the subMathFormula for a '->'
      pos = subMathFormulas[i].find("->");//returns -1 if it doesn't find anything.
      if (pos>0) {
        if (debugMode) cout << "Found \'->\' in @" << i << " at position " << pos << endl;
        if (debugMode) cout << subMathFormulas[i] << "  --->  ";
      }
      else {
        if (debugMode) cout << "Found no \'->\' in @" << i << endl;
        continue;
      }
      //Get the statement 'P' that precedes it
      string strP = subMathFormulas[i].substr(pos-1,1);
      string strAt = subMathFormulas[i].substr(pos-2,1);
      if (strAt=="@" || strAt=="~") {
        strP = subMathFormulas[i].substr(pos-2,2);
      }
      if (subMathFormulas[i].substr(pos-2,1)=="~") {
        strP = subMathFormulas[i].substr(pos-3,3);
      }
      //Get the statement 'Q' that follows
      string strQ = subMathFormulas[i].substr(pos+2,1);
      if (strQ=="~") {
        strQ = subMathFormulas[i].substr(pos+2,2);
      }
      else if (strQ=="@") {
        strQ = subMathFormulas[i].substr(pos+2,2);
      }
      if (strQ=="~@") {
        strQ = subMathFormulas[i].substr(pos+2,3);
      }
      //Replace the substring 'P->Q' in the formula with '(1-P*(1-Q))'
      string mathString = "(1-"+strP+"*(1-"+strQ+"))";
      int posP = subMathFormulas[i].find(strP);
      subMathFormulas[i] = subMathFormulas[i].substr(0,posP)+mathString+subMathFormulas[i].substr(pos+strQ.length()+2);
      if (debugMode) cout << subMathFormulas[i] << endl;
    }
  }

  //Change the '~P' to '(1-P)'
  for (int i=0;i<numSubs;i++) {
    int pos=1;
    while (pos>0) {
      //Search in the subMathFormula for a '~'
      pos = subMathFormulas[i].find("~");//returns -1 if it doesn't find anything.
      if (pos>0) {
        if (debugMode) cout << "Found \'~\' in @" << i << " at position " << pos << endl;
        if (debugMode) cout << subMathFormulas[i] << "  --->  ";
      }
      else {
        if (debugMode) cout << "Found no \'~\' in @" << i << endl;
        continue;
      }
      //Get the statement 'P' that follows
      string strP = subMathFormulas[i].substr(pos+1,1);
      if (strP=="@") {
        strP = subMathFormulas[i].substr(pos+1,2);
      }
      //cout << "strP = " << strP << endl;
      //Replace the substring '~P' in the formula with '(1-P)'
      string mathString = "(1-"+strP+")";
      subMathFormulas[i] = subMathFormulas[i].substr(0,pos)+mathString+subMathFormulas[i].substr(pos+strP.length()+1);
      if (debugMode) cout << subMathFormulas[i] << endl;
    }
  }

  //Print out mathematical statements
  for (int i=0;i<numSubs;i++) {
    cout << subMathFormulaNames[i] << " = " << subMathFormulas[i] << endl;
  }

  //Flatten the main formula
  cout << endl << "Flattening the main mathematical expression..." << endl;
  string mathFormula = subMathFormulas[0];
  while (mathFormula.find("@")>=0) {
    if (debugMode) cout << "mathFormula = " << mathFormula << endl;
    int posat = mathFormula.find("@");
    if (posat<0) break;
    int numSub = std::stoi(mathFormula.substr(posat+1));
    if (debugMode) cout << "numSub = " << numSub << endl;
    mathFormula = mathFormula.substr(0,posat)+subMathFormulas[numSub]+mathFormula.substr(posat+2);
  }

  return mathFormula;
}
