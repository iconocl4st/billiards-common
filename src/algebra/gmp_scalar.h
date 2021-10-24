////
//// Created by thallock on 10/22/21.
////
//
//#ifndef IDEA_GMP_SCALAR_H
//#define IDEA_GMP_SCALAR_H
//#include <gmpxx.h>
//#include <Eigen/Core>
//#include <boost/operators.hpp>
//
//namespace Eigen {
//	template<> struct NumTraits<mpf_class> : GenericNumTraits<mpf_class>
//	{
//		typedef mpf_class Real;
//		typedef mpf_class NonInteger;
//		typedef mpf_class Nested;
//
//		static inline Real epsilon() { return 0; }
//		static inline Real dummy_precision() { return 0; }
//		static inline int digits10() { return 0; }
//
//		enum {
//			IsInteger = 0,
//			IsSigned = 1,
//			IsComplex = 0,
//			RequireInitialization = 1,
//			ReadCost = 6,
//			AddCost = 150,
//			MulCost = 100
//		};
//	};
//}
//
//
//#endif //IDEA_GMP_SCALAR_H
