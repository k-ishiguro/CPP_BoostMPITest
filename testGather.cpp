/**
 * Written by Katsuhiko Ishiguro <k.ishiguro.jp@ieee.org>
 * Last Update: 02/09, 2016 (dd/mm, yyyy)
*/

#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <cstdlib>

namespace bmpi = boost::mpi;

int main(int argc, char **argv){

  bmpi::environment env(argc, argv);
  bmpi::communicator world;

  std::vector<double> x, y;

  if(world.rank() == 0){ // maseter routine
    std::srand(std::time(0));
    x.resize(world.size());
    y.resize(world.size());

    auto f = []() {return double(std::rand()) / RAND_MAX;};

    std::generate(x.begin(), x.end(), f);
    std::generate(y.begin(), y.end(), f);
    
    for(int i = 0;i < x.size(); ++i) {
      std::cout << "Original x : " << x[i] << std::endl;
    };

    for(int i = 0;i < y.size(); ++i) {
      std::cout << "Original y : " << y[i] << std::endl;
    };
    
  } 
  
  double x_value, y_value;

  /** 
      this done once in the world
      split the vectors x(y) and 
      send a double to each thread's x_value(y_value) 
  */
  bmpi::scatter(world, x, x_value, 0);
  bmpi::scatter(world, y, y_value, 0);

  /* this is a thread-wise computaion */
  const double result = x_value + y_value;
  std::cout << "Rank " << world.rank() << ", result=" << result << std::endl;
  
  /**
     This done once in the world. 
     collect the result for threads, and store it in thared 0's x.
   */
  bmpi::gather(world, result, x, 0);

  if(world.rank() == 0){
    std::cout << "gathered result. " << std::endl;
    std::for_each(x.begin(), x.end(), 
		  [](double i){ std::cout << i << std::endl; });
  }
  
}
