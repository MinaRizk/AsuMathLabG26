#include <fstream>
#include "CMatrix.h"
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

CMatrix::CMatrix() {
  nR = nC = 0;
  values = NULL;
}

CMatrix::CMatrix(int nR, int nC, int initialization,
                 double initializationValue) {
  this->nR = nR;
  this->nC = nC;
  if ((nR * nC) == 0) {
    values = NULL;
    return;
  }

  values = new double *[nR];
  // Random seed
  srand(time(NULL));
  for (int iR = 0; iR < nR; iR++) {
    values[iR] = new double[nC];
    for (int iC = 0; iC < nC; iC++) {
      switch (initialization) {
      case MI_ZEROS:
        values[iR][iC] = 0;
        break;
      case MI_ONES:
        values[iR][iC] = 1;
        break;
      case MI_EYE:
        values[iR][iC] = (iR == iC) ? 1 : 0;
        break;
      case MI_RAND:

        values[iR][iC] = rand() % (100-1))/20.0;

        break;
      case MI_VALUE:
        values[iR][iC] = initializationValue;
        break;
      }
    }
  }
}
/*
void CMatrix::setSubMatrix(int r, int c, CMatrix &m) {/////////////////////////////////////////
  if ((r + m.nR) > nR || (c + m.nC) > nC)
    throw("Invalid matrix dimension");
  for (int iR = 0; iR < m.nR; iR++)
    for (int iC = 0; iC < m.nC; iC++)
      values[r + iR][c + iC] = m.values[iR][iC];
}

CMatrix::CMatrix(string s) {
  nR = nC = 0;
  values = NULL;
  copy(s);
}
CMatrix CMatrix::operator=(string s) {///////////////////////////////////////////////
  copy(s);
  return *this;
}
void CMatrix::copy(string s) {
  reset();
  char *buffer = new char[s.length() + 1];
  strcpy(buffer, s.c_str());
  const char *lineSeparators = ";\r\n";
  char *line = strtok(buffer, lineSeparators);
  char* remainlines = strtok(NULL, "");

  while (line) {
    CMatrix row;
    const char *separators = " []";
    char *token = strtok(line, separators);
    while (token) {
      CMatrix item(atof(token));
      row.addColumn(item);
      token = strtok(NULL, separators);
    }
    if (row.nC > 0 && (row.nC == nC || nR == 0))
      addRow(row);
    line = strtok(remainlines, lineSeparators);
    remainlines = strtok(NULL,"");
  }
  delete[] buffer;
}
*/
CMatrix::CMatrix(double d) {
  nR = nC = 0;
  values = NULL;
  copy(d);
}

/*void CMatrix::addColumn(CMatrix &m) {
  CMatrix n(max(nR, m.nR), nC + m.nC);
  n.setSubMatrix(0, 0, *this);
  n.setSubMatrix(0, nC, m);
  *this = n;
}

void CMatrix::addRow(CMatrix &m) {
  CMatrix n(nR + m.nR, max(nC, m.nC));
  n.setSubMatrix(0, 0, *this);
  n.setSubMatrix(nR, 0, m);
  *this = n;
}*/
CMatrix::CMatrix(CMatrix &m) {
  nR = nC = 0;
  values = NULL;
  copy(m);
}

CMatrix CMatrix::operator=(double d) {///////////////////////////////////////////////
  copy(d);
  return *this;
}
void CMatrix::operator+=(double d) {
  CMatrix C(nR, nC, MI_VALUE, d);
  add(C);
}
double CMatrix::getDeterminant2() //(waiting)
{
  if(nR!=nC)
    throw("Invalid matrix dimension");
  if(nR==1&&nC==1)return values[0][0];
    double value = 0, m = 1;
    for(int iR=0;iR<nR;iR++){
         value+= m * values[0][iR] * getCofactor(0, iR).getDeterminant2();
    m *= -1;
    } return value;
}

CMatrix CMatrix::operator+(CMatrix &m) {
  CMatrix r = *this;
  r += m;
  return r;
}
istream& operator >> (istream &is, CMatrix& m) { string s; getline(is, s, ']'); s+="]";
m = CMatrix(s);
return is;
}

