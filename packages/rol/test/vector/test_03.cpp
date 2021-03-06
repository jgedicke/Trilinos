// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact lead developers:
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER

/*! \file  test_03.cpp
    \brief Test MultiVector interface.

*/

#include "ROL_MultiVectorDefault.hpp"
#include "ROL_StdVector.hpp"
#include "ROL_Types.hpp"

#include "Teuchos_oblackholestream.hpp"
#include "Teuchos_GlobalMPISession.hpp"

#include <iomanip>
#include <iostream>


typedef double RealT;

using namespace ROL;
using Teuchos::RCP;
using Teuchos::rcp;
using Teuchos::ArrayRCP;


template<class Real>
Real norm_sum(const MultiVector<Real> &A) {
    int numVectors = A.getNumberOfVectors();
    std::vector<RealT> norms(numVectors);
    A.norms(norms);
    Real sum = 0;
    for(int i=0;i<numVectors;++i) {
        sum += norms[i]; 
    }
    return sum;
}


int main(int argc, char *argv[]) {

    Teuchos::GlobalMPISession mpiSession(&argc,&argv);

    int iprint     = argc - 1;
    Teuchos::RCP<std::ostream> outStream;
    Teuchos::oblackholestream bhs; // outputs nothing
    if (iprint > 0)
        outStream = Teuchos::rcp(&std::cout, false);
    else
        outStream = Teuchos::rcp(&bhs, false);

    int errorFlag = 0;

    try { 

	Teuchos::SerialDenseMatrix<int,RealT> M(2,2,true);
	M(0,0) = 2.0;
	M(0,1) = -1.0;
	M(1,0) = -2.0;
	M(1,1) = 3.0;


	Teuchos::RCP<std::vector<RealT> > w_rcp = Teuchos::rcp(new std::vector<RealT>(2));
	Teuchos::RCP<std::vector<RealT> > x_rcp = Teuchos::rcp(new std::vector<RealT>(2));
	Teuchos::RCP<std::vector<RealT> > y_rcp = Teuchos::rcp(new std::vector<RealT>(2));
	Teuchos::RCP<std::vector<RealT> > z_rcp = Teuchos::rcp(new std::vector<RealT>(2));

	(*w_rcp)[0] = 0.0;
	(*w_rcp)[1] = 1.0;

	(*x_rcp)[0] = 1.0;
	(*x_rcp)[1] = 0.0;

	(*y_rcp)[0] = 3.0;
	(*y_rcp)[1] = 4.0;

	(*z_rcp)[0] = -1.0;
	(*z_rcp)[1] =  1.0;
       
	RCP<Vector<RealT> > w = rcp(new StdVector<RealT>(w_rcp)); 
	RCP<Vector<RealT> > x = rcp(new StdVector<RealT>(x_rcp)); 
	RCP<Vector<RealT> > y = rcp(new StdVector<RealT>(y_rcp)); 
	RCP<Vector<RealT> > z = rcp(new StdVector<RealT>(z_rcp)); 

	ArrayRCP<RCP<Vector<RealT> > > A_rcp(2);
	ArrayRCP<RCP<Vector<RealT> > > B_rcp(2);

	A_rcp[0] = x;     
	A_rcp[1] = y;     

	B_rcp[0] = w;     
	B_rcp[1] = z;     

	RCP<MultiVector<RealT> > A = rcp(new MultiVectorDefault<RealT>(A_rcp));
	RCP<MultiVector<RealT> > B = rcp(new MultiVectorDefault<RealT>(B_rcp));
       
	// Test norm
	if(static_cast<int>(norm_sum(*A)) != 6) {
            *outStream << "Norm test failed!\n";
	    ++errorFlag;
	}

	// Test clone
	RCP<MultiVector<RealT> > C = A->clone();    
	if(norm_sum(*C) != 0) {
            *outStream << "Clone test failed!\n";
	    ++errorFlag;
	}

	// Test deep copy
        RCP<MultiVector<RealT> > D = A->deepCopy();
	if(static_cast<int>(norm_sum(*D)) != 6) {
            *outStream << "Deep copy test failed!\n";
	    ++errorFlag;
	}
        
        // Test shallow copy
	std::vector<int> index(1);
	index[0] = 0;

        RCP<MultiVector<RealT> > S = A->shallowCopy(index);
	if(static_cast<int>(norm_sum(*S)) != 1) {
            *outStream << "Shallow copy test failed!\n";
	    ++errorFlag;
	}

        // Test scaling
	std::vector<RealT> alpha(2);
	alpha[0] = 4.0;
	alpha[1] = 9.0;
	A->scale(alpha);
	if(static_cast<int>(norm_sum(*A)) != 49) {
            *outStream << "Scaling test failed!\n";
	    ++errorFlag;
	}

	// Test matrix multiplication 
	A->gemm(2.0,*B,M,1.0);
	if(static_cast<int>(norm_sum(*A)) != 53) {
            *outStream << "Matmat multiply test failed!  The norm_sum is " << static_cast<int>(norm_sum(*A)) << ", not equal to 49.\n";
	    ++errorFlag;
	}

	// Test set
        A->set(*B);
	if(static_cast<int>(norm_sum(*A)) != 2) {
            *outStream << "Set test failed!\n";
	    ++errorFlag;
	}

	// Test innerProducts
        Teuchos::SerialDenseMatrix<int,RealT> P(2,2,true);
        B->innerProducts(1.0,*B,P);
        Teuchos::SerialDenseMatrix<int,RealT> Check(2,2,true);
        Check(0,0) = 1.0;   
        Check(0,1) = 1.0;   
        Check(1,0) = 1.0;   
        Check(1,1) = 2.0;   
        if( P != Check ) {
            *outStream << "Inner product test failed!\n";
	    ++errorFlag;
        }

	// Test dot products
        std::vector<RealT> dots(2); 
        D->dots(*D,dots);
        if(static_cast<int>(dots[0]) != 1 || 
           static_cast<int>(dots[1]) != 25 ) {
            *outStream << "Dot product test failed!\n";
            ++errorFlag;
        }

//    StdVector<RealT> d0 = Teuchos::dyn_cast<StdVector<RealT>>(*D->getVector(0));
//    StdVector<RealT> d1 = Teuchos::dyn_cast<StdVector<RealT>>(*D->getVector(1));

//    std::cout << (*d0.getVector())[0] << std::endl;
//    std::cout << (*d0.getVector())[1] << std::endl;
//    std::cout << (*d1.getVector())[0] << std::endl;
//    std::cout << (*d1.getVector())[1] << std::endl;

    }
    catch(std::logic_error err) {
        *outStream << err.what() << "\n";
        errorFlag = -1000;
    }; // end try

    if (errorFlag != 0) {
        std::cout << "End Result: TEST FAILED\n";
    }
    else {
        std::cout << "End Result: TEST PASSED\n";
    }

    return 0;
}

