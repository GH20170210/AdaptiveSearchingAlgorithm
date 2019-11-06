#include <iostream>
#include <ctime>
#include <random>
using namespace std;

int activate(double input[4], double weight[3], double sita);
void train(double sita, double speed, double weight[3], double input[4], int output);

int main()
{
    //初始化网络
    double speed; //学习速率，即权值调整系数大小
    double sita;  //训练网络阈值
    int epoch;    //训练网络次数，算法退出条件之一
    //double error_expectation; //期望误差，算法退出条件之一
    int input_number; //输入网络的输入向量个数
    int i, j, k;
    double weight[3]; //权向量
    double input[4];
    /*  double input_test[8][4] = {
        {0.81, 1.02, 8.85, 1}, {0.82, 0.98, 8.67, 1}, {0.78, 0.99, 8.75, 1}, {0.79, 1.01, 8.80, 1}, {0.56, 0.85, 7.32, 0}, {0.58, 0.86, 7.33, 0}, {0.59, 0.83, 7.29, 0}, {0.57, 0.84, 7.31, 0}}; */
    int output; //输出向量

    default_random_engine e(time(0));
    uniform_real_distribution<double> u(-0.5, 0.5);

    cout << "Now let's start the perceptron neural network algorithm!" << endl;
    cout << "Please input the threshold value(float):" << endl;
    cin >> sita;
    cout << "Please input the training epochs of the network:" << endl;
    cin >> epoch;
    //由于此处为线性分类器，只有1、0输出，因此无需预期误差

    //一系列初始化工作
    sita = 10;
    epoch = 50;
    speed = 0.03;

    for (i = 0; i < 3; i++)
    {
        weight[i] = u(e);
    }
    cout << "The initialed weight is :" << endl;
    for (i = 0; i < 3; i++)
    {
        cout << weight[i] << " ";
    }
    cout << endl;
    cout << "*****************************" << endl;

    cout << "Please input the number of the input for the network:" << endl;
    cin >> input_number;
    cout << "Now ,you can make the input, like 0.5 0.5 0.5 1,the fiirst,second,third value is the value of type,the forth value is the output of expectation." << endl;

    for (i = 0; i < input_number; i++)
    {
        cout << "The no." << i << " input is" << endl;
        for (j = 0; j < 4; j++)
        {
            cin >> input[j];
        }
        /*  for(j=0;j<4;j++){
            input[j]=input_test[i][j];
        } */

        for (k = 0; k < epoch; k++)
        {
            output = activate(input, weight, sita);
            train(sita, speed, weight, input, output);
        }
    }
    cout << "***********************************************************" << endl;
    cout << "The trained weight is :" << endl;
    for (i = 0; i < 3; i++)
    {
        cout << weight[i] << " ";
    }
    cout << "****************************" << endl;
    cout << endl;

    cout << "The resualt below the line is the resualt of [0.60,0.88,7.45] and [0.76,1.00,8.78]." << endl;
    double test1[3] = {0.60, 0.88, 7.45};
    double test2[3] = {0.76, 1.00, 8.78};
    double test_output[2];
    test_output[0] = test1[0] * weight[0] + test1[1] * weight[1] + test1[2] * weight[2] - sita;
    test_output[1] = test2[0] * weight[0] + test2[1] * weight[1] + test2[2] * weight[2] - sita;
    for (i = 0; i < 2; i++)
    {
        if (test_output[i] >= 0)
        {
            cout << "This is chelizi!" << endl;
        }
        else if (test_output[i] < 0)
        {
            cout << "This is cherry!" << endl;
        }
    }
    cout << "Now,if you wanna test other data,Please input the number of the data you wan to test(if not ,please input'0')" << endl;
    int test_number;
    cin>>test_number;
    for (i = 0; i < test_number; i++)
    {
        cout << "The no." << i << " data is:" << endl;
        for (j = 0; j < 3; j++)
        {
            cin >> test1[j];
        }
        cout<<"The resualt of classification is:"<<endl;
         if (test1[0] * weight[0] + test1[1] * weight[1] + test1[2] * weight[2] - sita>= 0)
        {
            cout << "This is chelizi!" << endl;
        }
        else 
        {
            cout << "This is cherry!" << endl;
        }
    }
    return 0;
}

//-----------------------------The border for the main and the sub--------------------------------//
//-------------------------------------------激活函数----------------------------------------------//
int activate(double input[4], double weight[3], double sita)
{
    int output;
    if ((input[0] * weight[0] + input[1] * weight[1] + input[2] * weight[2] - sita) >= 0)
    {
        output = 1;
    }
    else
    {
        output = 0;
    }
    return output;
}
//--------------------------------------------------------------------------------------------------//
//-------------------------------------------权向量训练函数------------------------------------------//
void train(double sita, double speed, double weight[3], double input[4], int output)
{
    int k;
    for (k = 0; k < 3; k++)
    {
        weight[k] = weight[k] + speed * input[k] * (input[3] - output);
    }
    //感知器为线性分类器，故输出仅为1或0，因此设置期望误差意义不大
    /*  for (i = 0; i < 8; i++)
        {
            y_step[i] = input[i][0] * weight[0] + input[i][1] * weight[1] + input[i][2] * weight[2] - sita;
            if (y_step[i] >= 0)
            {
                output[i] = 1;
                y_step[i] = 1; //无用，因为输出即为分类函数的输出
            }
            else
            {
                output[i] = 0;
                y_step[i] = 0; //无用，因为输出即为分类函数的输出
            }
        } */
}
//--------------------------------------------------------------------------------------------------//
