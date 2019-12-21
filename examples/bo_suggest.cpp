#include <iostream>
#include <bayesopt/bayesopt.hpp>
#include <bayesopt/parameters.hpp>   // c++ parameters classes

using namespace std;

namespace bayesopt {
    class ContCase: public ContinuousModel
    {
        public:
            ContCase(size_t dim, Parameters par):
                ContinuousModel(dim, par) {}

            double evaluateSample( const vectord& xin)
            {
                double x = xin(0);
                double y = xin(1);
                
                return (x - 0.35) * (x - 0.35) + (y - 0.8) * (y - 0.8);
            }

            bool checkReachability(const vectord &query)
            {return true;};
    };
}  //namespace bayesopt


int main() {
    bayesopt::Parameters par;
    par = initialize_parameters_to_default();
    par.n_init_samples = 10;
    par.n_iterations = 10;
    par.n_inner_iterations = 0;
    par.random_seed = 0;
    par.verbose_level = 1;
    par.noise = 1e-10;
    par.force_jump = 0;

    bayesopt::ContCase mycase(2, par);
    vectord result(2);
    vectord lb(2); lb(0) = 0; lb(1) = 0;
    vectord ub(2); ub(0) = 1; ub(1) = 1;
    mycase.setBoundingBox(lb,ub);

    mycase.initializeOptimization();
    vectord xNext(2);
    for (size_t ii = mycase.getCurrentIter(); ii < par.n_iterations; ++ii) {
        // stepOptimization in old style
        // mycase.stepOptimization();

        // suggest-evaluate-registerPoint style
        xNext = mycase.suggest();
        double yNext = mycase.evaluateSample(xNext);
        mycase.registerPoint(xNext, yNext);
    }
   
    result = mycase.getFinalResult();
    std::cout << "Result: " << result << "->" 
            << mycase.evaluateSample(result) << std::endl;

    return 0;
}