#ifndef LAB5_EXPRESSIONMANAGER_H
#define LAB5_EXPRESSIONMANAGER_H
#include "ExpressionManagerInterface.h"
#include <stack>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

class ExpressionManager : public ExpressionManagerInterface {

public:


    ExpressionManager(string userExpression) {
     expression = userExpression;
    }
    ExpressionManager() {

    }
    ~ExpressionManager(){}

    int value(void) {
        string token;
        string post = postfix();
        stringstream in(post);
        vector<string> userPost;
        stack<string> t;
        int left = 0;
        int right = 0;
        int result = 0;
        while (in >> token) {
            userPost.push_back(token);
        }
        for (int i = 0; i < userPost.size(); ++i) {
            if (isDigit(userPost.at(i))) {
                t.push(userPost.at(i));
            } else if (isOperator(userPost.at(i))) {
                stringstream convert(t.top());
                convert >> right;
                t.pop();
                stringstream what(t.top());
                what >> left;
                t.pop();
                result = Evaluate(left, right, userPost.at(i));
                //convert result back to a string
                t.push(to_string(result));
            }

        }
        string finalResult;
        while (!t.empty()) {
            finalResult = t.top();
            t.pop();
        }
        int lastResult;
        stringstream conversion(finalResult);
        conversion >> lastResult;
        return lastResult;
    }

    string infix(void) {
        string token;
        stringstream in(expression);
        vector<string> userexpression;
        while (in >> token) {
            userexpression.push_back(token);
        }

        if (isBalenced(expression) == false) {
            return " Unbalenced";
        }
        bool order = isOperator(userexpression.at(0));
        for (int i = 1; i < userexpression.size() - 1; ++i) {
            if (isOperator(userexpression.at(i)) == false && isDigit(userexpression.at(i)) == false) {
                return " Illegal Operator";
            }
           if(isDigit(userexpression.at(i)) && isDigit(userexpression.at(i + 1))) {
               return " Missing Operator";
           }
           if(isMathVal(userexpression.at(i)) && isMathVal(userexpression.at(i + 1))) {
               return " Missing Operand";
           }

        }
        if(isMathVal(userexpression.at(userexpression.size()-1))) {
            return " Missing Operand";
        }
        return GetExpression();
    }



    //string illegalOperator
    string postfix(void) {
        vector<string> holder;
        string token;
        string postfix = "";

        stringstream in(expression);
        while(in >> token) {
            holder.push_back(token);
        }
        stack<string> t;
        bool balenced = true;
        int i = 0;


        for(int i = 0; i < holder.size(); ++ i) {
            if (isDigit(holder.at(i))) {
                postfix = postfix + " " + holder.at(i);
            } else if (Open(holder.at((i)))) {
                t.push(holder.at(i));
            } else if (Closed(holder.at(i))) {
                while (!t.empty() && !isPair(t.top(), holder.at(i))) {
                    postfix = postfix + " " + t.top();
                    t.pop();
                }
                if(!t.empty() && isPair(t.top(),holder.at(i))) {
                    t.pop();
                }

            } else if (isOperator(holder.at(i))) {
                while (!t.empty() && prec(holder.at(i)) <= prec(t.top())) {
                    postfix = postfix + " " + t.top();
                    t.pop();
                }
                t.push(holder.at(i));
            }
        }
        while(!t.empty()) {
            postfix = postfix + " " + t.top();
            t.pop();
        }

        cout << postfix;


return postfix;
    }
    string prefix(void) {
        string token;
        string prefix = expression;
        reverse(prefix.begin(),prefix.end());
        stringstream in(prefix);
        vector <string> items;
        while(in >> token) {
            items.push_back(token);
        }
        //switch all of the parenthesis
        for( int i = 0; i < items.size(); ++i) {
            if(Closed(items.at(i)) || Open(items.at(i)) ) {
               items.at(i) = Switch(items.at(i));
            }
        }
        //convert items vector to post fix
        string postfixversion;

        stack<string> t;
        bool balenced = true;
        int i = 0;

//postfix conversion
        for(int i = 0; i < items.size(); ++ i) {
            if (isDigit(items.at(i))) {
                postfixversion = postfixversion + " " + items.at(i);
            } else if (Open(items.at((i)))) {
                t.push(items.at(i));
            } else if (Closed(items.at(i))) {
                while (!t.empty() && !isPair(t.top(), items.at(i))) {
                    postfixversion = postfixversion + " " + t.top();
                    t.pop();
                }
                if(!t.empty() && isPair(t.top(),items.at(i))) {
                    t.pop();
                }

            } else if (isOperator(items.at(i))) {
                while (!t.empty() && prec(items.at(i)) < prec(t.top()) && t.top() != items.at(i)) {
                    postfixversion = postfixversion + " " + t.top();
                    t.pop();
                }
                t.push(items.at(i));
            }
        }
        while(!t.empty()) {
            postfixversion = postfixversion + " " + t.top();
            t.pop();
        }
         reverse(postfixversion.begin(), postfixversion.end());
        string finalPrefix = postfixversion;
        return finalPrefix;


    }
    string toString(void) const {

    }

