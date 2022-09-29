#include <iostream>
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>

int main() {

    boost::icl::interval<double>::type interval1 = boost::icl::interval<double>::closed(34.5, 78.9);
    boost::icl::interval<double>::type interval2(11.8, 68.5);
    boost::icl::interval<double>::type interval3(0.0, 1.0);
    boost::icl::interval<double>::type interval4(99.8, 100.0);

    boost::icl::interval_set<double> intersection_set;
    boost::icl::interval_set<double> union_set;
    boost::icl::interval_set<double> difference_set;

    intersection_set = boost::icl::interval_set<double>();
    intersection_set &= boost::icl::interval_set<double>(interval2);

    std::cout << "intersection - sets: 1, 2." << std::endl;
    for (boost::icl::interval_set<double>::iterator iter = intersection_set.begin();
         iter != intersection_set.end(); iter++) {
        std::cout << *iter << std::endl;
    }


    union_set = boost::icl::interval_set<double>(interval1) +
                boost::icl::interval_set<double>(interval2) +
                boost::icl::interval_set<double>(interval3) +
                boost::icl::interval_set<double>(interval4);

    std::cout << "union - sets: 1, 2, 3, 4." << std::endl;
    for (boost::icl::interval_set<double>::iterator iter = union_set.begin();
         iter != union_set.end(); iter++) {
        std::cout << *iter << std::endl;
    }


    difference_set = static_cast <boost::icl::interval_set<double> >(interval1);
    difference_set -= boost::icl::interval_set<double>(interval2);
    difference_set -= boost::icl::interval_set<double>(interval3);

    std::cout << "difference (1-2-3) - sets: 1, 2, 3." << std::endl;
    for (boost::icl::interval_set<double>::iterator iter = difference_set.begin();
         iter != difference_set.end(); iter++) {
        std::cout << *iter << std::endl;
    }

    return 0;
}
