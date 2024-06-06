#include "../../include/cfg.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <new>
#include <ostream>
#include <string>
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

void Production::setRightSymbol(vector<string> rightSymbols){
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

void Rule::addProduction(Production production){
  productions.push_back(production);
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
void Grammer::execute(){
  //execute
}

void Grammer::execute(string fileName){
  readFile(fileName);
  
  for(int i=0;i<rule.productions.size();i++){
    solveImmediate(rule.productions.at(i));
  }//
  //execute
}
void Grammer::solveImmediate(Production &production) {
  string leftSymbol = production.leftSymbol;
  vector<string> alphas, betas, newRules,oldRules;
  string newName = leftSymbol + "'";

  for (string alternative : production.rightSymbols) {
    if (alternative.substr(0, leftSymbol.length()) == leftSymbol) {
      alphas.push_back(alternative.substr(leftSymbol.length()));
    } // if name is matched
    else {
      betas.push_back(alternative);
    } //
  }   // each alternatives

  if (alphas.size() == 0)
    return;

  // production.clear(); // clear rightsymbols
  for (string beta : betas) {
    if(beta !="\u03B5"){
    oldRules.push_back(beta + " " + newName);
    }else{
      oldRules.push_back(newName);
    }
  }

  production.setRightSymbol(oldRules);

  Production newProduction;
  newProduction.setLeftSymbol(newName);
  for (string alpha : alphas) {
    newProduction.addRightSymbol(alpha+" "+newName);
  }
  newProduction.addRightSymbol("\u03B5"); 
  rule.addProduction(newProduction);

} //