    bool isMathVal( string b) {
        if( b == "+") {
            return true;
        }
        if( b == "-") {
            return true;
        }
        if( b == "*") {
            return true;
        }
        if( b == "/") {
            return true;
        }
        if( b == "%") {
            return true;
        }
        return false;
    }

    int Evaluate(int left, int right, string op) {
        int value = 0;
        if(op == "*") {
            value = left * right;
            return value;
        }
        if(op == "-") {
            value = left - right;
        }
        if(op == "+") {
            value = left + right;
        }
        if(op == "/") {
            value = left / right;
        }
        if(op == "%") {

        }
        return value;
    }
    int prec(string c) {
        int two = 2;
        int one = 1;
        int zero = 0;
        if (c == "*" || c == "/" || c == "%") {
            return two;
        }
        else if (c == "+" || c == "," || c == "-") {
            return one;
        }
       else if (c == "(" || c == "[" || c == "{") {
            return zero;
        }
        return zero;
    }
    string Switch(string c) {
        if(c == ")") {
            return "(";
        }
        if(c== "(") {
            return ")";
        }
        if(c == "{") {
            return "}";
        }
        if(c== "}") {
            return "{";
        }
        if( c == "[") {
            return "]";
        }
        if(c == "]") {
            return "[";
        }
        else return "a";

    }

    char parCheck(char b) {
        if (b == '[' || b == ']') {
            return true;
        }
        if (b == '{' || b == '}') {
            return true;
        }

        if (b == '[' || b == ']') {
            return true;
        }
        else return false;
    }



    string GetExpression() {
        string token;
        vector<string> updateExpression;
        stringstream in(expression);
        while(in >> token) {
            updateExpression.push_back(token);
        }
        string spacedExpression = "";

        for(int i = 0; i < updateExpression.size(); ++i ) {
            spacedExpression = spacedExpression + " " + updateExpression.at(i);
        }


        return spacedExpression;
    }
    string GetInitialExpression(){
        return expression;
    }

    void SetExpression(string in) {
        expression = in;
    }
    bool isDigit(string digit)  {
        vector<char> numbers = {'0','1','2','3','4','5','6','7','8','9'};
        int count = digit.size();
        for(int i = 0; i < digit.size(); ++i)  {
            for( int j = 0; j < numbers.size(); ++j) {
             if(digit.at(i) == numbers.at(j)){
                 count = count - 1;
             }
            }
        }
        if(count == 0) {
            return true;
        }
        else return false;
    }

    bool isOperator(string b){
        int num = 0;
       vector<string> operater = {"(","{","[","]","}",")","/","*","-","+","%"};
       for(int i = 0; i < operater.size(); ++i) {
           if(b == operater.at(i)) {
               num = num + 1;
           }
       }

       if(num == 1) {
           return true;
       }
       else {
           return false;
       }
    }


    bool Open(string open) {
        string isOpen = "({[";
        return isOpen.find(open) != string::npos;
    }
    bool Closed(string closed) {
        string isClosed = ")}]";
        return isClosed.find(closed) != string::npos;
    }


    bool isPair(string left, string right)  {
        bool pair;
        if(left == "(" && right == ")") {
            pair = true;
        }
        else if(left == "{" && right == "}") {
            pair = true;
        }
        else if(left == "[" && right == "]")  {
            pair = true;
        }
        else {
            pair = false;
        }
        return pair;

    }



    bool isBalenced(const string& input) {
        vector<string> holder;
        string token;
        string postfix = "";

        stringstream in(input);
        while(in >> token) {
            holder.push_back(token);
        }
        stack<string> t;
        bool balenced = true;
        int i = 0;


        while(balenced && i < holder.size()) {
            if (isDigit(holder.at(i))) {
                postfix = postfix + holder.at(i);
            } else if (Open(holder.at((i)))) {
                t.push(holder.at(i));
            } else if (Closed(holder.at(i))) {
                while (!t.empty() && !isPair(t.top(), holder.at(i))) {
                    postfix = postfix + t.top();
                    t.pop();
                }
                if(t.empty() ||!isPair(t.top(), holder.at(i))) {
                    balenced = false;
                }

                if(!t.empty() && isPair(t.top(),holder.at(i))) {
                    t.pop();
                }

            } else if (isOperator(holder.at(i))) {
                while (!t.empty() && prec(holder.at(i)) <= prec(t.top())) {
                    postfix = postfix + t.top();
                    t.pop();
                }
                t.push(holder.at(i));
            }
            ++i;
        }
        while(!t.empty()) {
            postfix = postfix + t.top();
            t.pop();
        }

return balenced && t.empty();

    }



private:
    string expression;
    string Postfix = "";




























};

#endif //LAB5_EXPRESSIONMANAGER_H
