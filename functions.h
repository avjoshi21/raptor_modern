/*
 * Radboud Polarized Integrator
 * Copyright 2014-2020 Black Hole Cam (ERC Synergy Grant)
 * Authors: Thomas Bronzwaer, Monika Mościbrodzka
 *
 * A list of all RAPTOR functions.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

// GRMATH.C
///////////

// Lowers the index of the contravariant vector V_u, storing the results in a
// covariant one (V_d), based on the metric at position X_u
void lower_index(const double X_u[4], double V_u[4], double V_d[4]);

// Lowers two indices of a rank (2, 0) tensor
void lower_two_indices(double N_uu[4][4], double N_dd[4][4], double X_u[4]);

// Lowers the index of a contravariant vector V_u in BL coordinates.
void BL_lower_index(const double X_u[4], double V_u[4], double V_d[4]);

// Raises the index of the covariant vector V_d, storing the results in a
// contravariant one (V_u), based on the metric at position X_u
void raise_index(const double X_u[4], double V_d[4], double V_u[4]);

// Adjusts y[4] = U_u[0] so that y describes a lightray/null geodesic
void normalize_null(double X_u[4], double U_u[4]);

// Returns the norm of U_u, which is the scalar g_dd[a][b] * U_u[a] * U_u[b]
double four_velocity_norm(double X_u[4], double U_u[4]);

// Returns the inner product of vectors A and B, i.e. A_u B_d
double inner_product(double *X_u, double *A_u, double *B_u);

// Transform a contravariant vector from BL to KS coordinates
void BL_to_KS_u(double *BLphoton_u, double *KSphoton_u);

// Transform a contravariant vector from KS to BL coordinates
void KS_to_BL_u(double *KSphoton_u, double *BLphoton_u);

// Return the photon frequency in the co-moving frame of the plasma
double freq_in_plasma_frame(double Uplasma_u[4], double k_d[4]);

// Angle between k_u and B_u in the plasma frame
double pitch_angle(double *X_u, double *k_u, double *B_u, double *Uplasma_u);

// INTEGRATOR.C
///////////////

// Returns an appropriate stepsize dlambda, which depends on position & velocity
double stepsize(double X_u[4], double U_u[4]);

// Updates the vector y (containing position/velocity) by one RK4 step.
void rk4_step(double *y, void (*f)(double *, double *), double dt);

// Updates the vector y (containing position/velocity) by one RK2 step.
void rk2_step(double *y, void (*f)(double *, double *), double dt);

// Updates the vector y (containing position/velocity) by one Verlet step.
void verlet_step(double *y, void (*f)(double *, double *), double dl);

// The function to be used by the integrator - it solves the geodesic eqn.
// y contains the 4-position and the 4-velocity for one lightray/particle
void f_geodesic(double *y, double *fvector);

// Integrate the null geodesic specified by alpha and beta, store results
// in lightpath
void integrate_geodesic(double alpha, double beta, double *photon_u,
                        double *lightpath, int *steps, double cutoff_inner);

double radiative_transfer_polarized(double *lightpath, int steps,
                                    double frequency, double *f_x, double *f_y,
                                    double *p, int PRINT_POLAR, double *IQUV);

// METRIC.C
///////////

// Computes the metric at location X
void metric_dd(const double X_u[4], double g_dd[4][4]);

// Computes the inverse metric at location X
void metric_uu(const double X_u[4], double g_uu[4][4]);

// Computes the Christoffel symbols at location X numerically (general metric)
void connection_num_udd(const double X_u[4], double gamma_udd[4][4][4]);

// Computes the Christoffel symbols at location X based on an exact metric
void connection_udd(const double X_u[4], double gamma_udd[4][4][4]);

// This function initializes a single 'superphoton' or light ray.
void initialize_photon(double alpha, double beta, double k_u[4], double t_init);

// Transformation functions
double Xg2_approx_rand(double Xr2);

double Ug2_approx_rand(double Ur2, double Xg2);

// TETRAD.C
///////////

double determ(double matrix[][4], int n);

void create_tetrad(double X_u[], double k_u[],
                   double U_u[], double tetrad_u[][4]);

void create_observer_tetrad(double X_u[], double k_u[],
                            double U_u[], double b_u[],
                            double tetrad_u[][4]);

double tetrad_identity_eta(const double X_u[4], const double tetrad_u[4][4],
                           const int a, const int b);

double tetrad_identity_g(const double tetrad_u[][4], const int mu,
                         const int nu);

double tetrad_identity_sum_latin(const double tetrad_u[4][4],
                                 const double tetrad_d[4][4], const int mu,
                                 const int nu);

double tetrad_identity_sum_greek(const double tetrad_u[4][4],
                                 const double tetrad_d[4][4], const int a,
                                 const int b);

void create_tetrad_d(const double X_u[], const double tetrad_u[][4],
                     double tetrad_d[][4]);

double check_tetrad_compact(const double X_u[], const double tetrad_u[][4]);

void check_tetrad_identities(const double X_u[], double tetrad_u[][4]);

// EMISSION.C
/////////////

// Return emission coefficient j_nu for kappa distribution function
double emission_coeff_kappa_FIT(double nu, double Ne, double Thetae, double B,
                                double theta);

// Return absorption coefficient for kappa distribution function
double absorption_coeff_kappa_FIT(double nu, double Ne, double Thetae, double B,
                                  double theta);

// Return emission coefficient j_nu for thermal synchrotron radiation
double emission_coeff_THSYNCH(double B_, double theta, double THETA_e_,
                              double nu_plasma, double n_e);

// Return emission coefficient for angle-averaged thermal synchrotron radiation
double emission_coeff_THSYNCHAV(double B_, double THETA_e_, double nu_plasma,
                                double n_e);

// Return emission coefficient for thermal free-free radiation
double emission_coeff_FFTHERMAL(double nu, double n_e, double T);

// Simple emissivity model: orbiting Gaussian hotspot (see Dexter 2009)
double emissivity_hotspot(double *X_u);

// Simple emissivity model: thin disk line emission (see Dexter 2009)
double emissivity_thindisk(double *X_u);

// Return absorption coefficient a_nu
double absorption_coeff_TH(double j_nu, double nu, double THETA_e);

// Planck function
double planck_function(double nu, double THETA_e);

// Perform radiative transfer along the ray stored in "lightpath"
double radiative_transfer(double *lightpath, int steps, double frequency);

// Backward transfer
double backward_transfer(double alpha, double beta, double *photon_u,
                         int *steps);

// UTILITIES.C
//////////////

void set_constants();

// Write the array "intensityfield" (scaled by "scalefactor") to the file
// "imgfile"
void write_image(FILE *imgfile, double *intensityfield, double scalefactor);

void write_image_polarized(FILE *imgfile, double *intensityfield,
                           double *f_x_field, double *f_y_field,
                           double *p_field, double scalefactor);

void write_image_IQUV(FILE *imgfile, double *Ifield, double *Qfield,
                      double *Ufield, double *Vfield, double scalefactor);

// Write the arrays "intensityfield" (scaled by "scalefactor") and "lambdafield"
// to a VTK file
void write_VTK_image(FILE *imgfile, double *intensityfield, double *lambdafield,
                     double scalefactor);

// RAPTOR_HARM_MODEL.C
//////////////////////

// See grmonty paper by Dolence et al.
// HARM model internal utilities
void init_model();

void set_units(double);

void init_harm3d_data(char *fname);

void init_storage();

double interp_scalar(double ***var, int i, int j, int k, double coeff[4]);

void Xtoijk(double *X, int *i, int *j, int *k, double *del);

void get_fluid_params(double X[4], double *Ne, double *Thetae, double *B,
                      double *B_u, double Ucon[4], int *IN_VOLUME);

#endif // FUNCTIONS_H
