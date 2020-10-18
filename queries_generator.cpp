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
    switch(rand() % 4)
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
       // case 4:
       //     op = "^";
       //     break;
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

string random_col(string& aws_expr)
{
    int num = 1 + (rand() % NUM_COLUMN);
    aws_expr = "cast(_" + to_string(num) + " as int)";
    return "int(_" + to_string(num) + ")";
}

string random_number(string& aws_expr)
{
    int num = rand() % 10;
    aws_expr = to_string(num);
    return "int(" + to_string(num) + ")";
}

string random_num_expr(int depth, string& aws_expr)
{
    string aws_expr1, aws_expr2, ceph_expr, op;
    if (depth == 0)
    {
        ceph_expr = random_number(aws_expr1);
        aws_expr = aws_expr1;
        return ceph_expr;
    }
    op = random_arth_op();
    ceph_expr = random_num_expr(depth-1, aws_expr1) + op +
            random_num_expr(depth-1, aws_expr2);
    aws_expr = aws_expr1 + op + aws_expr2;
    return ceph_expr;
}

string random_num_col_expr(int depth, string& aws_expr)
{
    string aws_expr1, aws_expr2, ceph_expr, op;
    if (depth == 0)
    {
        if ((rand() % 2) == 0)
        {
            ceph_expr = random_col(aws_expr1);
            aws_expr = aws_expr1;
            return ceph_expr;
        }
        else
        {
            ceph_expr = random_number(aws_expr1);
	    aws_expr = aws_expr1;
            return ceph_expr;
        }
    }
    op = random_arth_op();
    ceph_expr = random_num_col_expr(depth-1, aws_expr1) + op +
	    random_num_col_expr(depth-1, aws_expr2);
    aws_expr = aws_expr1 + op + aws_expr2;
    return ceph_expr;
}

