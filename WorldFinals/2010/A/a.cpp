#include "a.h"

Element makeElementFlagOnly(int flag) {
  Element e;
  e.flag = flag;
  return e;
}

Element makeElementSingleNumber(int number) {
  Element e;
  e.flag = TYPE_ARRAY_1D;
  e.array1d.push_back(number);
  return e;
}

Element makeElementVariable(string name) {
  Element e;
  e.flag = TYPE_VARIABLE;
  e.name = name;
  return e;
}

Element makeElementArray1D (const vector<int> &array1d) {
  Element e;
  e.flag = TYPE_ARRAY_1D;
  e.array1d = array1d;
  return e;
}


Element convertStringToElement(string token) {
  switch (token[0]) {
  case '+':
    return makeElementFlagOnly(TYPE_PLUS);
  case '-':
    return makeElementFlagOnly(TYPE_MINUS);
  case '*':
    return makeElementFlagOnly(TYPE_MULTIPLY);
  case '/':
    return makeElementFlagOnly(TYPE_SLASH);
  case '=':
    return makeElementFlagOnly(TYPE_EQUAL);
  case '(':
    return makeElementFlagOnly(TYPE_OPEN);
  case ')':
    return makeElementFlagOnly(TYPE_CLOSE);
  default:
//    DEBUG;
//    cout << "[" << token << "]" << endl;
    if (isdigit(token[0])) {
      return makeElementSingleNumber(atoi(token.c_str()));
    } else if (token == "rho") {
      return makeElementFlagOnly(TYPE_RHO);
    } else if (token == "iota") {
      return makeElementFlagOnly(TYPE_IOTA);
    } else if (token == "drop") {
      return makeElementFlagOnly(TYPE_DROP);
    } else {
      return makeElementVariable(token);
    }
  }
}

vector<Element> parse(string line) {
  istringstream is(line);
  
  string token;
  vector<Element> output;
  while(is >> token){
    output.push_back(convertStringToElement(token));
  }
  return output;
}

void appendNumbers (vector<int> &numberBuffer, vector<Element> &elements) {
  if (numberBuffer.size() == 1) {
    elements.push_back(makeElementSingleNumber(numberBuffer[0]));
    numberBuffer.clear();
  } else if (numberBuffer.size() > 1) {
    elements.push_back(makeElementArray1D(numberBuffer));
    numberBuffer.clear();
  }
}

vector<Element> convertContinuousNumbersToArray1D(const vector<Element> &elements) {
  vector<Element> output;
  vector<int> numberBuffer;
  
  for (vector<Element>::const_iterator it = elements.begin(); it != elements.end(); it++) {
    if (it->flag == TYPE_ARRAY_1D) {
      numberBuffer.push_back(it->array1d[0]);
    } else {
      appendNumbers(numberBuffer, output);
      output.push_back(*it);
    }
  }
  appendNumbers(numberBuffer, output);
  return output;
}

bool isArray(const Element &e) {
  switch(e.flag) {
  case TYPE_ARRAY_1D:
  case TYPE_ARRAY_2D:
  case TYPE_ARRAY_3D:
    return true;
  default:
    return false;
  }
}

Element getVariableValue(const Environment &environment, string name) {
  Environment::const_iterator it = environment.find(name);
  if (it != environment.end()) {
    return it->second;
  }
  return makeElementFlagOnly(TYPE_NULL);
}

bool verifyCloseResult(pair<int, Element> &subResult, const vector<Element> &elements) {
  if (!isArray(subResult.second)) {
    REPORT("Result of inside of (...) should be array.");
    throw EXCEPTION_RE;
  }
  if (elements[subResult.first-1].flag != TYPE_OPEN) {
    REPORT("'(' is not found.");
    throw EXCEPTION_RE;
  }
  return true;
}

bool isSameShape1D(const Element &left, const Element &right) {
  return left.array1d.size() == right.array1d.size();
}

bool isSameShape2D(const Element &left, const Element &right) {
  if (left.array2d.size() != right.array2d.size()) {
    return false;
  }
  for (int i = 0; i < left.array2d.size(); i++) {
    if (left.array2d[i].size() != right.array2d[i].size()) {
      return false;
    }
  }
  return true;
}

