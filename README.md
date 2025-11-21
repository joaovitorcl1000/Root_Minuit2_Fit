# 💥 Root_Minuit2_Fit

## Non-Linear Optimization for the Radioactive Decay Law

This repository implements **non-linear parameter estimation** (fitting $A_0$ and $\lambda$) for the **Radioactive Decay Law** ($N(t) = A_0 e^{-\lambda t}$) using the **Minuit2** optimization library within the **ROOT** data analysis framework.

The project focuses on using the **Chi-square** ($\chi^2$) method and is optimized to handle **asymmetric errors** in the statistical analysis, a powerful feature provided by Minuit2 (via the **MINOS** algorithm).

---

### Implementation Details

#### About Minuit2

Minuit2 is an object-oriented (C++) re-implementation of the popular **MINUIT** minimization package, originally developed at CERN. It is the core tool in **statistical analysis** and **High Energy Physics** for:

1.  **Finding the minimum** value of a multi-parameter function (the *FCN*, typically $\chi^2$ or *log-likelihood*).
2.  **Analyzing the function shape** around the minimum to calculate the **best-fit parameter values** and their **uncertainties** (errors).

* **Algorithms:** Minuit2 employs robust algorithms such as **MIGRAD** (for minimization) and **MINOS** (for precise, asymmetric error calculation).
* **$\chi^2$ Optimization:** The package includes the **FUMILI** algorithm, which is specifically optimized for least square ($\chi^2$) and log-likelihood minimizations, ensuring high numerical accuracy and performance.

#### Installation and Usage

To compile and run this project, the **ROOT** framework must be installed, as it distributes Minuit2.

* ROOT installation instructions can be found here: **https://root.cern/**

The main entry point for minimization within this project utilizes the **`ROOT::Math::Minimizer`** interface, typically by instantiating the **`ROOT::Minuit2::Minuit2Minimizer`** class.

---

## 🔬 Fitted Equation (External)

The function fitted via the Chi-Square method is the radioactive decay law:

$$N(t) = A_0 e^{-\lambda t}$$

Where:
* $N(t)$ is the number of nuclei as a function of time.
* $A_0$ is the initial number of nuclei (parameter to be estimated).
* $\lambda$ is the decay constant (parameter to be estimated).
* $t$ is time.
