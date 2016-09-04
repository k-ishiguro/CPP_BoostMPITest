/**
 * Written by Katsuhiko Ishiguro <k.ishiguro.jp@ieee.org>
 * Last Update: 02/09, 2016 (dd/mm, yyyy)
*/


#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/serialization/string.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <ctime>

namespace bmpi = boost::mpi;

int main(int argc, char **argv){

  bmpi::environment env(argc, argv);
  bmpi::communicator world;

  const int N = 8;
  assert(N % world.size() == 0);
  std::vector<double> v, rv;

  if(world.rank() == 0){ // maseter routine
    v.resize(N); 
    rv.resize(N);
    std::srand(std::time(0));
    std::generate(v.begin(), v.end(), [] () {
	return double(std::rand()) / RAND_MAX;}); // v initialized

    std::for_each(v.begin(), v.end(), [&](double i) {
	std::cout << "Original v : " << world.rank() << ", " << i << std::endl;
      });

  } else { // remaining threads
    rv.resize(N / world.size()); // so, slaves have different v. 
  }

  /* partition and ditrubute the value of V (of master) to all threads*/
  bmpi::scatter(world, v, rv.data(), N / world.size(), 0); 
  // v: pointer to the data to send
  // rv.data(): pointer where the data recieved
  
  if(world.rank() == 0){
    rv.resize(N / world.size()); // now master has the same size
  }

  std::for_each(std::begin(rv), std::end(rv), [&](double i) {
      std::cout << "Rank : " << world.rank() << ", " << i << std::endl;
    });

}