bool isSameShape3D(const Element &left, const Element &right) {
  if (left.array3d.size() != right.array3d.size()) {
    return false;
  }
  for (int i = 0; i < left.array3d.size(); i++) {
    if (left.array3d[i].size() != right.array3d[i].size()) {
      return false;
    }
    for (int j = 0; j < left.array3d[i].size(); j++) {
      if (left.array3d[i][j].size() != right.array3d[i][j].size()) {
        return false;
      }
    }
  }
  return true;
}

bool isSameShape(const Element &left, const Element &right) {
  if (left.flag != right.flag) {
    return false;
  }
  switch (left.flag) {
  case TYPE_ARRAY_1D:
    return isSameShape1D(left, right);
  case TYPE_ARRAY_2D:
    return isSameShape2D(left, right);
  case TYPE_ARRAY_3D:
    return isSameShape3D(left, right);
  default:
    REPORT("Unexpected type.");
    throw EXCEPTION_RE;
  }
}

bool is1DAnd1Element(const Element &e) {
  return e.flag == TYPE_ARRAY_1D && e.array1d.size() == 1;
}

int calculateSingleValue(int operatorFlag, int left, int right) {
  switch (operatorFlag) {
  case TYPE_PLUS:
    return left+right;
  case TYPE_MINUS:
    return left-right;
  case TYPE_MULTIPLY:
    return left*right;
  default:
    REPORT("Unexpected operator.");
    throw EXCEPTION_RE;
  }
}

Element calcualteSameShape1D(int operatorFlag, const Element &left, const Element &right) {
  Element output;
  output.flag = TYPE_ARRAY_1D;
  for (int i = 0; i < left.array1d.size(); i++) {
    output.array1d.push_back(calculateSingleValue(operatorFlag, left.array1d[i], right.array1d[i]));
  }
  return output;
}

Element calcualteSameShape2D(int operatorFlag, const Element &left, const Element &right) {
  Element output;
  output.flag = TYPE_ARRAY_2D;
  for (int i = 0; i < left.array2d.size(); i++) {
    output.array2d.push_back(vector<int>());
    for (int j = 0; j < left.array2d[i].size(); j++) {
      output.array2d[i].push_back(calculateSingleValue(operatorFlag, left.array2d[i][j], right.array2d[i][j]));
    }
  }
  return output;
}

Element calcualteSameShape3D(int operatorFlag, const Element &left, const Element &right) {
  Element output;
  output.flag = TYPE_ARRAY_3D;
  for (int i = 0; i < left.array3d.size(); i++) {
    output.array3d.push_back(vector<vector<int> >());
    for (int j = 0; j < left.array3d[i].size(); j++) {
      output.array3d[i].push_back(vector<int>());
      for (int k = 0; k < left.array3d[i][j].size(); k++) {
        output.array3d[i][j].push_back(calculateSingleValue(operatorFlag, left.array3d[i][j][k], right.array3d[i][j][k]));
      }
    }
  }
  return output;
}

Element calculateSameShape(int operatorFlag, const Element &left, const Element &right) {
  switch (left.flag) {
  case TYPE_ARRAY_1D:
    return calcualteSameShape1D(operatorFlag, left, right);
  case TYPE_ARRAY_2D:
    return calcualteSameShape2D(operatorFlag, left, right);
  case TYPE_ARRAY_3D:
    return calcualteSameShape3D(operatorFlag, left, right);
  default:
    REPORT("Unexpected type.");
    throw EXCEPTION_RE;
  }
}

Element calculateWith1Element1D(int operatorFlag, const Element &left, int right) {
  Element output;
  output.flag = TYPE_ARRAY_1D;
  for (int i = 0; i < left.array1d.size(); i++) {
    output.array1d.push_back(calculateSingleValue(operatorFlag, left.array1d[i], right));
  }
  return output;
}

