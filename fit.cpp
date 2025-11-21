// ============================================================================
// Author: João Vitor Costa Lovato
// Email:  joaovitorcl1000@outlook.com
// Date:   2025
//
// Title:  Parameter Estimation in Radioactive Decay using ROOT Minuit2
//
// Description:
//   This program performs a two-parameter fit of the radioactive decay law:
//
//        A(t) = A0 * exp(-lambda * t)
//
//   where:
//      - A0     : initial activity (fit parameter)
//      - lambda : decay constant (fit parameter)
//
//   Pseudo-experimental data with asymmetric uncertainties are provided, and
//   the χ² function is minimized using ROOT's Minuit2 (Migrad algorithm).
//
//   This example illustrates:
//      * How to build and evaluate a physical model;
//      * How to construct a χ² with asymmetric errors;
//      * How to set up a ROOT::Math::Functor for Minuit2;
//      * How to perform a nonlinear least-squares fit in C++;
//      * How to interpret best-fit parameters and uncertainties;
//
//   This code is intended as a clean, educational, and professional example
//   of scientific parameter estimation in C++.
//
// ============================================================================

#include <iostream>
#include <cmath>
#include <vector>

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/MinimizerOptions.h"
#include "TError.h"

using namespace ROOT::Math;
using namespace std;

// ---------------------------------------------------
// Data structure
// ---------------------------------------------------

struct datapoint
{
    double exp;      // measured activity A(t)
    double t;        // time (e.g. days)
    double err_mnus; // negative error
    double err_plus; // positive error
    double err;      // combined error (unused here, but stored)
};

// ---------------------------------------------------
// Pseudo-data (A0_true = 1000, lambda_true = 0.1)
// ---------------------------------------------------

const vector<datapoint> Data =
{
    { 995.0,  0.0, 30.0, 30.0, sqrt(30.0*30.0 + 30.0*30.0) },
    { 615.0,  5.0, 20.0, 20.0, sqrt(20.0*20.0 + 20.0*20.0) },
    { 375.0, 10.0, 15.0, 15.0, sqrt(15.0*15.0 + 15.0*15.0) },
    { 220.0, 15.0, 10.0, 10.0, sqrt(10.0*10.0 + 10.0*10.0) },
    { 140.0, 20.0,  8.0,  8.0, sqrt( 8.0* 8.0 +  8.0* 8.0) },
    {  85.0, 25.0,  5.0,  5.0, sqrt( 5.0* 5.0 +  5.0* 5.0) },
    {  51.0, 30.0,  4.0,  4.0, sqrt( 4.0* 4.0 +  4.0* 4.0) },
    {  32.0, 35.0,  3.0,  3.0, sqrt( 3.0* 3.0 +  3.0* 3.0) },
    {  17.5, 40.0,  2.0,  2.0, sqrt( 2.0* 2.0 +  2.0* 2.0) }
};

// ---------------------------------------------------
// Theoretical model: A(t) = A0 * exp(-lambda * t)
// Parameters: xx[0] = lambda, xx[1] = A0
// ---------------------------------------------------

double Prediction(const double* xx, double t)
{
    double lambda = xx[0];
    double A0     = xx[1];
    return A0 * exp(-lambda * t);
}

// ---------------------------------------------------
// Chi-square with asymmetric errors
// ---------------------------------------------------

double chi2(const double* xx, const vector<datapoint>& exp_data)
{
    double tot = 0.0;

    for (const auto& dp : exp_data)
    {
        double pred = Prediction(xx, dp.t);
        double diff = pred - dp.exp;

        if (diff > 0.0) {
            // use upper error if prediction is above the data
            tot += (diff * diff) / (dp.err_plus * dp.err_plus);
        } else {
            // use lower error if prediction is below the data
            tot += (diff * diff) / (dp.err_mnus * dp.err_mnus);
        }
    }

    return tot;
}

// Wrapper for ROOT::Math::Functor (no extra arguments)
double chi2_wrapper(const double* xx)
{
    return chi2(xx, Data);
}

// ---------------------------------------------------
// Main
// ---------------------------------------------------

int main()
{
    // Optional: reduce ROOT warnings
    gErrorIgnoreLevel = kWarning;

    // Create the Minuit2 minimizer with Migrad
    Minimizer* minimum = Factory::CreateMinimizer("Minuit2", "Migrad");
    if (!minimum) {
        cerr << "Error: cannot create Minuit2/Migrad minimizer.\n";
        return 1;
    }

    const unsigned int Dim = 2; // Two parameters lambda, A0
    Functor f(&chi2_wrapper, Dim);
    minimum->SetFunction(f);

    // Initial guesses and step sizes
    double lambda_0 = 0.2; // initial guess for lambda
    double A0_0     = 900.0; // initial guess for A0
    double step_lambda = 0.01;
    double step_A0     = 10.0;

    minimum->SetVariable(0, "lambda", lambda_0, step_lambda);
    minimum->SetVariable(1, "A0",     A0_0,     step_A0);

    minimum->SetStrategy(2);  // more thorough
    minimum->SetMaxFunctionCalls(10000); //max calls
    minimum->SetMaxIterations(10000);
    minimum->SetTolerance(1e-6);

    bool success = minimum->Minimize();
    const double* best_params = minimum->X();
    const double* errors      = minimum->Errors();

    double lambda_fit = best_params[0];
    double A0_fit     = best_params[1];

    cout << "Fit success: " << (success ? "yes" : "no") << "\n";
    cout << "lambda = " << lambda_fit << " ± " << errors[0] << "\n";
    cout << "A0     = " << A0_fit     << " ± " << errors[1] << "\n";

    cout << "chi2   = " << minimum->MinValue()
         << " (Npoints = " << Data.size()
         << ", Npar = " << Dim << ")\n";

    delete minimum;
    return 0;
}

