#include <iostream>
#include <fstream>

using namespace std;

string random_col()
{
    int num = (rand() % 7) + 1;
    return "int(_" + to_string(num) + ")";
}

string random_number()
{
    int num = rand() % 35;
    return to_string(num);
}

string random_string_expr(int depth, string& input_str, int type)
{
    string expr;
    if (depth == 0)
    {
        switch (type)
        {
            case 1:
            case 4:
                expr = random_number();
                break;
            case 2:
                expr = "\"" + input_str + "\"";
                break;
            case 3:
                expr = random_col();
                break;
        }
        return expr;
    }

    int option;
    if (type == 1)  //return type is int
    {
        switch (option = rand() % 7)
        {
            case 0:
                expr = "avg(" + random_string_expr(depth-1, input_str, 3) + " )";
                break;
            case 1:
                expr = "count(" + random_string_expr(depth-1, input_str, 3) + " )"; 
                break;
            case 2:
                expr = "max(" + random_string_expr(depth-1, input_str, 3) + " )";
                break;
            case 3:
                expr = "min(" + random_string_expr(depth-1, input_str, 3) + " )";
                break;
            case 4:
                expr = "sum(" + random_string_expr(depth-1, input_str, 4) + ", " +
                        random_string_expr(depth-1, input_str, 4) + " )";
                break;
	    case 5:
		expr = "charlength( " + random_string_expr(depth-1, input_str, 2) + " )";
		break;
	    case 6:
		expr = "characterlength( " + random_string_expr(depth-1, input_str, 2) + " )";
		break;
        }
    }
    else if (type == 2)  // return type is string
    {
        switch (option = rand() % 3)
        {
            case 0:
                expr = "lower( " + random_string_expr(depth-1, input_str, 2) + " )";
                break;
            case 1:
                expr = "upper( " + random_string_expr(depth-1, input_str, 2) + " )";
                break;
            case 2:
                expr = "substr( " + random_string_expr(depth-1, input_str, 2) + ", " +
                        random_string_expr(depth-1, input_str, 1) + ", " +
                        random_string_expr(depth-1, input_str, 1)  + " )";
                break;
        }    
    }
    else if (type == 3)  // return type integer column number
    {
        expr = random_col();
    }
    else if (type == 4)  // return type randon number
    {
        expr = random_number();
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
            const std::string input_query = "select " + random_string_expr(3, input_str, (rand() % 2) + 1)
		    				+ " from stdin;";
            query_file << input_query << endl;
	    //cout << input_query << endl;
            reps--;
        }
	query_file.close();
    }
    return 0;
}

