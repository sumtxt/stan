#ifndef __STAN__MCMC__BASE__LEAPFROG__BETA__
#define __STAN__MCMC__BASE__LEAPFROG__BETA__

#include <iostream>
#include <iomanip>
#include <stan/mcmc/hmc/integrators/base_integrator.hpp>

namespace stan {
  
  namespace mcmc {
    
    template <typename H, typename P>
    class base_leapfrog: public base_integrator<H, P> {
      
    public:
      
      void evolve(P& z, H& hamiltonian, const double epsilon) {
        
        begin_update_p(z, hamiltonian, 0.5 * epsilon);
        
        update_q(z, hamiltonian, epsilon);
        hamiltonian.update(z);
        
        end_update_p(z, hamiltonian, 0.5 * epsilon);
        
      }
      
      void verbose_evolve(P& z, H& hamiltonian, const double epsilon, std::ostream* error_stream = 0) {
        
        std::cout.precision(15);
        int width = 20;
        int nColumn = 4;
        
        std::cout << "Verbose Hamiltonian Evolution, Step Size = " << epsilon << ":" << std::endl;
        std::cout << "    " << std::setw(nColumn * width) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
        std::cout << "    "
                  << std::setw(width) << std::left << "Poisson"
                  << std::setw(width) << std::left << "Initial" 
                  << std::setw(width) << std::left << "Current"
                  << std::setw(width) << std::left << "DeltaH"
                  << std::endl;
        std::cout << "    "
                  << std::setw(width) << std::left << "Operator"
                  << std::setw(width) << std::left << "Hamiltonian" 
                  << std::setw(width) << std::left << "Hamiltonian"
                  << std::setw(width) << std::left << "/ Stepsize^{2}"
                  << std::endl;
        std::cout << "    " << std::setw(nColumn * width) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
        
        double H0 = hamiltonian.H(z);
        
        begin_update_p(z, hamiltonian, 0.5 * epsilon);
        
        double H1 = hamiltonian.H(z);
        
        std::cout << "    "
                  << std::setw(width) << std::left << "hat{V}/2"
                  << std::setw(width) << std::left << H0
                  << std::setw(width) << std::left << H1
                  << std::setw(width) << std::left << (H1 - H0) / (epsilon * epsilon)
                  << std::endl;
        
        update_q(z, hamiltonian, epsilon);
        //hamiltonian.update(z, error_stream);
        hamiltonian.update(z);
        
        double H2 = hamiltonian.H(z);
        
        std::cout << "    "
                  << std::setw(width) << std::left << "hat{T}"
                  << std::setw(width) << std::left << H0
                  << std::setw(width) << std::left << H2
                  << std::setw(width) << std::left << (H2 - H0) / (epsilon * epsilon)
                  << std::endl;
        
        end_update_p(z, hamiltonian, 0.5 * epsilon);
        
        double H3 = hamiltonian.H(z);
        
        std::cout << "    "
                  << std::setw(width) << std::left << "hat{V}/2"
                  << std::setw(width) << std::left << H0
                  << std::setw(width) << std::left << H3
                  << std::setw(width) << std::left << (H3 - H0) / (epsilon * epsilon)
                  << std::endl;
        
        std::cout << "    " << std::setw(nColumn * width) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
        
      }
      
      virtual void begin_update_p(P& z, H& hamiltonian, double epsilon) = 0;
      virtual void update_q(P& z, H& hamiltonian, double epsilon) = 0;
      virtual void end_update_p(P& z, H& hamiltonian, double epsilon) = 0;
      
    };
    
  } // mcmc
  
} // stan


#endif
