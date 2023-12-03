/*
 * jacobi.h
 *
 *  Created on: 2019-05-03
 *      Author: mbreit
 */

#ifndef JACOBI_H
#define JACOBI_H


#include "preconditioner_interface.h"


template <typename TMatrix>
class Jacobi
: public IPreconditioner<TMatrix>
{
	public:
		typedef Vector vector_type;
		typedef TMatrix matrix_type;

	public:
		/// constructor
		Jacobi();

		/// @copydoc IPreconditioner::init
		virtual bool init(const vector_type& x);

		/// @copydoc IPreconditioner::apply
		virtual bool apply(vector_type& c, const vector_type& d) const;
    
        /// sets the damping factor
        void set_damping(double damp) {m_damp = damp;}
        
    protected:
    	double m_damp;
};

#endif // JACOBI_H