string random_query_expr(int depth, string& input_str, int type, string& aws_expr)
{
    string ceph_expr;
    if (depth == 0)
    {
        switch (type)
        {
            case INTEGER:
                ceph_expr = random_number(aws_expr);
                break;
            case STRING:
                ceph_expr = "\"" + input_str + "\"";
		aws_expr = "\'" + input_str + "\'";
                break;
            case MIX_COL_NUM:
                ceph_expr = random_num_col_expr(depth, aws_expr);
                break;
        }
        return ceph_expr;
    }

    int option;
    if (type == INTEGER)  //return type is int
    {
        string ceph_col, aws_col, aws_expr1, aws_expr2, op1, op2;
        switch (option = rand() % 7)
        {
            case 0:
		ceph_col = random_col(aws_col);
		op1 = random_arth_op();
		op2 = random_arth_op();
                ceph_expr = "int(avg(" + ceph_col + op1 + random_num_col_expr(depth-1, aws_expr1) +
                        ") " + op2 + " "  + random_num_expr(depth-1, aws_expr2) + ")";
		aws_expr = "cast((avg(" + aws_col + op1 + aws_expr1 + ") " + op2 + " " + aws_expr2 +
                        ") as int)";
                break;
            case 1:
                ceph_col = random_col(aws_col);
		op1 = random_arth_op();
		op2 = random_arth_op();
                ceph_expr = "count(" + ceph_col + op1 + random_num_col_expr(depth-1, aws_expr1) +
			") " + op2 + " " + random_num_expr(depth-1, aws_expr2);
		aws_expr = "count(" + aws_col + op1 + aws_expr1 + ") " + op2 + " " + aws_expr2;
                break;
            case 2:
                ceph_col = random_col(aws_col);
                op1 = random_arth_op();
                op2 = random_arth_op();
                ceph_expr = "max(" + ceph_col + op1 + random_num_col_expr(depth-1,aws_expr1) + ") " +
			op2 + " " + random_num_expr(depth-1, aws_expr2);
		aws_expr = "max(" + aws_col + op1 + aws_expr1 + ") " + op2 + " " + aws_expr2;
                break;
            case 3:
                ceph_col = random_col(aws_col);
                op1 = random_arth_op();
                op2 = random_arth_op();
                ceph_expr = "min(" + ceph_col + op1 + random_num_col_expr(depth-1, aws_expr1) + ") " +
			op2 + " " + random_num_expr(depth-1, aws_expr2);
		aws_expr = "min(" + aws_col + op1 + aws_expr1 + ") " + op2 + " " + aws_expr2;
                break;
            case 4:
                ceph_col = random_col(aws_col);
		op1 = random_arth_op();
		op2 = random_arth_op();
                ceph_expr = "sum(" + ceph_col + op1 + random_num_col_expr(depth-1, aws_expr1) +
                        ") " + op2 + " " + random_num_expr(depth-1, aws_expr2);
		aws_expr = "sum(" + aws_col + op1 + aws_expr1 + ") " + op2 + " " + aws_expr2;
                break;
	    case 5:
		ceph_expr = "charlength(" + random_query_expr(depth-1, input_str, STRING,
					aws_expr1) + ")";
		aws_expr = "char_length(" + aws_expr1 + ")";
		break;
	    case 6:
		ceph_expr = "characterlength(" + random_query_expr(depth-1, input_str, STRING,
					aws_expr1) + ")";
		aws_expr = "character_length(" + aws_expr1 + ")";
                break;
        }
    }
    else if (type == STRING)  // return type is string
    {
        string aws_expr1, aws_expr2, aws_expr3;
        switch (option = rand() % 3)
        {
            case 0:
                ceph_expr = "lower(" + random_query_expr(depth-1, input_str, STRING, aws_expr1) +
                        ")";
                aws_expr = "lower(" + aws_expr1 + ")";
                break;
            case 1:
                ceph_expr = "upper(" + random_query_expr(depth-1, input_str, STRING, aws_expr1) +
                        ")";
                aws_expr = "upper(" + aws_expr1 + ")";
                break;
            case 2:
                ceph_expr = "substr(" + random_query_expr(depth-1, input_str, STRING, aws_expr1) +
                        ", " + random_query_expr(depth-1, input_str, INTEGER, aws_expr2) + ", " +
                        random_query_expr(depth-1, input_str, INTEGER, aws_expr3)  + ")";
                aws_expr = "substring(" + aws_expr1 + ", " + aws_expr2 + ", " + aws_expr3 + ")";
                break;
        }
    }
    else if (type == MIX_COL_NUM)
    {
        //string aws_expr1;
        ceph_expr = random_num_col_expr(depth-1, aws_expr);
	//aws_expr = aws_expr1;
    }
    else if (type == COLUMN)  // return type integer column number
    {
        ceph_expr = random_col(aws_expr);
    }
    else if (type == NUMBER)  // return type randon number
    {
        ceph_expr = random_number(aws_expr);
    }
    else
    {
        aws_expr = "error";
        ceph_expr = "error";
    }
    return ceph_expr;
}

int main()
{
    srand(time(0));
    int reps, depth;
    fstream query_file, aws_query_file;
    query_file.open("queries.txt", ios::out);
    aws_query_file.open("aws_queries.txt", ios::out);
    string input_str = "  %%AbCdEfGhIjKlMnOpQrStUvWxYz##  ";
    cout << "Enter number of quries to be generated: ";
    cin >> reps;
    cout << "Enter depth of queries to be generated: ";
    cin >> depth;
    if(query_file.is_open() && aws_query_file.is_open()) //checking whether the file is open
    {
        while (reps)
        {
            string aws_expr;
            int type;
            string ceph_query = "select ";
            string aws_query = "select ";
	    int projection = rand() % 4;
            while (projection > 1)
            {
                type = rand() % 3;
                ceph_query = ceph_query + random_query_expr(depth, input_str,
                                type, aws_expr) + ", ";
                aws_query = aws_query + aws_expr + ", ";
		projection--;
            }
            type = rand() % 3;
            ceph_query = ceph_query + random_query_expr(depth, input_str, type,
                            aws_expr)+ " from stdin;";
            aws_query = aws_query + aws_expr + " from s3object;";
            query_file << ceph_query << endl;
            aws_query_file << aws_query <<endl;
            reps--;
        }
	query_file.close();
    }
    return 0;
}

