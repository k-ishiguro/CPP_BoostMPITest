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

  const int N = 128;
  std::vector<float> v(N, 0.0);
  std::string s;

  if(world.rank() == 0){ // maseter routine
    std::srand(std::time(0));
    std::generate(v.begin(), v.end(), [] () {
	return double(std::rand()) / RAND_MAX;}); // v initialized
    
    for(int i = 0;i < 10; ++i) {
      std::cout << "Original v : " << v[i] << std::endl;
    };

    s = "Hello world";
    
  } 
  
  /* copy the same data to all threads*/
  bmpi::broadcast(world, s, 0); 
  bmpi::broadcast(world, v, 0); 
  

  std::cout << "Rank : " << world.rank() << ", s=" << s << ", v[0]=" << v[0] << std::endl;  
  
}
