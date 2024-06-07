#include "../../include/cfg.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <new>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

Production::Production() {}

void Production::setLeftSymbol(string leftSymbol) {
  this->leftSymbol = leftSymbol;
}

void Production::addRightSymbol(string rightSymbol) {
  this->rightSymbols.push_back(rightSymbol);
}

void Production::printProduction() {
  cout << leftSymbol + " -> ";
  for (size_t i = 0; i < rightSymbols.size(); i++) {
    cout << rightSymbols[i];
    if (i != rightSymbols.size() - 1) {
      cout << " | ";
    }
  } // for
  cout << endl;
}

void Production::setRightSymbol(vector<string> rightSymbols) {
  this->rightSymbols.clear();
  this->rightSymbols = rightSymbols;
}

Rule::Rule() {}

void Rule::addProduction(string productionStr) {

  string query = "";
  bool isFoundLeft = false;
  int index = -1;
  Production production;

  for (char c : productionStr) {
    if (c == ' ' && (query.length() == 0 || query[query.length() - 1] == ' '))
      continue;
    if (!isFoundLeft) {
      if (c == ' ')
        continue;
      if (c != '-')
        query += c;
      else {
        if (query.length() == 0) {
          cout << "Invalid production rule at '" + productionStr + "'" << endl;
          exit(1);
        }
        isFoundLeft = true;
        int counter = 0;
        for (Production proc : productions) {
          if (proc.leftSymbol == query) {
            production = proc;
            index = counter;
            break;
          }
          counter++;
        } // checking
        production.setLeftSymbol(query);
        query = "";
      }
    } else if (c == '>' && query.length() == 0)
      continue;
    else if (c != '|')
      query += c;
    else {
      if (query.size() && query[query.size() - 1] == ' ')
        query.erase(query.size() - 1, 1);
      if (query.length() == 0) {
        cout << "Invalid production rule at '" + productionStr + "'" << endl;
        exit(1);
      }
      if (query == "e'")
        query = "\u03B5";
      production.addRightSymbol(query);
      query = "";
    }
  } // for
    //
  if (query.size() > 0) {
    if (query.size() && query[query.size() - 1] == ' ')
      query.erase(query.size() - 1, 1);
    if (query == "e'")
      query = "\u03B5";
    production.addRightSymbol(query);
  } else {
    cout << "Invalid production rule at '" + productionStr + "'" << endl;
    exit(1);
  }
  if (index == -1)
    productions.push_back(production);
  else
    productions[index] = production;
  // print();
}

void Rule::addProduction(Production production) {

  productions.push_back(production);
}

void Rule::sort(){
  for(auto &production : productions){
    vector<string> &rsyms = production.rightSymbols;
    std::sort(rsyms.begin(),rsyms.end());
  }
}

void Rule::print() {
  for (Production production : this->productions) {
    production.printProduction();
  }
}

Grammer::Grammer() {}

void Grammer::print() { rule.print(); }

void Grammer::readFile(string fileName) {
  ifstream file(fileName, ios::in);

  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      if (line.length() == 0 ||
          all_of(line.begin(), line.end(), [](char c) { return c == ' '; }))
        continue;
      rule.addProduction(line);
    }
  } // oen
  else {
    cerr << "Error: Unable to open file '" + fileName + "'" << endl;
  }
}
void Grammer::execute() {
  // execute
}

void Grammer::execute(string fileName) {
  readFile(fileName);
  int size = rule.productions.size();
  rule.sort();

  for (int i = 0; i < size; i++) {
    solveLeftFactoring(rule.productions[i],i);
    for (int j = 0; j < i; j++) {
      // solveNonImmediate(rule.productions[i], rule.productions[j]);
    }
    solveImmediate(rule.productions.at(i));
  } //
  // execute
}
void Grammer::solveImmediate(Production &production) {
  string leftSymbol = production.leftSymbol;
  vector<string> alphas, betas, newRules, oldRules;
  string newName = leftSymbol + "'";
  vector<string> tempAltSplit;

  for (string alternative : production.rightSymbols) {
    tempAltSplit = splitSpacesToVector(alternative);
    if(tempAltSplit[0] == leftSymbol ) {
      for(string alt: tempAltSplit)
        alphas.push_back(alt);
    }
    else{
      betas.push_back(tempAltSplit[0]);
    }

    // if (alternative.substr(0, leftSymbol.length()) == leftSymbol) {
    //   alphas.push_back(alternative.substr(leftSymbol.length()));
    // } // if name is matched
    // else {
    //   betas.push_back(alternative);
    // } //
  }   // each alternatives

  if (alphas.size() == 0)
    return;

  // production.clear(); // clear rightsymbols
  for (string beta : betas) {
    if (beta != "\u03B5") {
      oldRules.push_back(beta + " " + newName);
    } else {
      oldRules.push_back(newName);
    }
  }

  production.setRightSymbol(oldRules);

  Production newProduction;
  newProduction.setLeftSymbol(newName);
  for (string alpha : alphas) {
    newProduction.addRightSymbol(alpha + " " + newName);
  }
  newProduction.addRightSymbol("\u03B5");
  rule.addProduction(newProduction);

} //