Element calculateWith1Element2D(int operatorFlag, const Element &left, int right) {
  Element output;
  output.flag = TYPE_ARRAY_2D;
  for (int i = 0; i < left.array2d.size(); i++) {
    output.array2d.push_back(vector<int>());
    for (int j = 0; j < left.array2d[i].size(); j++) {
      output.array2d[i].push_back(calculateSingleValue(operatorFlag, left.array2d[i][j], right));
    }
  }
  return output;
}

Element calculateWith1Element3D(int operatorFlag, const Element &left, int right) {
  Element output;
  output.flag = TYPE_ARRAY_3D;
  for (int i = 0; i < left.array3d.size(); i++) {
    output.array3d.push_back(vector<vector<int> >());
    for (int j = 0; j < left.array3d[i].size(); j++) {
      output.array3d[i].push_back(vector<int>());
      for (int k = 0; k < left.array3d[i][j].size(); k++) {
        output.array3d[i][j].push_back(calculateSingleValue(operatorFlag, left.array3d[i][j][k], right));
      }
    }
  }
  return output;
}

Element calculateWith1Element(int operatorFlag, const Element &left, int right) {
  switch (left.flag) {
  case TYPE_ARRAY_1D:
    return calculateWith1Element1D(operatorFlag, left, right);
  case TYPE_ARRAY_2D:
    return calculateWith1Element2D(operatorFlag, left, right);
  case TYPE_ARRAY_3D:
    return calculateWith1Element3D(operatorFlag, left, right);
  default:
    REPORT("Unexpected type.");
    throw EXCEPTION_RE;
  }
}

Element calculatePlusMinusOrMultiply(int operatorFlag, const Element &left, const Element &right) {
  if (isSameShape(left, right)) {
    return calculateSameShape(operatorFlag, left, right);
  } else if (is1DAnd1Element(right)) {
    return calculateWith1Element(operatorFlag, left, right.array1d[0]);
  } else if (is1DAnd1Element(left) && operatorFlag == TYPE_MINUS) {
    Element temp = calculateWith1Element(TYPE_MINUS, right, left.array1d[0]);
    return calculateWith1Element(TYPE_MULTIPLY, temp, -1);
  } else if (is1DAnd1Element(left)) {
    return calculateWith1Element(operatorFlag, right, left.array1d[0]);
  } else {
    REPORT("Left and right should be same shape or at least one of them is 1 element vector.");
    throw EXCEPTION_RE;
  }
}

Element calculateDrop1D(int count, const Element &right) {
  if (count >= right.array1d.size()) {
    REPORT("Drop size should be less than right operand array size.");
    throw EXCEPTION_RE;
  }
  
  Element output;
  output.flag = TYPE_ARRAY_1D;
  for (int i = count; i < right.array1d.size(); i++) {
    output.array1d.push_back(right.array1d[i]);
  }
  return output;
}

Element calculateDrop2D(int count, const Element &right) {
  if (count >= right.array2d.size()) {
    REPORT("Drop size should be less than right operand array size.");
    throw EXCEPTION_RE;
  }
  
  Element output;
  output.flag = TYPE_ARRAY_2D;
  for (int i = count; i < right.array2d.size(); i++) {
    output.array2d.push_back(right.array2d[i]);
  }
  return output;
}

Element calculateDrop3D(int count, const Element &right) {
  if (count >= right.array3d.size()) {
    REPORT("Drop size should be less than right operand array size.");
    throw EXCEPTION_RE;
  }
  
  Element output;
  output.flag = TYPE_ARRAY_3D;
  for (int i = count; i < right.array3d.size(); i++) {
    output.array3d.push_back(right.array3d[i]);
  }
  return output;
}

Element calculateDrop(const Element &left, const Element &right) {
  if (left.flag != TYPE_ARRAY_1D) {
    REPORT("left operand should be 1D array.");
    throw EXCEPTION_RE;
  }
  if (left.array1d.size() != 1) {
    REPORT("left operand should contains exactly 1 element.");
    throw EXCEPTION_RE;
  }
  
  switch (right.flag) {
  case TYPE_ARRAY_1D:
    return calculateDrop1D(left.array1d[0], right);
  case TYPE_ARRAY_2D:
    return calculateDrop2D(left.array1d[0], right);
  case TYPE_ARRAY_3D:
    return calculateDrop3D(left.array1d[0], right);
  default:
    REPORT("right operand should be 1D, 2D or 3D array.");
    throw EXCEPTION_RE;
  }
}