ostream& operator << (ostream &os, CMatrix& m) { os<<m.getString(); return os; }
void CMatrix::operator-=(CMatrix &m)
{
   sub(m);
}

CMatrix operator /(double d,CMatrix &m){

        CMatrix ans(m.nR,m.nC);
        for (int iR = 0; iR<m.nR; iR++)
                for (int iC = 0; iC<m.nC; iC++)
                        ans.values[iR][iC] = d/m.values[iR][iC];
    return ans;
}

CMatrix CMatrix:: operator-(CMatrix &m){
    CMatrix r=*this;
    r.sub(m);
    return r;
}


void CMatrix::copy(double d) {
  reset();
  this->nR = 1;
  this->nC = 1;
  values = new double *[1];
  values[0] = new double[1];
  values[0][0] = d;
}

CMatrix::~CMatrix() { reset(); }

void CMatrix::copy(const CMatrix &m) {
  reset();
  this->nR = m.nR;
  this->nC = m.nC;
  if ((nR * nC) == 0) {
    values = NULL;
    return;
  }
  values = new double *[nR];
  for (int iR = 0; iR < nR; iR++) {
    values[iR] = new double[nC];
    for (int iC = 0; iC < nC; iC++) {
      values[iR][iC] = m.values[iR][iC];
    }
  }
}

string CMatrix::getString() {
  string s;
  for (int iR = 0; iR < nR; iR++) {
    for (int iC = 0; iC < nC; iC++) {
      char buffer[200];
      sprintf(buffer, "%8g\t", values[iR][iC]);
      s += buffer;
    }
    s += "\n";
  }
  return s;
}

void CMatrix::reset() {
  if (values) {
    for (int i = 0; i < nR; i++)
      delete[] values[i];
    delete[] values;
  }
  nR = nC = 0;
  values = NULL;
}

void CMatrix::add(const CMatrix &m) {
  if (nR != m.nR || nC != m.nC)
    throw("Invalid matrix dimension");
  for (int iR = 0; iR < nR; iR++)
    for (int iC = 0; iC < nC; iC++)
      values[iR][iC] += m.values[iR][iC];
}

CMatrix CMatrix::operator=(const CMatrix &m) {
  copy(m);
  return *this;
}
/*
void CMatrix::operator/=(double d) {
  for (int iR = 0; iR < nR; iR++)
    for (int iC = 0; iC < nC; iC++)
      values[iR][iC] /= d;
}*/

CMatrix CMatrix::operator--() {
  add(CMatrix(nR, nC, MI_VALUE, -1.0));
  return *this;
}
CMatrix CMatrix::operator--(int) {
  CMatrix r = *this;

  add(CMatrix(nR, nC, MI_VALUE, -1.0));
  return r;
}

// tested
CMatrix CMatrix::operator-() {//////////////////////////////////////
  CMatrix C = *this;
  for (int iR = 0; iR < nR; iR++)
    for (int iC = 0; iC < nC; iC++)
      C.values[iR][iC] = -values[iR][iC];
  return C;
}

CMatrix CMatrix::operator++() {
  CMatrix C(nR, nC, MI_VALUE, 1.0);
  add(C);
  return *this;
}


//r and c are the beginning of where you want ur subMatrix
// nr and nc are the size of the subMatrix

CMatrix CMatrix::getSubMatrix(int r, int c, int nr, int nc) {
        if((r+nr)>nR || (c+nc)>nC)
                throw("Invalid matrix dimension");
        CMatrix m(nr, nc);
        for(int iR=0;iR<m.nR;iR++)
                for(int iC=0;iC<m.nC;iC++)
                        m.values[iR][iC] = values[r+iR][c+iC];
        return m;
}

void CMatrix::operator+=(CMatrix &m) // tested
{
  add(m);
}

CMatrix CMatrix::operator++(int) {
  CMatrix C = *this;
  add(CMatrix(nR, nC, MI_VALUE, 1.0));
  return C;
}
CMatrix CMatrix::getCofactor(int r,
                             int c) // r and c represents the index of the
