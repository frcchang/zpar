#ifndef DBN_H
#define DBN_H
#include "bitarray.h"
#include <cstdlib>
#include <ctime>
class RBM
{private:
 int v_size;
 int h_size;

 public:
 double **W;
 double **Q;     // Q is a 2 D matrix having it's column vectors as Q(h_0 = 1|v0) and Q(h1 = 1|v1), where Q is conditional probability
 double *P;      // P is Probability P(v1 = 1|h0)
 double **v_samples;
 double *v_bias;
 double **h_samples;
 double *h_bias;
 inline double uniform(double min,double max) {
    return rand()/(RAND_MAX + 1.0) * (max - min) + min;
 }
 inline double sigm(double x) {
    return 1.0/(1.0+exp(-x));
 }
 inline double binomial(int n,double p){
  double c=0;
  if(p < 0 || p > 1) return 0;
  double r;

  for(int i=0;i<n;i++){
     r=rand() / (RAND_MAX + 1.0);
     if(r < p) c++;
     }
  return c;
 }
 //Methods / Functions
 public:
 RBM(int v_size=0,int h_size=0);
 const int &gethsize() const { return h_size; }
 const int &getvsize() const { return v_size;} 
 void manual_initialize();
 void random_initialize();
 friend std::istream& operator>>(std::istream&,RBM&);
 friend std::ostream& operator<<(std::ostream&,RBM&);
 void update(int,int N=1,double learning_rate=0.1);
 //void propup(int);
 //void propdown(int);
 //void sample_h_given_v(int);
 //void sample_v_given_h(int);
 void train_RBM();
 void test_RBM();
 void predict_RBM();
 void print_parameters();
 void initialize(int,int);
 void finalize();
 void update(const CBitArray &,int,int N=1);
 void train(const std::string &);
 inline void propup(int k) {
    int i,j;
    double count=0;

            //Calculate Q[k]
            for(i=0;i<h_size;i++)
                {   count=0;
                    for(j=0;j<v_size;j++)
                    {count = count + W[i][j]*v_samples[j][k];}

                    Q[i][k]=sigm(h_bias[i]+count);


                }
 }
 inline void propdown(int k) {
    int i,j;
    double count=0;
    //Calculate P
            for(j=0;j<v_size;j++)
                {   count=0;
                    for(i=0;i<h_size;i++)
                    {count = count + W[i][j]*h_samples[i][k];}

                    P[j]=sigm(v_bias[j]+count);
                }
 }
 inline void sample_h_given_v(int k) {
            propup(k);
            //sample H[k]
            for(int i=0;i<h_size;i++)
            {
                h_samples[i][k]=binomial(1,Q[i][k]);
            }
 }
 inline void sample_v_given_h(int k) {
            propdown(k);
            //sample V[1]
            for(int i=0;i<v_size;i++)

            {v_samples[i][1]=binomial(1,P[i]); }
 }

 ~RBM();
};
class DBN
{public:
int n_inputs;
int *hidden_layer_sizes;
int n_outputs;
int n_layers;
RBM *rbm_layers;
//double **delta;
public:
DBN(int n_inputs=0,int *hidden_layer_size=0,int n_outputs=0,int n_layers=0);
friend std::istream& operator>>(std::istream&,DBN&);
friend std::ostream& operator<<(std::ostream&,DBN&);
void initialize(int);
void finalize();
void pretrain(double ***,double,int,int N=1);
void finetune(double *,double *,double,int,int N=1);
//void softmax(double *,int);
void logistic_regression_train(double,int,int);
//void predict(double *);
void forward_propagation(double *);
void forward_propagation(const CBitArray &v);
void forward_propagation();
void print_trained_parameters();
void train(const std::string &);
void getsamples(const int &,CBitArray &);
int getnlayers() const;
~DBN();
};



#endif