Element calculateRho1D(int size0, const Element &right) {
  if (size0 <= 0) {
    REPORT("size should be positive.");
    throw EXCEPTION_RE;
  }
  
  int index = 0;
  Element output;
  output.flag = TYPE_ARRAY_1D;
  for (int i = 0; i < size0; i++) {
    output.array1d.push_back(right.array1d[index % right.array1d.size()]);
    index++;
  }
  return output;
}

Element calculateRho2D(int size0, int size1, const Element &right) {
  if (size0 <= 0) {
    REPORT("size should be positive.");
    throw EXCEPTION_RE;
  }
  if (size1 <= 0) {
    REPORT("size should be positive.");
    throw EXCEPTION_RE;
  }
  
  int index = 0;
  Element output;
  output.flag = TYPE_ARRAY_2D;
  for (int i = 0; i < size0; i++) {
    output.array2d.push_back(vector<int>());
    for (int j = 0; j < size1; j++) {
      output.array2d[i].push_back(right.array1d[index % right.array1d.size()]);
      index++;
    }
  }
  return output;
}

Element calculateRho3D(int size0, int size1, int size2, const Element &right) {
  if (size0 <= 0) {
    REPORT("size should be positive.");
    throw EXCEPTION_RE;
  }
  if (size1 <= 0) {
    REPORT("size should be positive.");
    throw EXCEPTION_RE;
  }
  if (size2 <= 0) {
    REPORT("size should be positive.");
    throw EXCEPTION_RE;
  }
  
  int index = 0;
  Element output;
  output.flag = TYPE_ARRAY_3D;
  for (int i = 0; i < size0; i++) {
    output.array3d.push_back(vector<vector<int> >());
    for (int j = 0; j < size1; j++) {
      output.array3d[i].push_back(vector<int>());
      for (int k = 0; k < size2; k++) {
        output.array3d[i][j].push_back(right.array1d[index % right.array1d.size()]);
        index++;
      }
    }
  }
  return output;
}

Element calculateRho(const Element &left, const Element &right) {
  if (left.flag != TYPE_ARRAY_1D || right.flag != TYPE_ARRAY_1D) {
    REPORT("both left and right operand should be 1D array.");
    throw EXCEPTION_RE;
  }
  switch (left.array1d.size()) {
  case 1:
    return calculateRho1D(left.array1d[0], right);
  case 2:
    return calculateRho2D(left.array1d[0], left.array1d[1], right);
  case 3:
    return calculateRho3D(left.array1d[0], left.array1d[1], left.array1d[2], right);
  default:
    REPORT("left operand should be contains 1, 2 or 3 integers.");
    throw EXCEPTION_RE;
  }
}

Element calculateOperator(int operatorFlag, const Element &left, const Element &right) {
  switch (operatorFlag) {
  case TYPE_PLUS:
  case TYPE_MINUS:
  case TYPE_MULTIPLY:
    return calculatePlusMinusOrMultiply(operatorFlag, left, right);
  case TYPE_DROP:
    return calculateDrop(left, right);
  case TYPE_RHO:
    return calculateRho(left, right);
  default:
    REPORT("Unexpected operator.");
    throw EXCEPTION_RE;
  }
}

int calculateSlashOneVector(int operatorFlag, const vector<int> &array) {
  int result = array[array.size() - 1];
  for (int i = array.size() - 2; i >= 0; i--) {
//    DEBUG;
//    cout << result << endl;
    switch (operatorFlag) {
    case TYPE_PLUS:
      result = array[i] + result;
      break;
    case TYPE_MINUS:
      result = array[i] - result;
      break;
    case TYPE_MULTIPLY:
      result = array[i] * result;
      break;
    default:
      REPORT("Unexpected operator.");
      throw EXCEPTION_RE;
    }
  }
  return result;
}

