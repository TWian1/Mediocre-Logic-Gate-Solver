#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

string DECTOBIN(int num, int length){
  vector<int> powVec;
  int curint = num;
  string outstring;
  for (int a = 0; a < length; a++){powVec.push_back(pow(2.0, float(length-a-1)));}
  for (int c = 0; c < powVec.size(); c++){
    if (curint >= powVec[c]){outstring += "1";curint -= powVec[c];}
    else{outstring += "0";}
  }
  return outstring;
}
void join(const vector<string>& v, char c, string& s) {

   s.clear();

   for (vector<string>::const_iterator p = v.begin();
        p != v.end(); ++p) {
      s += *p;
      if (p != v.end() - 1)
        s += c;
   }
}

vector<int> genInputs(string text){
  string temp;
  char c;
  vector<int> inputs;
  for (int a = 0; a < text.length(); a++){
    c = text[a];
    if (c == ' '){
      if (temp.length() >= 1){inputs.push_back(stoi(temp));}
      temp = "";
    }
    else if (c >= '0' && c <= '9'){temp += c;}
  }
  if (temp.length() >= 1){inputs.push_back(stoi(temp));}
  return inputs;
}

int LogicGate(string gate, vector<int> cs, vector<int> inp){
  int state;
  if (gate == "NOT "){state = 1-cs[inp[0]];}
  else if (gate == "AND "){state = cs[inp[0]]*cs[inp[1]];}
  else if (gate == "OR  "){state = (cs[inp[0]]+cs[inp[1]]) - (cs[inp[0]]*cs[inp[1]]);}
  else if (gate == "XOR "){state = (cs[inp[0]]+cs[inp[1]]) - (2*cs[inp[0]]*cs[inp[1]]);}
  else if (gate == "NOR "){state = 1-((cs[inp[0]]+cs[inp[1]]) - (cs[inp[0]]*cs[inp[1]]));}
  else if (gate == "NAND"){state = 1-(cs[inp[0]]*cs[inp[1]]);}
  else if (gate == "XNOR"){state = 1-((cs[inp[0]]+cs[inp[1]]) - (2*cs[inp[0]]*cs[inp[1]]));}
  return state;
}



vector<string> evaluateLogic(vector<string> values,vector<string> inputval,vector<int> outputval,bool show){
  string BINVAL;
  string CURVAL;
  vector<string> outstates;
  vector<int> curstates;
  vector<int> inputs;
  string gate;
  int state;
  bool inside;
  for (int binValIndex = 0; binValIndex < inputval.size(); binValIndex++){
    BINVAL = inputval[binValIndex];
    state = 0;
    curstates.clear();
    outstates.push_back("");
    for (int valueIndex = 0; valueIndex < values.size(); valueIndex++){
      CURVAL = values[valueIndex];
      if (CURVAL.length() == 1){
        state = int(BINVAL[CURVAL[0] - 'A']) - 48;
        curstates.push_back(state);
      }
      else{
        inputs = genInputs(CURVAL);
        gate = CURVAL.substr(0, 4);
        state = LogicGate(gate, curstates, inputs);
        curstates.push_back(state);
      }
      inside = false;
      for (int a = 0; a < outputval.size(); a++){
        if (valueIndex == outputval[a]){inside = true; break;}
      }
      if (inside) {outstates[binValIndex] += to_string(state);}
      if (show){cout << CURVAL << " " << to_string(state) << endl;}
    }
  }
  return outstates;
}

int main(){
  bool display = true;
  string bits;
  string upto;
  string temp;
  vector<string> expected;
  cout << "Number of bits:";
  cin >> bits;
  while (true){

    cout << "upto:";
    cin >> upto;
    if (stoi(upto)+1 <= pow(2, stoi(bits)) && stoi(upto) >= 0){
      break;
    }
  }
  for (int a = 0; a < stoi(upto)+1; a++){
    cout << to_string(a) << " (" << DECTOBIN(a, stoi(bits)) << ")";
    temp = "";
    cin >> temp;
    expected.push_back(temp);
  }
  srand(time(NULL));

  int gate;
  int maxlogicgates = 7;
  int minlogicgates = 2;
  int outvalcount = 1;
  int logicgatecount;
  vector<string> inputval;
  vector<string> values;
  vector<int> outputval;
  vector<string> inputlist;
  vector<string> evalout;

  int bestscore = 10000000000;
  string aaaa;
  bool works;
  bool error1;
  bool error2;
  double score;
  vector <int> bestoutputs;
  int bestlength = maxlogicgates + 4;
  vector<string> bestval;
  vector <string> gates = {"NOT ", "OR  ", "AND ", "NOR ", "NAND", "XOR ", "XNOR"};
  vector <double> scoreg = {1, 1, 1, 1.5, 1.5, 1.7, 2.2};
  vector <int> inputs = {1, 2, 2, 2, 2, 2, 2};
  string letlist = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int a = 0; a < stoi(upto)+1; a++){inputval.push_back(DECTOBIN(a, stoi(bits)));}
  int stime = time(NULL);
  //for (int a = 0; a < 100000; a++){
  while((time(NULL)-60) < stime){
    score = 0;
    maxlogicgates = bestlength - 4;
    values.clear();
    outputval.clear();
    for (int b = 0; b < stoi(bits); b++){values.push_back(string(1, letlist[b]));}
    logicgatecount = (rand()%(maxlogicgates-minlogicgates + 1)+ minlogicgates);
    for (int c = 0; c < logicgatecount; c++){
      inputlist.clear();
      aaaa = "";
      gate = rand()%(gates.size());
      score += scoreg[gate];
      for (int d = 0; d < inputs[gate]; d++){inputlist.push_back(to_string(rand()%(values.size())));}
      join(inputlist, ' ', aaaa);
      values.push_back(gates[gate] + "(" + aaaa + ")");
    }
    if (score >= bestscore && values.size() == bestlength){continue;}
    for (int d = 0; d < outvalcount; d++){outputval.push_back(rand()%values.size());}
    evalout.clear();
    evalout = evaluateLogic(values, inputval, outputval, false);
    works = true;
    error1 = true;
    error2 = true;
    for(int d =0; d < evalout.size(); d++){
      //cout << evalout[d] << endl;
      if (evalout[d] != expected[d] && works){works = false;break;}
      else if (evalout[d] != expected[d] && works == false && error1){error1 = false;}
      else if (evalout[d] != expected[d] && error1 == false){error2 = false; break;}
    }
    if ((works && values.size() < bestlength) || (works && score < bestscore && values.size() == bestlength)){
      cout << endl << endl << endl;
      bestlength = values.size();
      bestval = values;
      bestscore = score;
      bestoutputs = outputval;
      if (display){
        for(int k =0; k < values.size(); k++){cout<<values[k] << " " << endl;}
          cout << "Outputs: ";
          for(int k =0; k < outputval.size(); k++){cout<<outputval[k] << " ";}
          cout << endl;
          cout << "length: " << values.size() << endl;
          cout << "score: " << to_string(score) << endl;
      }
    }
  }
  if (display == false){
  for(int k =0; k < bestval.size(); k++){cout<<bestval[k] << " " << endl;}
  cout << "Outputs: ";
  for(int k =0; k < bestoutputs.size(); k++){cout<<bestoutputs[k] << " ";}
  cout << endl;
  cout << "length: " << bestlength << endl;
  cout << "score: " << to_string(bestscore) << endl;
  }
    return 0;
}