void Grammer::solveNonImmediate(Production &production,
                                Production &previousProduction) {
  string prevName = previousProduction.leftSymbol;
  vector<string> newRule;
  for (string alternate : production.rightSymbols) {
    if (alternate.substr(0, prevName.size()) == prevName) {
      for (string alternatePrev : previousProduction.rightSymbols) {
        if (alternatePrev == "\u03B5") {
          newRule.push_back(alternate.substr(prevName.size()));
        } else
          newRule.push_back(alternatePrev + alternate.substr(prevName.size()));
      }
    } // if
    else {
      newRule.push_back(alternate);
    }
  } // main loop
  production.setRightSymbol(newRule);
}


 vector<string> Grammer::splitSpacesToVector(string alternate){
    istringstream iss(alternate);
    vector<string> list;
    string eachToken;
    while (getline(iss,eachToken , ' ')) {
      list.push_back(eachToken);
    }
    
    return list;
}

string Grammer::convertToString(vector<string> alternate,int length){
    string joined;
    for(int i=0;i<length;i++){
      joined.append(alternate[i]);
      if(i!=length-1) joined.append(" ");
    }
  
    return joined;
}

void Grammer::addProductionAfterLeft(int originalLenght,int numberToAppend,string matched,int min,int max,int index){

      Production &productionNow = rule.productions[index];
      string name = productionNow.leftSymbol;
      string name2 = name + to_string(numberToAppend);
      vector<string> rightSymbols = productionNow.rightSymbols;
      vector<string> newRuleA;
      // cout<<"** "<<matched+" "+name2<<endl;
  //
      int min1 =min -  originalLenght + productionNow.rightSymbols.size();
      int max1 = min1 + max-min;
      string sym;

      int count = 0;
      for(int i=min1;i<=max1;i++){
          if(i==min1){
            productionNow.rightSymbols[i] = matched+ " "+ name2;
          }
          else
            productionNow.rightSymbols.erase(productionNow.rightSymbols.begin()+i-count++);
          sym = rightSymbols[i].substr(matched.length());
          if(sym.empty()) sym = "\u03B5";
          else if(sym[0]==' ') sym = sym.substr(1);
          newRuleA.push_back( sym );
      } 
      // productionNow.addRightSymbol(matched+" "+name2);
  //

      Production newProc,oldProc;
      newProc.setLeftSymbol(name2);
      newProc.setRightSymbol(newRuleA);

      rule.productions.push_back(newProc);
      
    
}


void Grammer::solveLeftFactoring(Production production,int index) {
  // addProductionAfterLeft(production,"Expression", 0,1);
  vector<string> rightSymbols = production.rightSymbols;
  // sort(rightSymbols.begin(), rightSymbols.end());
  // for (string sym : rightSymbols) {
  //   cout << sym + ", ";
  // } //
  //
  // cout<<&production<<"Hey"<<endl;
  int sizeOfAlt = rightSymbols.size();
  int compareSize = rightSymbols.size();
  int sizeOfEach;   // size of the current string
  string currentChar; // current char
  string currentCharCmp;  // current char
  int matchCurrent = 0, matchPrev = 0;
  int originalLength = production.rightSymbols.size();
  int numberToAppend = 1;
  vector<string> currentEach,currentToCompare;

  for (int i = 0; i < sizeOfAlt; i++) {
    currentEach = splitSpacesToVector(rightSymbols[i]);
    sizeOfEach = currentEach.size();
    matchPrev=0;
    compareSize = rightSymbols.size();
    for (int j = 0; j < sizeOfEach; j++) {
      currentChar = currentEach[j];
      // rule.productions.push_back(production);
      // production.addRightSymbol("Hello");
      matchCurrent = 0;
      for (int k = i + 1; k < compareSize; k++) {
        currentCharCmp = splitSpacesToVector(rightSymbols[k])[j];
        // cout<<"Comparing "<<currentChar<<","<<currentCharCmp<<endl;
        if (currentChar == currentCharCmp){
        // cout<<"Comparing equal"<<currentChar<<","<<currentCharCmp<<endl;
          matchCurrent++;
          // if(k==compareSize-1) matchPrev = matchCurrent;
          if(  k==compareSize-1 && (currentEach.size()==1 ||  j==sizeOfEach-1 )  ){
            string matched = convertToString(currentEach, j+1);
            // cout<<"1Found prefix at "<<i<<" to "<<i+matchCurrent<<" with '"<<matched<<"'"<<endl;
            addProductionAfterLeft(originalLength,numberToAppend++,matched, i , i+matchCurrent,index);
            i =i+ matchPrev;//it auto increment by 1 as loop
            break;
          }
          else if(currentEach.size()==1 || k==compareSize-1) matchPrev = matchCurrent;
        }//if they are equal
        else{
          if( matchCurrent!=0  && currentEach.size()==1){
            string matched = convertToString(currentEach, j+1);
            // cout<<"2Found prefix at "<<i<<" to "<<i+matchCurrent<<" with '"<<matched<<"'"<<endl;
            addProductionAfterLeft(originalLength,numberToAppend++,matched, i , i+matchCurrent,index);
            i =i+ matchPrev;//it auto increment by 1 as loop
            break;
          }//if less than previsous match
          if( matchCurrent < matchPrev){
            string matched = convertToString(currentEach, j);
            // cout<<i<<", "<<i+matchPrev<<endl;
            // cout<<"3Found prefix at "<<i<<" to "<<i+matchPrev<<" with '"<<matched<<"'"<<endl;
            addProductionAfterLeft(originalLength,numberToAppend++,matched, i , i+matchPrev,index);
            i =i+ matchPrev;//it auto increment by 1 as loop
            break;
          }//if less than previsous match
          else{
          compareSize = i+ matchCurrent+1;
          }
          matchPrev = matchCurrent;
        }//else 
      } // inner comparison
    }   // each character
  }     // outer each alternate
}



