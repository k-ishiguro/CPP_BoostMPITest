/**
 * Written by Katsuhiko Ishiguro <k.ishiguro.jp@ieee.org>
 * Last Update: 02/09, 2016 (dd/mm, yyyy)
*/

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>

namespace bmpi = boost::mpi;

int main(int argc, char **argv){
  bmpi::environment env(argc, argv);

  bmpi::communicator world;
  std::cout << "My rank =" << world.rank()
	    << " of " << world.size() << std::endl;

}
