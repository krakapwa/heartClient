#include <firfilt.h>

#include <QDebug>

FirFilt::FirFilt(double* a_, int n_){
   n = n_;

   for(int i=0; i<n; ++i) {
        a.push_back( a_[i]);
   }
        qDebug() << "hi";



   for(int i=0; i<n; ++i) {
      qDebug()<< "a[" << i << "] = " << a[i];
   }
      qDebug()<< "n = " << n;

}

double FirFilt::applyFilt(boost::circular_buffer<int> s){

    double y = 0;
    int N = s.capacity();
    //qDebug() << N;
    /*
    qDebug() << n;
    */
    if(N<n){
       qDebug() << "applyFilt: input signal must be larger than filter order";
       return 0;
    }

    if(s.full()){
        for(int i = 0;i<n-1;++i){
            //qDebug() << s[N-i];
            //qDebug() << i;
           y += a[i]*s[N-1-i];
        }
        return y;
    }else{
        return 0;
    }

}