//	element that we want to find its cofactor//tested
{
  if (nR <= 1 && nC <= 1)
    throw("Invalid matrix dimension");
  CMatrix m(nR - 1, nC - 1);
  for (int iR = 0; iR < m.nR; iR++)
    for (int iC = 0; iC < m.nC; iC++) {
      int sR = (iR < r) ? iR : iR + 1;
      int sC = (iC < c) ? iC : iC + 1;
      m.values[iR][iC] = values[sR][sC];
    }
  return m;
}

void CMatrix::sub(CMatrix& m){ //tested and works - tuna
    if(nR!=m.nR||nC!=m.nC)
        throw("Invalid matrix dimension");
    for(int iR=0;iR<nR;iR++)
        for(int iC=0;iC<nC;iC++)
        values[iR][iC] -= m.values[iR][iC];
    }

void CMatrix::operator-=(double d){ //tested and works - tuna
    CMatrix x(nR,nC,MI_VALUE,d);
    sub(x);
    }

CMatrix CMatrix::operator+(double d){ //tested and works - tuna
    CMatrix r = *this;
    r+=d;
    return r;
    }
//CMatrix CMatrix::operator-(double d){ //tested and works - tuna
//    CMatrix r = *this;
//    r-=d;
//    return r;
//    }
//
//CMatrix CMatrix::getTranspose(){      //CMatrix in UML, void in header file!!
//    if(nR==1&&nC==1){
//        CMatrix m(values[0][0]);
//        return m;
//    }
//    CMatrix m(nC, nR);
//
//    for(int iR=0;iR<m.nR;iR++)
//        for(int iC=0;iC<m.nC;iC++)
//            m.values[iR][iC] = values[iC][iR];
//
//    return m;
//}
//CMatrix CMatrix::getInverse(){
//    if (nR != nC)
//        throw("Invalid matrix dimension");
//    double det =getDeterminant();
//   // if (det==0)
//      //  throw("Matrix has no Inverse");
//
//    CMatrix cof(nC , nR);//to find cofactor matrix
//    CMatrix trans (nC,nR);//to find transpose of matrix
//    CMatrix Inv (nC,nR);
//    int sign=1;
//    for(int i=0;i<nR;i++)
//    {
//        for(int j=0;j<nC;j++)
//        {
//            cof.values[i][j]=sign*getCofactor(i,j).getDeterminant();
//
//            sign*=-1;
//
//        }
//        if(nC%2==0) sign *= -1;
//    }
//
//    for(int i=0;i<nR;i++)
//        for(int j=0;j<nC;j++)
//            Inv.values[i][j]=cof.values[j][i] / det;
//
//    return Inv;
//
//
//
//
//
//
//
//
//}
//void CMatrix::mul(CMatrix& m)
//{
//        if (nC != m.nR)
//                throw("Invalid matrix dimension");
//        CMatrix r(nR, m.nC);
//
//        for (int iR = 0; iR<r.nR; iR++)
//                for (int iC = 0; iC<r.nC; iC++)
//                {
//                        r.values[iR][iC] = 0;
//                        for (int k = 0; k<m.nC; k++)
//                                r.values[iR][iC] += values[iR][k] *m.values[k][iC];
//                }
//        copy(r);
//}
//void CMatrix::operator*=(CMatrix& m)
//{
//        mul(m);
//}
//void CMatrix::operator*=(double d)
//{
//        for (int iR = 0; iR<nR; iR++)
//                for (int iC = 0; iC<nC; iC++)
//                        values[iR][iC] *= d;
//}
//CMatrix CMatrix::operator*(CMatrix& m)
//{
//        CMatrix r = *this;
//        r *= m;
//        return r;
//}
//CMatrix CMatrix::operator*(double d)
//{
//        CMatrix r = *this;
//        r *= d;
//        return r;
//}
//
//void CMatrix::div(CMatrix& m)
//{
//    CMatrix t;
//
//    t=m.getInverse();
//
//    mul(t);
//        /*if (nC != m.nR)
//                throw("Invalid matrix dimension");
//        CMatrix r(nR, m.nC);
//
//        for (int iR = 0; iR<r.nR; iR++)
//                for (int iC = 0; iC<r.nC; iC++)
//                {
//                        r.values[iR][iC] = 0;
//                        for (int k = 0; k<m.nC; k++)
//                                r.values[iR][iC] += values[iR][k] /
//m.values[k][iC];
//                }
//        copy(r);*/
//}
//void CMatrix::operator/=(CMatrix& m)
//{
//        div(m);
//}
//void CMatrix::operator/=(double d)
//{
//        for (int iR = 0; iR<nR; iR++)
//                for (int iC = 0; iC<nC; iC++)
//                        values[iR][iC] /= d;
//}
//CMatrix CMatrix::operator/(CMatrix& m)
//{
//        CMatrix r = *this;
//        r /= m;
//        return r;
//}
//CMatrix CMatrix::operator/(double d)
//{
//        CMatrix r = *this;
//        r /= d;
//        return r;
//}
//
///*
//*
//*
//*/
//string CMatrix:: getString2(){
//     string s="[";
//  for (int iR = 0; iR < nR; iR++) {
//    for (int iC = 0; iC < nC; iC++) {
//      char buffer[200];
//      sprintf(buffer, "%g ", values[iR][iC]);
//      s += buffer;
//    }
//    s += ";";
//  }
//  s+="]";
//  return s;
//}
//
//void CMatrix::writeMatrixInFile(string file) {
//
//     std::ofstream out(file.c_str());
//     out<<getString2();
//    out.close();
//}
//
//
//double CMatrix::getDeterminant3(){
//
//    if(nR!=nC)
//    throw("Invalid matrix dimension");
//    if(nR==1) return(values[0][0]);
//    if(nR==2) return (values[0][0]*values[1][1]-values[0][1]*values[1][0]);
//    if (nR != nC)
//        throw("Invalid matrix dimension");
//   // CMatrix L(nR,nC);
//    CMatrix U(*this);
//    fixMatrix(U ,  -1,-1);
//   // double** l = L.values;
//    double** u = U.values;
//    double factor =0;
//   int i = 0, j = 0, k = 0;
//  double ans =1;
// if(checkIfZeroMatrix(U))return 0;
//
//  if(u[0][0]==0){
//    if(fixMatrix(U,0,0))
//       ans*=-1;
//  }
// if(u[0][0]!=1){
//    ans *= u[0][0];
//    double temp = u[0][0];
//    for(i=0; i<nC ; i++){
//        u[0][i]/=temp;
//    }
//
//
//
//  }
//
//
//
//
//
//   for(i= 0 ; i<nR;i++){
//
//         for(j = 0; j<i;j++){
//        if(u[j][j]==0){
//                CMatrix::fixMatrix(U,j,j)   ;
//                ans*=-1;
//                i-=2;
//
//                if(i<0) i=0;
//
//
//                break;
//              }
//        factor = u[i][j]/u[j][j];
//        for(k=0;k<nR;k++) {
//            if(factor==0)break;
//
//            double x = u[i][k]- (factor * u[j][k]);
//
//            if(x<0.000000000000001 && x>-0.00000000000001) x =0;
//
//            u[i][k]=x;
//
//            }
//
//    }
//
//
//
//   }
//
//
//
//   for(int i=0;i<nR; i++){
//        ans*=u[i][i];
//        if(ans==0) return ans;
//    }
//    return ans;
//
///*
//    for (i = 0; i < nR; i++)
//    {
//        for (j = 0; j < nR; j++)
//        {
//            if (j < i)
//                l[j][i] = 0;
//            else
//            {
//                l[j][i] = values[j][i];
//                for (k = 0; k < i; k++)
//                {
//                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
//                }
//            }
//        }
//        for (j = 0; j < nR; j++)
//        {
//            if (j < i)
//                u[i][j] = 0;
//            else if (j == i)
//                u[i][j] = 1;
//            else
//            {
//                u[i][j] = values[i][j] / l[i][i];
//                for (k = 0; k < i; k++)
//                {
//                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
//                }
//            }
//        }
//    }
//
//    double ans =1;
//    cout<<U<<endl <<endl << L;
//    for(int i=0;i<nR; i++){
//        ans*=l[i][i];
//        if(ans==0) return ans;
//    }
//    return ans;
//    */
//}
//
//bool  CMatrix:: fixMatrix(CMatrix &m , int r,int c) {
//
//    int index =0;
//    static int fR=-1;
//    static int fC=-1;
//    static int fnR = 0;
//    if(r == -1 && c == -1){
//         fR=-1;
//         fC=-1;
//
//        return true;
//    }
//    if(r == fR && fC==c){
//        fnR-=1;
//    }
//    else {
//        fR = r ;
//        fC = c ;
//        fnR = m.nR;
//    }
//
//    for(int i = fnR-1; i>-1;i--){
//
//        if(i==r) continue;
//        if(m.values[r][i]!=0){
//                index = i;
//
//
//                break;
//        }
//    }
//
//   double temp;
//    for(int j = 0 ; j<m.nC ; j++){
//                temp = m.values[j][c];
//                m.values[j][c] = m.values[j][index];
//                m.values[j][index] = temp;
//    }
//    return true;
//}
//
//
//
//bool  CMatrix:: checkIfZeroMatrix(CMatrix &m ){
//
//    bool zeroMatrix = false ;
//    for(int i = 0 ; i <m.nR ; i++){
//       for(int j = i+1; j<m.nR; j++ ){
//            if ((m.values[i][0]!=0 && m.values[j][0]==0)
//                ||(m.values[i][0]==0 && m.values[j][0]!=0)) continue;
//            double factor = m.values[i][0]/m.values[j][0];
//            for(int k =1 ; k<m.nC ; k++){
//                zeroMatrix = true;
//                double temp = m.values[i][k]/m.values[j][k];
//                if(!( (temp - factor) <0.000000001 && (temp-factor)> - 0.000000001)){
//                    zeroMatrix = false;
//                    break;
//                }
//            }
//            if(zeroMatrix) return zeroMatrix;
//
//
//       }
//    }
//    for(int i = 0 ; i <m.nR ; i++){
//       for(int j = i+1; j<m.nR; j++ ){
//            if ((m.values[i][0]!=0 && m.values[j][0]==0)
//                ||(m.values[i][0]==0 && m.values[j][0]!=0)) continue;
//            double factor = m.values[0][i]/m.values[0][j];
//            for(int k =1 ; k<m.nC ; k++){
//                zeroMatrix = true;
//                double temp = m.values[k][i]/m.values[k][j];
//                if(!( (temp - factor) <0.000000001 && (temp-factor)> - 0.000000001)){
//                    zeroMatrix = false;
//                    break;
//                }
//            }
//            if(zeroMatrix) return zeroMatrix;
//
//
//       }
//    }
//
//return zeroMatrix;
//}
//
//
///*
//*   function to getDeterminant (Intel research Paper)
//*
//*/
//double CMatrix::getDeterminant(){
//
//    int *ri = new int[nR];
//    int i, j, k;
//
//    double    det = 1.0;
//    CMatrix M(*this);
//    double ** m = M.values;
//// Initialize the pointer vector.
//    for (i = 0 ; i < nR; i++)
//        ri[i] = i;
//
//    for (int p = 1 ; p <= nR - 1; p++) {
//        // Find pivot element
//        for (i = p + 1 ; i <= nR; i++) {
//            if (abs(m[ri[i-1]][p-1]) > abs(m[ri[p-1]][p-1])) {
//                // Switch the index for the p-1 pivot row if necessary.
//                int t = ri[p-1];
//                ri[p-1] = ri[i-1];
//                ri[i-1] = t;
//                det = -det;
//            }
//        }
//        if (m[ri[p-1]][p-1] == 0) {
//            // The matrix is singular.
//                    return false;
//            }
//        // Multiply the diagonal elements.
//        det = det * m[ri[p-1]][p-1];
//
//        // Form multiplier.
//        for (i = p + 1 ; i <= nR; i++) {
//            m[ri[i-1]][p-1] /= m[ri[p-1]][p-1];
//            // Eliminate [p-1].
//            for (int j = p + 1 ; j <= nR; j++)
//                m[ri[i-1]][j-1] -= m[ri[i-1]][p-1] * m[ri[p-1]][j-1];
//        }
//    }
//    det = det * m[ri[nR-1]][nR-1];
//
//
//    return det;
//}
//
