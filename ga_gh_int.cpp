#include <iostream>
#include <string.h>
#include <ctime>
#include <random>
using namespace std;

typedef struct chrom
{
    short int chromosome[4]; //染色体编码，此处为5位二进制
    double fit;
    double rfit; //适应值比例
    double cfit; //累计适应值
} chrom;
//定义初始种群，下一代种群
chrom population_current[30];
chrom population_nextgene[30];
double pr = 0.85;
double px = 0.05;

double binary_to_decimal(chrom chrom);
double fit(double chrom);
void *Initial_population(chrom population_current[30]);
void *display_chroms(chrom population_current[30]);
void *pick_chroms(chrom population_nextgene[30]);
void *sort_chroms(chrom population_current[30]);
void *cross_chroms(chrom population_nextgene[30]);
void *variation_chroms(chrom population_nextgene[30]);
void *pick_chroms_tournament(chrom population_nextgene[30]);

int main()
{
    int number_gene; //设置遗传次数，遗传代数上限
    int i, j;
    double max, x; //最大适应值
    max = 0;
    cout << "Please input the number of generation:" << endl;
    cin >> number_gene;
    cout << endl;
    //适应度函数为f(x)=15*x-x^2
    cout << "Initial the population now!" << endl;
    Initial_population(population_current);
    display_chroms(population_current);
    max = population_current[0].fit;
    //display_chroms(population_current);
    //sort_chroms(population_current);
    //默认遗传代数为100
    cout << "*****************************************************************************" << endl;
    for (i = 1; i <50; i++)
    {
        for (j = 0; j < 30; j++)
        {
            population_nextgene[j] = population_current[j]; //准备对种群个体进行选择
        }

        pick_chroms_tournament(population_nextgene); //挑选优秀个体,此处得到的population_nextgene即为选中的个体
        /* cout << "***************The chroms has been picked*************" << endl;
        display_chroms(population_nextgene);
        cout << "******************************************************" << endl; */

        cross_chroms(population_nextgene); //接着对选中的个体染色体进行交叉
                                           /*  cout << "***************The chroms has been crossed*************" << endl;
        display_chroms(population_nextgene);
        cout << "******************************************************" << endl; */

        variation_chroms(population_nextgene); //个体进行变异
                                               /*  cout << "***************The chroms has been variated*************" << endl;
        display_chroms(population_nextgene);
        cout << "******************************************************" << endl; */

        for (j = 0; j < 30; j++)
        {
            population_current[j] = population_nextgene[j]; //  种群更新
        }
    }
    // display_chroms(population_current);
    cout << "The trained chroms is :" << endl;
    display_chroms(population_current);
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    //找到最大值，即从最后一代选择最优个体
    for (i = 1; i < 30; i++)
    {
        if (population_current[i].fit > max)
        {
            max = population_current[i].fit;
            x = binary_to_decimal(population_current[i]);
        }
    }
    //找到最优个体
    cout << "The maximum value is" << max << "when x=" << x << ".";
    return 0;
}

//***************************************主函数子函数分割线************************************************//
//                                        调用的函数如下
//------------------------------------------初始化种群----------------------------------------------------//
void *Initial_population(chrom population_current[30])
{
    int i, j, random;
    double sum = 0;
    for (i = 0; i < 30; i++)
    {
        for (j = 0; j < 4; j++)
        {
            random = (rand()) % 2;
            population_current[i].chromosome[j] = random;
        }
        double chromosome = binary_to_decimal(population_current[i]);
        population_current[i].fit = fit(chromosome);
        sum += population_current[i].fit;
    }
    for (i = 0; i < 30; i++)
    {
        population_current[i].rfit = population_current[i].fit / sum; //计算适应值比例
        if (i == 0)
            population_current[i].cfit = population_current[i].rfit; //初始化累计适应值
        else
            population_current[i].cfit = population_current[i].rfit + population_current[i - 1].cfit;
    }
    return 0;
}
//----------------------------------------------------------------//
//--------------------二进制转十进制，即进行解码---------------------//
double binary_to_decimal(chrom chrom)
{
    double chromosome = 0;
    int a = 1;
    int i;
    for (i = 3; i >= 0; i--)
    {
        chromosome = chromosome + a * (double)chrom.chromosome[i];
        a = a * 2;
    }
    return chromosome;
}
//------------------------------------------------------------------//
//--------------------------计算适应值------------------------------//
double fit(double chrom)
{
    double f;
    f = chrom * 15 - chrom * chrom;
    return f;
}
//-----------------------------------------------------------------//
void *display_chroms(chrom population_current[30])
{
    //++++++++++++++++++++++++++++++++++++++++显示初始种群+++++++++++++++++++++++++++++++++++++++++++//
    int i, j;
    for (i = 0; i < 30; i++)
    {
        cout << "chrom[" << i << "]:" << endl;
        cout << "     "
             << "chromesome=";
        for (j = 0; j < 4; j++)
        {
            cout << population_current[i].chromosome[j];
        }
        cout << endl;
        cout << "     "
             << "fit=" << population_current[i].fit;
        cout << "     "
             << "rfit=" << population_current[i].rfit;
        cout << "     "
             << "cfit=" << population_current[i].cfit;
        cout << endl;
    }
    return 0;
}
//-------------------选择优秀个体，轮盘赌法----------------------------//
void *pick_chroms(chrom population_nextgene[30])
{
    int j, k;
    double p;
    chrom population_temp[30];
    default_random_engine e(time(0));
    uniform_real_distribution<double> u(0, 1);
    for (j = 0; j < 30; j++)
    {
        p = u(e);
        for (k = 0; k < 29; k++)
        {
            if (p < population_nextgene[0].cfit)
            {
                population_temp[j] = population_nextgene[0];
            }
            else if (p > population_nextgene[k].cfit && p <= population_nextgene[k + 1].cfit)
            {
                population_temp[j] = population_nextgene[k + 1];
            }
        }
    }
    for (j = 0; j < 30; j++)
    {
        population_nextgene[j] = population_temp[j];
    }
    return 0;
}

