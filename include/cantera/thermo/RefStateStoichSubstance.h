/**
 * @file RefStateStoichSubstance.h
 * Header file for the RefStateStoichSubstance class, which represents a fixed-composition
 * incompressible substance without any pressure dependence(see \ref thermoprops and
 * class \link Cantera::StoichSubstance StoichSubstance\endlink)
 */

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_REFSTATESTOICHSUBSTANCE_H
#define CT_REFSTATESTOICHSUBSTANCE_H

#include "StoichSubstance.h"

namespace Cantera
{

//! Class RefStateStoichSubstance represents a stoichiometric (fixed composition)
//! incompressible substance without any pressure dependence on its enthalpies.
/*!
 * This class internally changes the independent degree of freedom from density
 * to pressure. This is necessary because the phase is incompressible. It uses a
 * constant volume approximation.
 *
 * ## Specification of Species Standard State Properties
 *
 * This class inherits from StoichSubstance. It is assumed that the reference
 * state thermodynamics may be obtained by a pointer to a populated species
 * thermodynamic property manager class (see ThermoPhase::m_spthermo). This class
 * differs from StoichSubstance w.r.t. pressure changes. The pressure changes are
 * completely ignored and only the reference state thermodynamics is used at all
 * pressures. The reason for this is the desire to use a class in conjuction with
 * Surface class, where pressure related changes are not accounted for.
 * changes to the reference state thermodynamics is resolved at
 * this level.
 *
 * For an incompressible, stoichiometric substance, the molar internal energy is
 * independent of pressure. Since the thermodynamic properties are specified by
 * giving the standard-state enthalpy, the term \f$ P_0 \hat v\f$ is subtracted
 * from the specified molar enthalpy to compute the molar internal energy. The
 * entropy is assumed to be independent of the pressure.
 *
 * The enthalpy function is given by the following relation.
 *
 *       \f[
 *              h^o_k(T,P) = h^{ref}_k(T) 
 *       \f]
 *
 * For an incompressible, stoichiometric substance, the molar internal energy is
 * independent of pressure. Since the thermodynamic properties are specified by
 * giving the standard-state enthalpy, the term \f$ P_{ref} \tilde v\f$ is
 * subtracted from the specified reference molar enthalpy to compute the molar
 * internal energy.
 *
 *       \f[
 *            u^o_k(T,P) = h^{ref}_k(T) - P_{ref} \tilde v
 *       \f]
 *
 * The standard state heat capacity and entropy are independent of pressure. The
 * standard state Gibbs free energy is obtained from the enthalpy and entropy
 * functions.
 *
 * ## Specification of Solution Thermodynamic Properties
 *
 * All solution properties are obtained from the standard state species
 * functions, since there is only one species in the phase.
 *
 * ## Application within Kinetics Managers
 *
 * The standard concentration is equal to 1.0. This means that the kinetics
 * operator works on an (activities basis). Since this is a stoichiometric
 * substance, this means that the concentration of this phase drops out of
 * kinetics expressions.
 *
 * An example of a reaction using this is a sticking coefficient reaction of a
 * substance in an ideal gas phase on a surface with a bulk phase species in
 * this phase. In this case, the rate of progress for this reaction,
 * \f$ R_s \f$, may be expressed via the following equation:
 *   \f[
 *    R_s = k_s C_{gas}
 *   \f]
 * where the units for \f$ R_s \f$ are kmol m-2 s-1. \f$ C_{gas} \f$ has units
 * of kmol m-3. Therefore, the kinetic rate constant, \f$ k_s \f$, has units of
 * m s-1. Nowhere does the concentration of the bulk phase appear in the rate
 * constant expression, since it's a stoichiometric phase and the activity is
 * always equal to 1.0.
 *
 * ## Instantiation of the Class
 *
 * The constructor for this phase is NOT located in the default ThermoFactory
 * for %Cantera. However, a new StoichSubstance may be created by
 * the following code snippets:
 *
 * @code
 *    XML_Node *xm = get_XML_NameID("phase", iFile + "#NaCl(S)", 0);
 *    StoichSubstance *solid = new StoichSubstance(*xm);
 * @endcode
 *
 * or by the following call to importPhase():
 *
 * @code
 *    XML_Node *xm = get_XML_NameID("phase", iFile + "#NaCl(S)", 0);
 *    StoichSubstance solid;
 *    importPhase(*xm, &solid);
 * @endcode
 *
 * ## XML Example
 *
 * The phase model name for this is called StoichSubstance. It must be supplied
 * as the model attribute of the thermo XML element entry. Within the phase XML
 * block, the density of the phase must be specified. An example of an XML file
 * this phase is given below.
 *
 * @code
 * <!-- phase NaCl(S)    -->
 * <phase dim="3" id="NaCl(S)">
 *    <elementArray datasrc="elements.xml">
 *       Na Cl
 *    </elementArray>
 *    <speciesArray datasrc="#species_NaCl(S)"> NaCl(S) </speciesArray>
 *    <thermo model="StoichSubstance">
 *       <density units="g/cm3">2.165</density>
 *    </thermo>
 *    <transport model="None"/>
 *    <kinetics model="none"/>
 * </phase>
 *
 * <!-- species definitions     -->
 * <speciesData id="species_NaCl(S)">
 *   <!-- species NaCl(S)   -->
 *   <species name="NaCl(S)">
 *      <atomArray> Na:1 Cl:1 </atomArray>
 *      <thermo>
 *         <Shomate Pref="1 bar" Tmax="1075.0" Tmin="250.0">
 *            <floatArray size="7">
 *                50.72389, 6.672267, -2.517167,
 *                10.15934, -0.200675, -427.2115,
 *                130.3973
 *            </floatArray>
 *         </Shomate>
 *      </thermo>
 *      <density units="g/cm3">2.165</density>
 *    </species>
 * </speciesData>  @endcode
 *
 * The model attribute, "StoichSubstance", on the thermo element
 * identifies the phase as being a StoichSubstance object.
 *
 * @ingroup thermoprops
 */
class RefStateStoichSubstance : public StoichSubstance
{
public:
    //! Default constructor for the StoichSubstance class
    RefStateStoichSubstance() {}

