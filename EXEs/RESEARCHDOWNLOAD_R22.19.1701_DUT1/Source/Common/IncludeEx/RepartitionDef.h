#pragma once

// Do repartition always
#define REPAR_STRATEGY_DO_ALWAYS        0
// Stop actions and report error when incompatible partition error occured
#define REPAR_STRATEGY_STOP             1
// Ignore incompatible partition error
#define REPAR_STRATEGY_IGNORE           2
// Do repartion action when imcompatible partition error occured
#define REPAR_STRATEGY_DO               3