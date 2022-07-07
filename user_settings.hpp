// This file includes necessary settings for you, the user
// ONLY CHANGE THE PARTS THAT ARE EXPLAINED IN THE COMMENTS
// Otherwise, being able to run the code cannot be guaranteed

// Change the number according to the format of the numbers in your matrices
// The default is float
// 0 = float
// 1 = double
// 2 = complex float
// 3 = complex double
#define USED_DATATYPE 1





// DO NOT CHANGE ANYTHING BEYOND THIS POINT

#if USED_DATATYPE == 1
      #define CALC_SVD LAPACKE_dgesvd_work
#elif USED_DATATYPE == 2
      #define CALC_SVD LAPACKE_cgesvd_work
#elif USED_DATATYPE == 3
      #define CALC_SVD LAPACKE_zgesvd_work
#else
      #define CALC_SVD LAPACKE_sgesvd_work
#endif
