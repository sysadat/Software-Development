#include "StringUtils.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>


namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end){
    int length = str.length();
    std::string newString;

    if (start < 0){
      start += length;
    }

    if (end < 0){
      end += length;
    }
    else if (end == 0){
      end = length;
    }

    for (int i = start; i < end; i++){
      newString += str[i];
    }

    return newString;
}

std::string Capitalize(const std::string &str){
    std::string newString;

    for(auto Ch : str){
      newString += tolower(Ch);
    }
    newString[0] = toupper(newString[0]);

    return newString;
}

std::string Title(const std::string &str){
    int length = str.length();
    std::string newString;

    for(auto Ch : str){
      newString += tolower(Ch);
    }

    newString[0] = toupper(newString[0]);

    for(int i = 0; i < length;i++){
      if (isspace(newString[i]) != 0){
        int next = (++i);
        newString[next] = toupper(newString[next]);
      }
      else if (isalpha(newString[i]) == 0){
        int next = (++i);
        newString[next] = toupper(newString[next]);
      }
    }
    return newString;
}

std::string LStrip(const std::string &str){
    // Your code goes here
    std::string newString = str;
    int length = newString.length();

    for(int i = 0; i < length; i++){
      if (isspace(newString[i]) == 0){
        int start = i;
        newString = Slice(newString,start,length);
        break;
      }
    }
    return newString;
}

std::string RStrip(const std::string &str){
    // Your code goes here
    std::string newString = str;
    int length = newString.length();
    int start = 0;
    int end = 0;

    for(int i = (length - 1); i > 0; i--){
      if (isspace(newString[i]) == 0){
        end = (i + 1);
        newString = Slice(newString,start,end);
        break;
      }
    }
    return newString;
}

std::string Strip(const std::string &str){
    // Your code goes here
    std::string newString = str;

    newString = LStrip(newString);
    newString = RStrip(newString);

    return newString;
}

std::string Center(const std::string &str, int width, char fill){
    // Your code goes here
    std::string newString = str;
    std::string input(1,fill);

    int length = newString.length();
    int difference = (width - length);
    int ifEven = 0;
    int remainder = 0;

    if (difference % 2 == 0){
      ifEven = (difference / 2);
      newString = LJust(newString,length + ifEven,fill);
      length = newString.length();
      newString = RJust(newString,length + ifEven,fill);
    }
    else if (difference % 2 != 0){
      remainder = (difference  % 2);
      ifEven = (difference / 2);
      newString = LJust(newString,length + (ifEven + remainder), fill);
      length = newString.length();
      newString = RJust(newString,length + ifEven, fill);
    }

    return newString;
}

std::string LJust(const std::string &str, int width, char fill){
    // Your code goes here
    std::string newString = str;
    std::string input(1,fill);

    int length = newString.length();

    for(int i = length; i < width; i++){
        newString.append(input);
    }
    return newString;
}

std::string RJust(const std::string &str, int width, char fill){
    // Your code goes here
    std::string newString = str;
    std::string input(1,fill);
    std::string secondInput(1,fill);

    int length = newString.length();

    for(int i = length; i < (width - 1); i++){
        input.append(secondInput);
    }
    newString = input.append(newString);

    return newString;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
    // Your code goes here
    std::string newString = str;
    std::string oldString = old;
    std::string newItem = rep;
    std::string emptyString;
    std::string finalString;

    int length = newString.length();
    int oldCheck = oldString.length();

    for (int i = 0; i < length; i++) {
      emptyString = Slice(newString,i, i + oldCheck);
      if (emptyString == old){
        finalString.append(newItem);
        i += (oldCheck - 1);
      }
      else{
        finalString += newString[i];
      }
    }
    return finalString;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt){
    // Your code goes here
    std::string newString = str;
    std::string replaceString = splt;
    std::string emptyString;
    std::string checkString;
    std::vector < std::string > returnVector;

    int length = newString.length();

    if(replaceString == ""){
      replaceString = " ";
      for (int j = 0; j < length; j++) {
        if(!isspace(newString[j])){
          checkString += newString[j];
        }
        else if(isspace(newString[j])){
          if(isspace(newString[j]) && isspace(newString[j+1])){
            continue;
          }
          returnVector.push_back(checkString);
          checkString.clear();
        }
      }
      returnVector.push_back(checkString);
      return returnVector;
    }

    int secondLength = replaceString.length();

    for (int i = 0; i < length; i++) {
      emptyString = Slice(newString,i, i + secondLength);
      if(emptyString == replaceString){
        returnVector.push_back(checkString);
        checkString.clear();
      }
      else{
        checkString += newString[i];
      }
    }
    returnVector.push_back(checkString);

    return returnVector;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect){
    // Your code goes here
    std::string newString;
    std::string replaceItem = str;
    std::vector <std::string> newVector = vect;

    int size = newVector.size();

    if(replaceItem.length() == 0){
      replaceItem = " ";
    }

    for (int i = 0; i < size; i++){
      newString.append(replaceItem);
      newString.append(newVector[i]);
    }

    newString = Slice(newString,1,newString.length());

    return newString;
}

std::string spaceFunc(int size){
  std::string newString = " ";
  std::string finalString;

  for (int i = 0; i < size; i++) {
    finalString.append(newString);
  }
  return finalString;
}

std::string ExpandTabs(const std::string &str, int tabsize){
    // Your code goes here
    std::string newString = str;
    std::string finalString;
    std::string funcString;
    int position = 0;
    int check = 0;

    for (auto Ch : newString){
      std::string charString = std::string(1,Ch);
      if(Ch == '\t'){
        check = 0;
        if(tabsize > 0){
          check = (tabsize - (position % tabsize));
        }
        funcString = spaceFunc(check);
        charString = funcString;
        position = 0;
      }
      else{
        position++;
      }
      finalString += charString;
    }
    return finalString;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase){
    // Your code goes here
  std::string leftString = left;
  std::string rightString = right;
  int finalVal = 0;
  int leftSize = 0;
  int rightSize = 0;

  if(ignorecase){
    finalVal = 0;
    return finalVal;
  }

  leftSize = leftString.size();
  rightSize = rightString.size();

  if(leftSize == 0){
    return rightSize;
  }
  if(rightSize == 0){
    return leftSize;
  }

  int *costs = new int[rightSize + 1];

  for(int k = 0; k <= rightSize; k++){
    costs[k] = k;
  }

  int i = 0;
  for (std::string::const_iterator it1 = leftString.begin(); it1 != leftString.end(); ++it1, ++i){
    costs[0] = i + 1;
    int corner = i;
    int j = 0;

    for (std::string::const_iterator it2 = rightString.begin(); it2 != rightString.end(); ++it2, ++j) {
      int upper = costs[j + 1];
      if(*it1 == *it2){
        costs[j+1] = corner;
     }
      else{
          int t(upper<corner?upper:corner);
          costs[j + 1] = (costs[j]<t?costs[j]:t) + 1;
      }
    corner = upper;
  }
}
  int result = costs[rightSize];
  delete [] costs;

  return result;
}
}
