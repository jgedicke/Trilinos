/*
// @HEADER
// ***********************************************************************
//
//          Tpetra: Templated Linear Algebra Services Package
//                 Copyright (2008) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
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
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
// @HEADER
*/

// Including this is the easy way to get access to all the Node types.
#include <Kokkos_DefaultNode.hpp>
#include <Tpetra_ConfigDefs.hpp>

#if defined(HAVE_TPETRA_EXPLICIT_INSTANTIATION) && defined(HAVE_TPETRACORE_TEUCHOSKOKKOSCOMPAT) && defined(HAVE_TPETRA_INST_SERIAL) && defined(TPETRA_HAVE_KOKKOS_REFACTOR) && defined(HAVE_TPETRA_INST_INT_UNSIGNED)

#include "Tpetra_CrsMatrix_decl.hpp"
#include "TpetraCore_ETIHelperMacros.h"
#include "Tpetra_CrsMatrix_def.hpp"

namespace Tpetra {

 TPETRA_ETI_MANGLING_TYPEDEFS()

#define TPETRA_CRSMATRIX_UNSIGNED_SERIALWRAPPERNODE_INSTANT( SCALAR, LO ) \
  TPETRA_CRSMATRIX_INSTANT( SCALAR, LO, unsigned, Kokkos::Compat::KokkosSerialWrapperNode )

 TPETRA_INSTANTIATE_SL(TPETRA_CRSMATRIX_UNSIGNED_SERIALWRAPPERNODE_INSTANT)

 // convert() gets instantiated in a separate file, Tpetra_CrsMatrix_convert.cpp

} // namespace Tpetra

#endif