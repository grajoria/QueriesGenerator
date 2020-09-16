#include <iostream>
#include <fstream>
#define NUM_COLUMN 3

using namespace std;

enum Return_type { INTEGER = 0,
            STRING = 1,
	    MIX_COL_NUM = 2,
	    COLUMN = 3,
	    NUMBER = 4};

string random_arth_op()
{
    string op;
    switch(rand() % 5)
    {
        case 0:
            op = "+";
            break;
        case 1:
            op = "-";
            break;
        case 2:
            op = "*";
            break;
        case 3:
            op = "/";
            break;
        case 4:
            op = "^";
            break;
    }
    return op;
}

string random_compare_op()
{
  string op;
  switch(rand() % 6)
  {
        case 0:
            op = ">";
            break;
        case 1:
            op = "<";
            break;
        case 2:
            op = ">=";
            break;
        case 3:
            op = "<=";
            break;
        case 4:
            op = "==";
            break;
        case 5:
            op = "!=";
            break;
    }
    return op;
}

string random_col()
{
    return "int(_" + to_string(1 + (rand() % NUM_COLUMN)) + ")";
}

string random_number()
{
    return "int(" + to_string(rand() % 10) + ")";
}

string random_num_expr(int depth)
{
    if (depth == 0)
    {
        if ((rand() % 2) == 0)
        {
            return random_col();
        }
        else
        {
            return random_number();
        }
    }
    return random_num_expr(depth-1) + random_arth_op() +
        random_num_expr(depth-1);
}

string random_query_expr(int depth, string& input_str, int type)
{
    string expr;
    if (depth == 0)
    {
        switch (type)
        {
            case INTEGER:
                expr = random_number();
                break;
            case STRING:
                expr = "\"" + input_str + "\"";
                break;
            case MIX_COL_NUM:
                expr = random_num_expr(depth);
                break;
        }
        return expr;
    }

    int option;
    if (type == INTEGER)  //return type is int
    {
        switch (option = rand() % 7)
        {
            case 0:
                expr = "avg(" + random_col() + random_arth_op() + random_num_expr(depth-1) +
			") " + random_arth_op() + " "  + random_num_expr(depth-1);
                break;
            case 1:
                expr = "count("+ random_col() + ") " + random_arth_op() + " " +
			random_num_expr(depth-1);
                break;
            case 2:
                expr = "max(" + random_col() + random_arth_op() + random_num_expr(depth-1) +
			") " + random_arth_op() + " " + random_num_expr(depth-1);
                break;
            case 3:
                expr = "min(" + random_col() + random_arth_op() + random_num_expr(depth-1) +
                        ") " + random_arth_op() + " " + random_num_expr(depth-1);
                break;
            case 4:
                expr = "sum(" + random_col() + ") " + random_arth_op() + " " +
			random_num_expr(depth-1);
                break;
	    case 5:
		expr = "charlength(" + random_query_expr(depth-1, input_str, STRING) + ")";
		break;
	    case 6:
		expr = "characterlength(" + random_query_expr(depth-1, input_str, STRING) + ")";
		break;
        }
    }
    else if (type == STRING)  // return type is string
    {
        switch (option = rand() % 3)
        {
            case 0:
                expr = "lower(" + random_query_expr(depth-1, input_str, STRING) + ")";
                break;
            case 1:
                expr = "upper(" + random_query_expr(depth-1, input_str, STRING) + ")";
                break;
            case 2:
                expr = "substr(" + random_query_expr(depth-1, input_str, STRING) + ", " +
                        random_query_expr(depth-1, input_str, INTEGER) + ", " +
                        random_query_expr(depth-1, input_str, INTEGER)  + ")";
                break;
        }
    }
    else if (type == MIX_COL_NUM)
    {
        expr = random_num_expr(depth-1);
    }
    else if (type == COLUMN)  // return type integer column number
    {
        expr = random_col();
    }
    else if (type == NUMBER)  // return type randon number
    {
        expr = random_number();
    }
    else
    {
        expr = "error";
    }
    return expr;
}

int main()
{
    srand(time(0));
    int reps;
    fstream query_file;
    query_file.open("queries.txt", ios::out);
    string input_str = "  $$AbCdEfGhIjKlMnOpQrStUvWxYz##  ";
    cout << "Enter number of quries to be generated: ";
    cin >> reps;
    if(query_file.is_open()) //checking whether the file is open
    {
        while (reps)
        {
            const string input_query = "select " + random_query_expr(3, input_str, (rand() % 3))
		    				+ " from stdin;";
            query_file << input_query << endl;
	    //cout << input_query << endl;
            reps--;
        }
	query_file.close();
    }
    return 0;
}