    //! Construct and initialize a StoichSubstance ThermoPhase object directly
    //! from an ASCII input file
    /*!
     * @param infile name of the input file
     * @param id     name of the phase id in the file.
     *               If this is blank, the first phase in the file is used.
     */
    RefStateStoichSubstance(const std::string& infile, const std::string& id = "");

    //! Construct and initialize a StoichSubstance ThermoPhase object directly
    //! from an XML database
    /*!
     *  @param phaseRef XML node pointing to a StoichSubstance description
     *  @param id       Id of the phase.
     */
    RefStateStoichSubstance(XML_Node& phaseRef, const std::string& id = "");

    virtual std::string type() const {
        return "RefStateStoichSubstance";
    }

    /*
    virtual bool isCompressible() const {
        return false;
    }
    */

    /**
     * @}
     * @name Activities, Standard States, and Activity Concentrations
     *
     *  This section is largely handled by parent classes, since there
     *  is only one species. Therefore, the activity is equal to one.
     * @{
     */

    // virtual Units standardConcentrationUnits() const;

    //! This method returns an array of generalized concentrations
    /*!
     * \f$ C^a_k\f$ are defined such that \f$ a_k = C^a_k / C^0_k, \f$ where
     * \f$ C^0_k \f$ is a standard concentration defined below and \f$ a_k \f$
     * are activities used in the thermodynamic functions. These activity (or
     * generalized) concentrations are used by kinetics manager classes to
     * compute the forward and reverse rates of elementary reactions.
     *
     * For a stoichiometric substance, there is only one species, and the
     * generalized concentration is 1.0.
     *
     * @param c Output array of generalized concentrations. The
     *           units depend upon the implementation of the
     *           reaction rate expressions within the phase.
     */

    //! Return the standard concentration for the kth species
    /*!
     * The standard concentration \f$ C^0_k \f$ used to normalize the activity
     * (i.e., generalized) concentration. This phase assumes that the kinetics
     * operator works on an dimensionless basis. Thus, the standard
     * concentration is equal to 1.0.
     *
     * @param k Optional parameter indicating the species. The default
     *         is to assume this refers to species 0.
     * @return
     *   Returns The standard Concentration as 1.0
     */

    //! Get the array of chemical potentials at unit activity for the species at
    //! their standard states at the current *T* and *P* of the solution.
    /*!
     * For a stoichiometric substance, there is no activity term in the chemical
     * potential expression, and therefore the standard chemical potential and
     * the chemical potential are both equal to the molar Gibbs function.
     *
     * These are the standard state chemical potentials \f$ \mu^0_k(T,P) \f$.
     * The values are evaluated at the current temperature and pressure of the
     * solution
     *
     * @param mu0     Output vector of chemical potentials.
     *                Length: m_kk.
     */

    //@}
    /// @name  Properties of the Standard State of the Species in the Solution
    //@{

    virtual void getEnthalpy_RT(doublereal* hrt) const;
    // virtual void getEntropy_R(doublereal* sr) const;

    //! Returns the vector of nondimensional Internal Energies of the standard
    //! state species at the current *T* and *P* of the solution
    /*!
     * For an incompressible, stoichiometric substance, the molar internal
     * energy is independent of pressure. Since the thermodynamic properties
     * are specified by giving the standard-state enthalpy, the term
     * \f$ P_{ref} \hat v\f$ is subtracted from the specified reference molar
     * enthalpy to compute the standard state molar internal energy.
     *
     * @param urt  output vector of nondimensional standard state
     *             internal energies of the species. Length: m_kk.
     */
    // virtual void getIntEnergy_RT(doublereal* urt) const;

    //@}
    /// @name Thermodynamic Values for the Species Reference States
    //@{

    //! Returns the vector of nondimensional internal Energies of the reference
    //! state at the current temperature of the solution and the reference
    //! pressure for each species.
    /*!
     * @param urt    Output vector of nondimensional reference state internal
     *               energies of the species. Length: m_kk
     */
    // virtual void getIntEnergy_RT_ref(doublereal* urt) const;
    // @}

    // virtual void initThermo();
    virtual void initThermoXML(XML_Node& phaseNode, const std::string& id);

    //! Set the equation of state parameters
    /*!
     * @internal
     *
     * @param n number of parameters
     * @param c array of \a n coefficients
     *        c[0] = density of phase [ kg/m3 ]
     */
    // virtual void setParameters(int n, doublereal* const c);

    //! Get the equation of state parameters in a vector
    /*!
     * @internal
     *
     * @param n number of parameters
     * @param c array of \a n coefficients
     *
     *  For this phase:
     *       -  n = 1
     *       -  c[0] = density of phase [ kg/m3 ]
     */
    // virtual void getParameters(int& n, doublereal* const c) const;

    //! Set equation of state parameter values from XML entries.
    /*!
     * For this phase, the density of the phase is specified in this block.
     *
     * @param eosdata An XML_Node object corresponding to
     *                the "thermo" entry for this phase in the input file.
     *
     * eosdata points to the thermo block, and looks like this:
     *
     *   @code
     *   <phase id="stoichsolid" >
     *     <thermo model="StoichSubstance">
     *         <density units="g/cm3">3.52</density>
     *     </thermo>
     *   </phase>
     *   @endcode
     */
    virtual void setParametersFromXML(const XML_Node& eosdata);
};

}

#endif