//-----------------选择优秀个体，使用竞争法选择，TournamentSelection-----------//
//-----------------此处每次选择的个体是2，因此也称为二进制竞争选择--------------//
void *pick_chroms_tournament(chrom population_nextgene[30])
{
    default_random_engine e(time(0));
    uniform_int_distribution<unsigned> u(0, 29);
    int a, b, i, j;
    chrom population_temp[30];
    for (i = 0; i < 30; i++)
    {
        a = u(e);
        b = u(e);
        if (population_nextgene[a].fit > population_nextgene[b].fit)
        {
            population_temp[i] = population_nextgene[a];
        }
        else
        {
            population_temp[i] = population_nextgene[b];
        }
    }
    for (j = 0; j < 30; j++)
    {
        population_nextgene[j] = population_temp[j];
    }
    return 0;
}
//--------------------------------------------------------------------------//

//------------------对种群适应值进行排序-------------------------------//
void *sort_chroms(chrom population_current[30])
{
    int p, q;
    double r;
    for (p = 0; p < 30; p++)
    {
        for (q = 0; q < p; q++)
        {
            if (population_current[q].fit > population_current[q + 1].fit)
            {
                r = population_current[q].fit;
                population_current[q].fit = population_current[q + 1].fit;
                population_current[q + 1].fit = r;
            }
        }
    }
    return 0;
}
//-----------------------------------------------------------------------//

//-------------------对选中的个体进行交叉,这里用单点交叉，产生随机数决定交叉点----------------------------------//
void *cross_chroms(chrom population_nextgene[30])
{
    default_random_engine r(time(0));
    uniform_int_distribution<unsigned> p(0, 3);
    uniform_real_distribution<double> q(0, 1);
    int i, j;
    i = 1;
    int t, a, b, c;
    double chromosome;
    double sum = 0;
    //单点交叉
    do
    {
        t = q(r);
        if (t < pr)
        {
            a = population_nextgene[i].chromosome[2];
            population_nextgene[i].chromosome[2] = population_nextgene[i - 1].chromosome[2];
            population_nextgene[i - 1].chromosome[2] = a;
            a = population_nextgene[i].chromosome[3];
            population_nextgene[i].chromosome[3] = population_nextgene[i - 1].chromosome[3];
            population_nextgene[i - 1].chromosome[3] = a;
        }
        i += 2;

    } while (i < 30);
    //随机点随机个数交叉
    //交叉完之后对个体的适应值需要重新计算
    do
    {
        t = q(r);
        b = p(r);
        if (t < pr)
        {
            for (j = 0; j < b; j++)
            {
                c = p(r);
                a = population_nextgene[i].chromosome[c];
                population_nextgene[i].chromosome[c] = population_nextgene[i - 1].chromosome[c];
                population_nextgene[i - 1].chromosome[c] = a;
            }
        }
        i += 2;
    } while (i < 30);

    //更新cfit,rfit,fit
    for (i = 0; i < 30; i++)
    {
        chromosome = binary_to_decimal(population_nextgene[i]);
        population_nextgene[i].fit = fit(chromosome);
        sum += population_nextgene[i].fit;
    }
    for (i = 0; i < 30; i++)
    {
        population_nextgene[i].rfit = population_nextgene[i].fit / sum;
        if (i == 0)
        {
            population_nextgene[i].cfit = population_nextgene[i].rfit;
        }
        else
        {
            population_nextgene[i].cfit = population_nextgene[i - 1].cfit + population_nextgene[i].rfit;
        }
    }

    return 0;
}
//------------------------------------------------------------------------//

//-------------------交叉完的个体进行变异----------------------------------//
void *variation_chroms(chrom population_nextgene[30])
{
    default_random_engine e(time(0));
    uniform_real_distribution<double> u(0, 1);
    uniform_int_distribution<unsigned> p(0, 3);
    //随机点进行变异
    int i, a;
    double r, chromosome, sum;
    sum = 0;
    for (i = 0; i < 30; i++)
    {
        r = u(e);
        if (r < px)
        {
            a = p(e);
            if (population_nextgene[i].chromosome[a] == 0)
            {
                population_nextgene[i].chromosome[a] = 1;
            }
            else if (population_nextgene[i].chromosome[a] == 1)
            {
                population_nextgene[i].chromosome[a] = 0;
            }
        }
    }
    //更新fit,rfit,cfit
    for (i = 0; i < 30; i++)
    {
        chromosome = binary_to_decimal(population_nextgene[i]);
        population_nextgene[i].fit = fit(chromosome);
        sum += population_nextgene[i].fit;
    }
    for (i = 0; i < 30; i++)
    {
        population_nextgene[i].rfit = population_nextgene[i].fit / sum;
        if (i == 0)
        {
            population_nextgene[i].cfit = population_nextgene[i].rfit;
        }
        else
        {
            population_nextgene[i].cfit = population_nextgene[i - 1].cfit + population_nextgene[i].rfit;
        }
    }
    return 0;
}
//-----------------------------------------------------------------------//