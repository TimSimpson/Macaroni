//#ifndef MACARONI_CPP_H
//#define MACARONI_CPP_H
//
///*******************************************************************************
// *	WARNING!
// *  This header file makes the namespace boost::spirit visible!
// ******************************************************************************/
//
//#include <boost/spirit/core.hpp>
//
//namespace Macaroni { namespace Cpp {
//
//using namespace boost::spirit;
//
//struct Mac : public grammar<Mac>
//{
//    template <typename ScannerT>
//    struct definition
//    {
//        definition(Mac const& self)
//        {
//            group       = '(' >> expression >> ')';
//            factor      = integer | group;
//            term        = factor >> *(('*' >> factor) | ('/' >> factor));
//            expression  = term >> *(('+' >> term) | ('-' >> term));
//        }
//
//        rule<ScannerT> expression, term, factor, group;
//
//        rule<ScannerT> const&
//        start() const { return expression; }
//    };
//};
//
//} } // End namespaces
//
//
//#endif
//
//
