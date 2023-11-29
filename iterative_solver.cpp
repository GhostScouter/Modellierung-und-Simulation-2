/*
 * Iterative_solver.cpp
 *
 *  Created on: 2019-05-03
 *      Author: 
 */

#include "iterative_solver.h"

template <typename TMatrix>
IterativeSolver<TMatrix>::IterativeSolver(const matrix_type& mat)
{
	//matrix_type* m_pA = mat;
    m_pA = &mat;
    m_corrector = nullptr;
    m_nit = 1;
    m_minDef = 1e-50;
    m_minRed = 1e-50;
    m_bVerbose = false;
    m_bInited = false;
}


template <typename TMatrix>
IterativeSolver<TMatrix>::IterativeSolver()
{
    m_corrector = nullptr;
    m_nit = 1;
    m_minDef = 1e-50;
    m_minRed = 1e-50;
    m_bVerbose = false;
    m_pA = nullptr;
    m_bInited = false;
}


template <typename TMatrix>
void IterativeSolver<TMatrix>::set_corrector(corrector_type* stepMethod)
{
	m_corrector = stepMethod;
    m_corrector->set_matrix(this->m_pA);
}


template <typename TMatrix>
bool IterativeSolver<TMatrix>::init(const vector_type& x)
{
	if(m_corrector){m_corrector->init(x);}
}


template <typename TMatrix>
void IterativeSolver<TMatrix>::set_matrix(const matrix_type* A)
{
	m_pA = A;
    if(m_corrector){m_corrector->set_matrix(A);}
}


template <typename TMatrix>
void IterativeSolver<TMatrix>::set_convergence_params
(
	std::size_t nIter,
	double minDef,
	double minRed
)
{
	m_nit = nIter;
    m_minDef = minDef;
    m_minRed = minRed;
}


template <typename TMatrix>
bool IterativeSolver<TMatrix>::solve(vector_type& x, const vector_type& b) const{

	//defekt d berechnen (Vektor)
    Vector defect_vector = Vector(x.size());            // speichert d ab
    defect_vector = m_pA.operator*(x);                  // M*x
    defect_vector.operator-=(b);                        // - b
    // d = Ax - b abgeschlossen


    //korrekturvektor erstellen
    Vector correction_vector = Vector(x.size());   	    // will hold improvements calculated by preconditioner
	
    //initiale defektnorm (skalar) zum Vergleich in der iteration behalten
    double def0 = defect_vector.norm();

    //defekt zum Update in der iteration
    double def = def0;

    //defekt zum vergleich zw lezter und atkueller iteration
    double def_last = std::numeric_limits<int>::max();           // im ersten Lauf soll "def < def_last" True ergeben

    size_t counter = 0;                                          // zählt Iterationen

    while(counter < m_nit && def > m_minDef && def > def0 * m_minRed){
        // counter < m_nit UND Fehler groß genug UND relative Verbesserung groß genug

        counter += 1;

        //korrektor ausführen (jacobi, gauss seidel etc.)
        correction_vector->apply(correction_vector, defect_vector);
        // FRAGE: Funktioniert so die Übergabe korrekt? correction_vector soll mit Inhalten c[i] gefüllt werden!

        //Korrekturschritt für unseren lösungsvektor x
        x.operator-=(correction_vector);

        //defekt updaten
        def_last = def;                         // speichere Fehler von letzter Iteration als def_last ab 
        defect_vector -= m_pA.operator*(c);     // neuer def_vektor STOPP C EXISTIERT NICHT^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        def = defect_vector.norm();             // norm davon ist Fehlergröße

        //alle k schritte sollte nun der aktuelle defekt mit aktueller iterationszahl etc
        //ausgegeben werden
        if(counter % 100 == 0){
            
            std::cout << "Aktuelle Iteration: " << counter << " ----- Aktueller Defekt: " << def << std::endl;

        }
    }
    return 0;
}




// explicit template declarations
template class IterativeSolver<Matrix>;
template class IterativeSolver<SparseMatrix>;