Element calculateSlash1D(int operatorFlag, const Element &right) {
  Element output;
  output.flag = TYPE_ARRAY_1D;
  output.array1d.push_back(calculateSlashOneVector(operatorFlag, right.array1d));
  return output;
}

Element calculateSlash2D(int operatorFlag, const Element &right) {
  Element output;
  output.flag = TYPE_ARRAY_1D;
  for (int i = 0; i < right.array2d.size(); i++) {
    output.array1d.push_back(calculateSlashOneVector(operatorFlag, right.array2d[i]));
  }
  return output;
}

Element calculateSlash3D(int operatorFlag, const Element &right) {
  Element output;
  output.flag = TYPE_ARRAY_2D;
  for (int i = 0; i < right.array3d.size(); i++) {
    output.array2d.push_back(vector<int>());
    for (int j = 0; j < right.array3d[i].size(); j++) {
      output.array2d[i].push_back(calculateSlashOneVector(operatorFlag, right.array3d[i][j]));
    }
  }
  return output;
}

Element calculateSlash(int operatorFlag, const Element &right) {
  switch (right.flag) {
  case TYPE_ARRAY_1D:
    return calculateSlash1D(operatorFlag, right);
  case TYPE_ARRAY_2D:
    return calculateSlash2D(operatorFlag, right);
  case TYPE_ARRAY_3D:
    return calculateSlash3D(operatorFlag, right);
  default:
    REPORT("Unexpected right.");
    throw EXCEPTION_RE;
  }
}

Element calculateItoa(const Element &right) {
  if (right.flag != TYPE_ARRAY_1D) {
    REPORT("Unexpected right.");
    throw EXCEPTION_RE;
  }
  if (right.array1d.size() != 1) {
    REPORT("Unexpected right.");
    throw EXCEPTION_RE;
  }
  if (right.array1d[0] <= 0) {
    REPORT("Unexpected right.");
    throw EXCEPTION_RE;
  }
  
  Element output;
  output.flag = TYPE_ARRAY_1D;
  for (int i = 1; i <= right.array1d[0]; i++) {
    output.array1d.push_back(i);
  }
  return output;
}

