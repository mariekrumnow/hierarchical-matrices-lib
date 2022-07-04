#ifndef HIERARCHICAL_MATRICES_EXCEPTIONS_H
#define HIERARCHICAL_MATRICES_EXCEPTIONS_H

#include <stdexcept>
#include <exception> //enthält what()

//fkt()noexcept{} ==in fkt darf/wird keine Exception geworfen

// try{
//       if(1==1) {
//             throw( DivByZeroException() );
//       }
// } catch( DivByZeroException exc ){
//       std::cerr << exc.what();
// }


// Exception for divisions by zero
class DivByZeroException: public std::exception{
public:
      virtual const char* what() const noexcept{
            return "Division by zero\n";
      }
};


// Exception for when the matrix inversion fails due to unchecked invertibility
class NonInvertibleMatrixException: public std::exception{
public:
      virtual const char* what() noexcept{
            return "Given matrix cannot be inverted\n";
      }
};

#endif // HIERARCHICAL_MATRICES_EXCEPTIONS_H
