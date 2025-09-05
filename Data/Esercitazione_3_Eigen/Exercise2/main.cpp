#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

int main()
{
    /*Creo le matrici dei sistemi lineari a partire da dei vettori di coefficienti 
    e poi utilizzo il metodo reshaped, che lavora per colonne (poi traspongo per riempire la matrice per righe )*/

    Vector4d v1(5.547001962252291e-01, -3.770900990025203e-02, 8.320502943378437e-01, -9.992887623566787e-01);
    Vector4d v2(5.547001962252291e-01, -5.540607316466765e-01, 8.320502943378437e-01, -8.324762492991313e-01);
    Vector4d v3(5.547001962252291e-01, -5.547001955851905e-01, 8.320502943378437e-01, -8.320502947645361e-01);

    Matrix2d A1=v1.reshaped(2, 2).transpose();
    Matrix2d A2=v2.reshaped(2, 2).transpose();
    Matrix2d A3=v3.reshaped(2, 2).transpose();

    /*Costruisco i vettori dei termini noti sommando le righe delle rispettive matrici, 
    con un meno davanti perchè la soluzione deve esssere -(1,1)*/
    Vector2d b1=-A1.rowwise().sum();
    Vector2d b2=-A2.rowwise().sum();
    Vector2d b3=-A3.rowwise().sum();

     /*Faccio la decomposizione PA=LU, con la classe template offerta dalla libreria Eigen 
    per matrici non singolari, ed uso il metodo solve per risolvere il sistema con questa fattorizzazione*/
    FullPivLU<Matrix2d> lu1(A1);
    FullPivLU<Matrix2d> lu2(A2);
    FullPivLU<Matrix2d> lu3(A3);

    if(lu1.rank()<2) cerr<<"Error: the matrix A1 is singular"<<endl;    
    else cout<<"The LU decomposition for A1 is well defined"<<endl;
    
    if(lu2.rank()<2) cerr<<"Error: the matrix A2 is singular"<<endl;
    else cout<<"The LU decomposition for A2 is well defined"<<endl;
    
    if(lu3.rank()<2) cerr<<"Error: the matrix A3 is singular"<<endl; 
    else cout<<"The LU decomposition for A3 is well defined"<<endl;

    Vector2d x1_lu=lu1.solve(b1);
    Vector2d x2_lu=lu2.solve(b2);
    Vector2d x3_lu=lu3.solve(b3);
    
    /*Faccio la decomposizone A=QR* e calcolo la soluzione del sistema con il metodo solve*/
    HouseholderQR<Matrix2d> qr1(A1); 
    HouseholderQR<Matrix2d> qr2(A2);
    HouseholderQR<Matrix2d> qr3(A3); 
    Vector2d x1_qr=qr1.solve(b1);
    Vector2d x2_qr=qr2.solve(b2);
    Vector2d x3_qr=qr3.solve(b3);


    /*Calcolo gli errori relativi dei sistemi.
    Per farlo uso il metodo norm definito sulla classe Vector, che permette di calcolare la norma euclidea dei vettori*/
    Vector2d x_ex=-Vector2d::Ones();
    double err1_lu=(x1_lu-x_ex).norm()/x_ex.norm();
    double err1_qr=(x1_qr-x_ex).norm()/x_ex.norm();
    double err2_lu=(x2_lu-x_ex).norm()/x_ex.norm();
    double err2_qr=(x2_qr-x_ex).norm()/x_ex.norm();
    double err3_lu=(x3_lu-x_ex).norm()/x_ex.norm();
    double err3_qr=(x3_qr-x_ex).norm()/x_ex.norm();


    /*Output esemplificativo per vedere che tutto funzioni*/
    cout<<endl;
    cout<<"1.1)LU decomposition x1_LU = ("<<x1_lu.transpose()<<")"<<" err1_LU = "<<err1_lu<<endl;
    cout<<"1.2)QR decomposition x1_QR = ("<<x1_qr.transpose()<<")"<<" err1_QR = "<<err1_qr<<endl;
    cout<<endl;
    cout<<"2.1)LU decomposition x2_LU = ("<<x2_lu.transpose()<<")"<<" err2_LU = "<<err2_lu<<endl;
    cout<<"2.2)QR decomposition x2_QR = ("<<x2_qr.transpose()<<")"<<" err2_QR = "<<err2_qr<<endl;
    cout<<endl;
    cout<<"3.1)LU decomposition x3_LU = ("<<x3_lu.transpose()<<")"<<" err3_LU = "<<err3_lu<<endl;
    cout<<"3.2)QR decomposition x3_QR = ("<<x3_qr.transpose()<<")"<<" err3_QR = "<<err3_qr<<endl;

    return 0;
}