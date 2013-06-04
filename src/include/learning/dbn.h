#ifndef DBN_H
#define DBN_H
#include "bitarray.h"
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

 //Methods / Functions
 public:
 RBM(int v_size=0,int h_size=0);
 const int &gethsize() const { return h_size; }
 const int &getvsize() const { return v_size; }
 void manual_initialize();
 void random_initialize();
 friend std::istream& operator>>(std::istream&,RBM&);
 friend std::ostream& operator<<(std::ostream&,RBM&);
 void update(int,int N=1,double learning_rate=0.1);
 void propup(int);
 void propdown(int);
 void sample_h_given_v(int);
 void sample_v_given_h(int);
 void train_RBM();
 void test_RBM();
 void predict_RBM();
 void print_parameters();
 void initialize(int,int);
 void finalize();
 void update(const CBitArray &,int,int N=1);
 void train(const std::string &);
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
void pretrain(double *,double,int,int N=1);
void finetune(double *,double *,double,int,int N=1);
//void softmax(double *,int);
void logistic_regression_train(double,int,int);
//void predict(double *);
void forward_propagation(double *);
void forward_propagation(const CBitArray &v);
void forward_propagation();
void print_trained_parameters();
void train(const std::string &);
void gethsamples(const int &,CBitArray &);
int getnlayers() const;
~DBN();
};
#endif
