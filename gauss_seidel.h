/*
 * gauss_seidel.h
 *
 *  Created on: 2019-05-03
 *      Author: mbreit
 */

#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H


#include "preconditioner_interface.h"


template <typename TMatrix>
class GaussSeidel
        : public IPreconditioner<TMatrix>
{
private:

    TMatrix Inverse;

public:
    typedef Vector vector_type;
    typedef TMatrix matrix_type;

public:
    /// constructor
    GaussSeidel();

    /// @copydoc IPreconditioner::init
    virtual bool init(const vector_type& x);

    /// @copydoc IPreconditioner::apply
    virtual bool apply(vector_type& c, const vector_type& d) const;
};

#endif // GAUSS_SEIDEL_H