pair<int, Element> exec(int index, const vector<Element> &elements, Environment &environment) {
  Element currentResult = makeElementFlagOnly(TYPE_NULL);
  int i = index;
  while (i >= 0) {
    /*
    DEBUG;
    print(currentResult);
    DEBUG;
    cout << i << endl;
    */
    
    switch (elements[i].flag) {
    case TYPE_PLUS:
    case TYPE_MINUS:
    case TYPE_MULTIPLY:
    case TYPE_RHO:
    case TYPE_DROP:
      {
        Element leftValue;
        int operatorFlag = elements[i].flag;
        switch (elements[i-1].flag) {
        case TYPE_ARRAY_1D:
        case TYPE_ARRAY_2D:
        case TYPE_ARRAY_3D:
          leftValue = elements[i-1];
          i = i-2;
          break;
        case TYPE_VARIABLE:
          leftValue = getVariableValue(environment, elements[i-1].name);
          i = i-2;
          break;
        case TYPE_CLOSE:
          {
            pair<int, Element> subResult = exec(i-2, elements, environment);
            if (!verifyCloseResult(subResult, elements)) {
              REPORT("Verification failed.");
              throw EXCEPTION_RE;
            }
            i = subResult.first - 2;
            leftValue = subResult.second;
            break;
          }
        default:
          REPORT("Unexpected left.");
          throw EXCEPTION_RE;
        }
        
        currentResult = calculateOperator(operatorFlag, leftValue, currentResult);
      }
      break;
    case TYPE_SLASH:
      switch (elements[i-1].flag) {
      case TYPE_PLUS:
      case TYPE_MINUS:
      case TYPE_MULTIPLY:
        currentResult = calculateSlash(elements[i-1].flag, currentResult);
        i = i-2;
        break;
      default:
        REPORT("Unexpected left operator.");
        throw EXCEPTION_RE;
      }
      break;
    case TYPE_EQUAL:
      if (currentResult.flag == TYPE_NULL) {
        REPORT("Unexpected right operand.");
        throw EXCEPTION_RE;
      }
      if (elements[i-1].flag != TYPE_VARIABLE) {
        REPORT("Unexpected leftt operand.");
        throw EXCEPTION_RE;
      }
      environment[elements[i-1].name] = currentResult;
      i = i-2;
      break;
    case TYPE_IOTA:
      currentResult = calculateItoa(currentResult);
      i = i-1;
      break;
    case TYPE_ARRAY_1D:
    case TYPE_ARRAY_2D:
    case TYPE_ARRAY_3D:
      if (currentResult.flag == TYPE_NULL) {
        currentResult = elements[i];
        i--;
        break;
      } else {
        REPORT("Unexpected number or array appears.");
        throw EXCEPTION_RE;
      }
    case TYPE_VARIABLE:
      if (currentResult.flag == TYPE_NULL) {
        currentResult = getVariableValue(environment, elements[i].name);
        if (currentResult.flag == TYPE_NULL) {
          REPORT("Undefined variable.");
          throw EXCEPTION_RE;
        }
        i--;
        break;
      } else {
        REPORT("Unexpected variable appears.");
        REPORT(elements[i].name);
        throw EXCEPTION_RE;
      }
    case TYPE_CLOSE:
      {
        pair<int, Element> subResult = exec(i-1, elements, environment);
        if (!verifyCloseResult(subResult, elements)) {
          REPORT("Verification failed.");
          throw EXCEPTION_RE;
        }
        i = subResult.first - 2;
        currentResult = subResult.second;
        break;
      }
    case TYPE_OPEN:
      if (currentResult.flag != TYPE_NULL) {
        return make_pair(i+1, currentResult);
      } else {
        REPORT("Unexpected number or array appears.");
        throw EXCEPTION_RE;
      }
    }
  }
  return make_pair(i, currentResult);
}

Element solve(Environment &environment, string line) {
  vector<Element> elements = parse(line);
  elements = convertContinuousNumbersToArray1D(elements);
  pair<int, Element> result = exec(elements.size() - 1, elements, environment);
  if (result.first != -1) {
    REPORT("Unexpected number or array appears.");
    throw EXCEPTION_RE;
  }
  return result.second;
}

string toString1D(const vector<int> &v) {
  ostringstream buf;
  
  for (int i = 0; i < v.size(); i++) {
    buf << " " << v[i];
  }
  buf << endl;
  return buf.str();
}

string toString2D(const vector<vector<int> > &v) {
  ostringstream buf;
  for (int i = 0; i < v.size(); i++) {
    buf << toString1D(v[i]);
  }
  return buf.str();
}

string toString3D(const vector<vector<vector<int> > > &v) {
  ostringstream buf;
  buf << toString2D(v[0]);
  for (int i = 1; i < v.size(); i++) {
    buf << endl << toString2D(v[i]);
  }
  return buf.str();
}



string toString(const Element &element) {
  switch (element.flag) {
  case TYPE_PLUS:
    return "+";
  case TYPE_MINUS:
    return "-";
  case TYPE_MULTIPLY:
    return "*";
  case TYPE_SLASH:
    return "/";
  case TYPE_EQUAL:
    return "=";
  case TYPE_IOTA:
    return "itoa";
  case TYPE_RHO:
    return "rho";
  case TYPE_DROP:
    return "drop";
  case TYPE_OPEN:
    return "(";
  case TYPE_CLOSE:
    return ")";
  case TYPE_ARRAY_1D:
    return toString1D(element.array1d);
    break;
  case TYPE_ARRAY_2D:
    return toString2D(element.array2d);
    break;
  case TYPE_ARRAY_3D:
    return toString3D(element.array3d);
    break;
  case TYPE_VARIABLE:
    return ("n:" + element.name);
  case TYPE_NULL:
    return "null";
  default:
    REPORT("Unexpected type.");
    throw EXCEPTION_RE;
  }
}

void print(const Element &element) {
  cout << toString(element);
}
