#include<stdio.h>
#include <iostream>
#include"CMatrix.h"
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "Matlab.h"
#include<conio.h>

using namespace std;


void stressTesting(){

while(true){
        CMatrix B(32,32,CMatrix::MI_RAND);
        //cout<<B.getString2()<<"  ";
         float ans1 = B.getDeterminant();
        float ans2 = B.getDeterminant3();

        if(abs(ans1-ans2)<0.001)
            cout<<" test is ok"<<endl;
        else {
            cout<<endl<<"testFailed"<<endl;
            cout<<"Det 1 = "<<ans1<<endl<<"Det 2 = "<<ans2<<endl;
            B.writeMatrixInFile("error.txt");
            break;
        }


    }

}
void printVector( vector<Matlab>& myVector){
    for(int i =0; i<myVector.size(); i++) {
        cout<<myVector[i].getString()<<endl;
    }
}


int main(int argc, char*argv[])
{

    Matlab x ;//-33333+5656+99+999-565-565-5656
    string e = "99-356+985-8996+5632-965+545+51562-5566-3333-33333+5656+99+999-565-565-5656+5-55-55+100+3000-4000+200+200+200+200+200-200-200-200+2000+200-20000-2*-2+2-2-4000+200+200+200+200+200-200-200-200+2000+200-20000";
    //cout<<x.checkStringForMatrix(e)<<"here"<<endl;
    cout<<x.calcSimpleExpression(e)<<endl;
e = "8+2";
cout<<x.calcSimpleExpression(e)<<endl;
e = "8-2";
cout<<x.calcSimpleExpression(e)<<endl;
e = "-8+2";
cout<<x.calcSimpleExpression(e)<<endl;
e = "-8-2";
cout<<x.calcSimpleExpression(e)<<endl;
e = "-8-2+2*2";
cout<<x.calcSimpleExpression(e)<<endl;


    getch();



    /*  a brief example on how to use vectors  */

    vector<Matlab> myVector;   //initialized a vector  for matlab objects
    CMatrix m("[1 2 3;4 5 6]");// initialized a matrix  m
    myVector.push_back( Matlab("A",m));// put a new Matlab object with name A and matrix m in myVector;
    cout<<myVector[0].getString()<<endl;// the same as array myVector[0] will return the firstMatlab object pushed in myVector


   /*           Now I will add another Matlab objects for u to test           */

    myVector.push_back(Matlab("luffy",CMatrix("[1 2 3 ; 4 5 6 ; 7 8 9]")));

    myVector.push_back(Matlab("x",CMatrix("[0 0 0 0 ; 0 0 0 0 ; 0 0 0 0]")));


    cout<<endl<<"printing the Matlab Objects in the Vector"<<endl<<endl;
    printVector(myVector);



    return 0;
}
